#include "Payment.h"

using namespace PawCare;

Payment::Payment()
{
    paymentID = 0;
    appointmentID = 0;
    consultationFee = 0.0;
    treatmentFee = 0.0;
    otherFee = 0.0;
    totalAmount = 0.0;
    paymentMethod = METHOD_CASH;
    paymentStatus = STATUS_PENDING;
}

Payment::Payment(int id, int apptId, double consultFee, double treatFee, double othFee, String^ method)
{
    PaymentID = id;
    AppointmentID = apptId;
    validateFee(consultFee, "ConsultationFee");
    validateFee(treatFee, "TreatmentFee");
    validateFee(othFee, "OtherFee");

    consultationFee = Math::Round(consultFee, 2);
    treatmentFee = Math::Round(treatFee, 2);
    otherFee = Math::Round(othFee, 2);
    PaymentMethod = method;
    paymentStatus = STATUS_PENDING;
    recalculateTotal();
}

void Payment::validateFee(double value, String^ fieldName)
{
    if (Double::IsNaN(value) || Double::IsInfinity(value))
    {
        throw gcnew ArgumentException(fieldName + " cannot be NaN or Infinity.");
    }
    if (value < 0.0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, "Fee cannot be negative.");
    }
}

void Payment::recalculateTotal()
{
    totalAmount = Math::Round(consultationFee + treatmentFee + otherFee, 2);
}

int Payment::PaymentID::get() { return paymentID; }
void Payment::PaymentID::set(int value)
{
    if (value < 0)
        throw gcnew ArgumentOutOfRangeException("PaymentID", "Payment ID cannot be negative.");
    paymentID = value;
}

int Payment::AppointmentID::get() { return appointmentID; }
void Payment::AppointmentID::set(int value)
{
    if (value < 0)
        throw gcnew ArgumentOutOfRangeException("AppointmentID", "Appointment ID cannot be negative.");
    appointmentID = value;
}

double Payment::ConsultationFee::get() { return consultationFee; }
void Payment::ConsultationFee::set(double value)
{
    validateFee(value, "ConsultationFee");
    consultationFee = Math::Round(value, 2);
    recalculateTotal();
}

double Payment::TreatmentFee::get() { return treatmentFee; }
void Payment::TreatmentFee::set(double value)
{
    validateFee(value, "TreatmentFee");
    treatmentFee = Math::Round(value, 2);
    recalculateTotal();
}

double Payment::OtherFee::get() { return otherFee; }
void Payment::OtherFee::set(double value)
{
    validateFee(value, "OtherFee");
    otherFee = Math::Round(value, 2);
    recalculateTotal();
}

double Payment::TotalAmount::get()
{
    // Auto-recalculate to guarantee total is never stale
    recalculateTotal();
    return totalAmount;
}

String^ Payment::PaymentMethod::get() { return paymentMethod; }
void Payment::PaymentMethod::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value) || !isValidMethod(value))
    {
        throw gcnew ArgumentException("Invalid payment method. Allowed methods: Cash, Card, Bank Transfer, Online.");
    }
    paymentMethod = value;
}

String^ Payment::PaymentStatus::get() { return paymentStatus; }
void Payment::PaymentStatus::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value) || !isValidStatus(value))
    {
        throw gcnew ArgumentException("Invalid payment status. Allowed statuses: PENDING, PAID, FAILED, CANCELLED, REFUNDED.");
    }
    paymentStatus = value;
}

double Payment::calculateTotal()
{
    recalculateTotal();
    return totalAmount;
}

bool Payment::processPayment()
{
    // Cannot pay an appointment without an ID
    if (appointmentID <= 0)
    {
        throw gcnew InvalidOperationException("Cannot process payment without a valid Appointment ID.");
    }

    // Recalculate total before validation
    recalculateTotal();

    if (totalAmount <= 0.0)
    {
        throw gcnew InvalidOperationException("Total payment amount must be greater than zero.");
    }

    // Do not process already paid or cancelled payments
    if (paymentStatus == STATUS_PAID)
    {
        throw gcnew InvalidOperationException("Payment has already been processed.");
    }

    if (paymentStatus == STATUS_CANCELLED)
    {
        throw gcnew InvalidOperationException("Cannot process a cancelled payment.");
    }

    paymentStatus = STATUS_PAID;
    return true;
}

void Payment::markFailed()
{
    if (paymentStatus != STATUS_PAID)
    {
        paymentStatus = STATUS_FAILED;
    }
}

void Payment::markRefunded()
{
    if (paymentStatus == STATUS_PAID)
    {
        paymentStatus = STATUS_REFUNDED;
    }
    else
    {
        throw gcnew InvalidOperationException("Only PAID payments can be refunded.");
    }
}

String^ Payment::displayPayment()
{
    recalculateTotal();
    return "Payment ID: " + paymentID.ToString() +
           "\r\nAppointment ID: " + appointmentID.ToString() +
           "\r\nConsultation Fee: Rs. " + consultationFee.ToString("F2") +
           "\r\nTreatment Fee: Rs. " + treatmentFee.ToString("F2") +
           "\r\nOther Fee: Rs. " + otherFee.ToString("F2") +
           "\r\nTotal: Rs. " + totalAmount.ToString("F2") +
           "\r\nMethod: " + paymentMethod +
           "\r\nStatus: " + paymentStatus;
}

bool Payment::isValidMethod(String^ method)
{
    if (String::IsNullOrWhiteSpace(method)) return false;
    return (method == METHOD_CASH ||
            method == METHOD_CARD ||
            method == METHOD_BANK_TRANSFER ||
            method == METHOD_ONLINE);
}

bool Payment::isValidStatus(String^ status)
{
    if (String::IsNullOrWhiteSpace(status)) return false;
    return (status == STATUS_PENDING ||
            status == STATUS_PAID ||
            status == STATUS_FAILED ||
            status == STATUS_CANCELLED ||
            status == STATUS_REFUNDED);
}

