#include "Appointment.h"

using namespace PawCare;
using namespace System::Globalization;

Appointment::Appointment()
{
    appointmentID = 0;
    petID = 0;
    veterinarianID = 0;
    appointmentDate = "";
    appointmentTime = "";
    reason = "";
    status = STATUS_BOOKED;
}

Appointment::Appointment(int id, int pId, int vetId, String^ date, String^ time, String^ apptReason)
{
    AppointmentID = id;
    PetID = pId;
    VeterinarianID = vetId;
    AppointmentDate = date;
    AppointmentTime = time;
    Reason = apptReason;
    status = STATUS_BOOKED;
}

Appointment::Appointment(int id, int pId, int vetId, String^ date, String^ time, String^ apptReason, String^ apptStatus)
{
    AppointmentID = id;
    PetID = pId;
    VeterinarianID = vetId;
    AppointmentDate = date;
    AppointmentTime = time;
    Reason = apptReason;
    setPersistedStatus(apptStatus);
}

void Appointment::validateId(int id, String^ fieldName)
{
    if (id < 0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, fieldName + " cannot be negative.");
    }
}

void Appointment::validateDateTime(String^ date, String^ time)
{
    if (String::IsNullOrWhiteSpace(date) || !isValidDateFormat(date))
    {
        throw gcnew ArgumentException("Invalid date format. Expected yyyy-MM-dd.");
    }
    if (String::IsNullOrWhiteSpace(time) || !isValidTimeFormat(time))
    {
        throw gcnew ArgumentException("Invalid time format. Expected HH:mm.");
    }
}

int Appointment::AppointmentID::get() { return appointmentID; }
void Appointment::AppointmentID::set(int value)
{
    validateId(value, "AppointmentID");
    appointmentID = value;
}

int Appointment::PetID::get() { return petID; }
void Appointment::PetID::set(int value)
{
    validateId(value, "PetID");
    petID = value;
}

int Appointment::VeterinarianID::get() { return veterinarianID; }
void Appointment::VeterinarianID::set(int value)
{
    validateId(value, "VeterinarianID");
    veterinarianID = value;
}

String^ Appointment::AppointmentDate::get() { return appointmentDate; }
void Appointment::AppointmentDate::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Appointment date cannot be null or empty.");
    }
    if (!isValidDateFormat(value))
    {
        throw gcnew ArgumentException("Invalid date format. Expected yyyy-MM-dd.");
    }
    appointmentDate = value->Trim();
}

String^ Appointment::AppointmentTime::get() { return appointmentTime; }
void Appointment::AppointmentTime::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Appointment time cannot be null or empty.");
    }
    if (!isValidTimeFormat(value))
    {
        throw gcnew ArgumentException("Invalid time format. Expected HH:mm (24-hour).");
    }
    appointmentTime = value->Trim();
}

String^ Appointment::Reason::get() { return reason; }
void Appointment::Reason::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Reason for appointment cannot be null or empty.");
    }
    reason = value->Trim();
}

String^ Appointment::Status::get() { return status; }
void Appointment::Status::set(String^ value)
{
    setPersistedStatus(value);
}

bool Appointment::isModifiable()
{
    return (status != STATUS_CANCELLED && status != STATUS_COMPLETED);
}

void Appointment::createAppointment()
{
    if (petID <= 0 || veterinarianID <= 0)
    {
        throw gcnew InvalidOperationException("Valid Pet ID and Veterinarian ID are required to create an appointment.");
    }
    if (String::IsNullOrWhiteSpace(appointmentDate) || String::IsNullOrWhiteSpace(appointmentTime))
    {
        throw gcnew InvalidOperationException("Appointment date and time must be set before booking.");
    }
    status = STATUS_BOOKED;
}

void Appointment::updateAppointment(String^ date, String^ time, String^ newReason)
{
    if (!isModifiable())
    {
        throw gcnew InvalidOperationException("Cannot update an appointment that is already " + status + ".");
    }

    AppointmentDate = date;
    AppointmentTime = time;
    Reason = newReason;
}

void Appointment::confirmAppointment()
{
    changeStatus(STATUS_CONFIRMED);
}

void Appointment::completeAppointment()
{
    changeStatus(STATUS_COMPLETED);
}

void Appointment::cancelAppointment()
{
    if (status == STATUS_COMPLETED)
    {
        throw gcnew InvalidOperationException("Cannot cancel an appointment that is already COMPLETED.");
    }
    status = STATUS_CANCELLED;
}

void Appointment::changeStatus(String^ newStatus)
{
    if (String::IsNullOrWhiteSpace(newStatus) || !isValidStatus(newStatus))
    {
        throw gcnew ArgumentException("Invalid appointment status: " + newStatus);
    }

    if (status == newStatus) return;

    if (!isValidTransition(status, newStatus))
    {
        throw gcnew InvalidOperationException("Invalid status transition from " + status + " to " + newStatus + ".");
    }

    status = newStatus;
}

void Appointment::setPersistedStatus(String^ persistedStatus)
{
    if (!String::IsNullOrWhiteSpace(persistedStatus) && isValidStatus(persistedStatus->Trim()))
    {
        status = persistedStatus->Trim();
    }
    else
    {
        status = STATUS_BOOKED;
    }
}

String^ Appointment::displayAppointment()
{
    return "Appointment ID: " + appointmentID.ToString() +
           "\r\nPet ID: " + petID.ToString() +
           "\r\nVeterinarian ID: " + veterinarianID.ToString() +
           "\r\nDate: " + appointmentDate +
           "\r\nTime: " + appointmentTime +
           "\r\nReason: " + reason +
           "\r\nStatus: " + status;
}

bool Appointment::isValidDateFormat(String^ dateStr)
{
    if (String::IsNullOrWhiteSpace(dateStr)) return false;
    DateTime parsed;
    return DateTime::TryParseExact(dateStr->Trim(), "yyyy-MM-dd",
        CultureInfo::InvariantCulture, DateTimeStyles::None, parsed);
}

bool Appointment::isValidTimeFormat(String^ timeStr)
{
    if (String::IsNullOrWhiteSpace(timeStr)) return false;
    DateTime parsed;
    return DateTime::TryParseExact(timeStr->Trim(), "HH:mm",
        CultureInfo::InvariantCulture, DateTimeStyles::None, parsed);
}

bool Appointment::isValidStatus(String^ statusStr)
{
    if (String::IsNullOrWhiteSpace(statusStr)) return false;
    return (statusStr == STATUS_BOOKED ||
            statusStr == STATUS_CONFIRMED ||
            statusStr == STATUS_COMPLETED ||
            statusStr == STATUS_CANCELLED);
}

bool Appointment::isValidTransition(String^ currentStatus, String^ newStatus)
{
    if (currentStatus == STATUS_CANCELLED || currentStatus == STATUS_COMPLETED)
    {
        // Terminal states cannot transition to other states
        return false;
    }

    if (currentStatus == STATUS_BOOKED)
    {
        return (newStatus == STATUS_CONFIRMED || newStatus == STATUS_COMPLETED || newStatus == STATUS_CANCELLED);
    }

    if (currentStatus == STATUS_CONFIRMED)
    {
        return (newStatus == STATUS_COMPLETED || newStatus == STATUS_CANCELLED);
    }

    return false;
}

