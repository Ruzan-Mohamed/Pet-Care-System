#include "Appointment.h"

using namespace PawCare;

Appointment::Appointment()
{
    appointmentID = 0;
    petID = 0;
    veterinarianID = 0;
    appointmentDate = "";
    appointmentTime = "";
    reason = "";
    status = "BOOKED";
}

int Appointment::AppointmentID::get() { return appointmentID; }
void Appointment::AppointmentID::set(int value) { appointmentID = value; }

int Appointment::PetID::get() { return petID; }
void Appointment::PetID::set(int value) { petID = value; }

int Appointment::VeterinarianID::get() { return veterinarianID; }
void Appointment::VeterinarianID::set(int value) { veterinarianID = value; }

String^ Appointment::AppointmentDate::get() { return appointmentDate; }
void Appointment::AppointmentDate::set(String^ value) { appointmentDate = value; }

String^ Appointment::AppointmentTime::get() { return appointmentTime; }
void Appointment::AppointmentTime::set(String^ value) { appointmentTime = value; }

String^ Appointment::Reason::get() { return reason; }
void Appointment::Reason::set(String^ value) { reason = value; }

String^ Appointment::Status::get() { return status; }
void Appointment::Status::set(String^ value) { status = value; }

void Appointment::createAppointment()
{
    status = "BOOKED";
}

void Appointment::updateAppointment(String^ date, String^ time, String^ newReason)
{
    appointmentDate = date;
    appointmentTime = time;
    reason = newReason;
}

void Appointment::cancelAppointment()
{
    status = "CANCELLED";
}

void Appointment::changeStatus(String^ newStatus)
{
    status = newStatus;
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
