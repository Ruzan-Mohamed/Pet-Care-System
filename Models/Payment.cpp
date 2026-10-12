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
    paymentMethod = "Cash";
    paymentStatus = "PENDING";
}

int Payment::PaymentID::get() { return paymentID; }
void Payment::PaymentID::set(int value) { paymentID = value; }

int Payment::AppointmentID::get() { return appointmentID; }
void Payment::AppointmentID::set(int value) { appointmentID = value; }

double Payment::ConsultationFee::get() { return consultationFee; }
void Payment::ConsultationFee::set(double value)
{
    consultationFee = value >= 0 ? value : 0.0;
}

double Payment::TreatmentFee::get() { return treatmentFee; }
void Payment::TreatmentFee::set(double value)
{
    treatmentFee = value >= 0 ? value : 0.0;
}

double Payment::OtherFee::get() { return otherFee; }
void Payment::OtherFee::set(double value)
{
    otherFee = value >= 0 ? value : 0.0;
}

double Payment::TotalAmount::get()
{
    return totalAmount;
}

String^ Payment::PaymentMethod::get() { return paymentMethod; }
void Payment::PaymentMethod::set(String^ value) { paymentMethod = value; }

String^ Payment::PaymentStatus::get() { return paymentStatus; }
void Payment::PaymentStatus::set(String^ value) { paymentStatus = value; }

double Payment::calculateTotal()
{
    totalAmount = consultationFee + treatmentFee + otherFee;
    return totalAmount;
}

void Payment::processPayment()
{
    calculateTotal();
    paymentStatus = "PAID";
}

String^ Payment::displayPayment()
{
    return "Consultation Fee: Rs. " + consultationFee.ToString("F2") +
           "\r\nTreatment Fee: Rs. " + treatmentFee.ToString("F2") +
           "\r\nOther Fee: Rs. " + otherFee.ToString("F2") +
           "\r\nTotal: Rs. " + totalAmount.ToString("F2") +
           "\r\nMethod: " + paymentMethod +
           "\r\nStatus: " + paymentStatus;
}
