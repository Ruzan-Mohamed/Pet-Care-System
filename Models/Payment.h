#pragma once
using namespace System;

namespace PawCare
{
    public ref class Payment
    {
    private:
        int paymentID;
        int appointmentID;
        double consultationFee;
        double treatmentFee;
        double otherFee;
        double totalAmount;
        String^ paymentMethod;
        String^ paymentStatus;

    public:
        Payment();

        property int PaymentID
        {
            int get();
            void set(int value);
        }

        property int AppointmentID
        {
            int get();
            void set(int value);
        }

        property double ConsultationFee
        {
            double get();
            void set(double value);
        }

        property double TreatmentFee
        {
            double get();
            void set(double value);
        }

        property double OtherFee
        {
            double get();
            void set(double value);
        }

        property double TotalAmount
        {
            double get();
        }

        property String^ PaymentMethod
        {
            String^ get();
            void set(String^ value);
        }

        property String^ PaymentStatus
        {
            String^ get();
            void set(String^ value);
        }

        double calculateTotal();
        void processPayment();
        String^ displayPayment();
    };
}
