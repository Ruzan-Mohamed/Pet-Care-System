#pragma once
using namespace System;

namespace PawCare
{
    public ref class Appointment
    {
    private:
        int appointmentID;
        int petID;
        int veterinarianID;
        String^ appointmentDate;
        String^ appointmentTime;
        String^ reason;
        String^ status;

    public:
        Appointment();

        property int AppointmentID
        {
            int get();
            void set(int value);
        }

        property int PetID
        {
            int get();
            void set(int value);
        }

        property int VeterinarianID
        {
            int get();
            void set(int value);
        }

        property String^ AppointmentDate
        {
            String^ get();
            void set(String^ value);
        }

        property String^ AppointmentTime
        {
            String^ get();
            void set(String^ value);
        }

        property String^ Reason
        {
            String^ get();
            void set(String^ value);
        }

        property String^ Status
        {
            String^ get();
            void set(String^ value);
        }

        void createAppointment();
        void updateAppointment(String^ date, String^ time, String^ newReason);
        void cancelAppointment();
        void changeStatus(String^ newStatus);
        String^ displayAppointment();
    };
}
