#pragma once
using namespace System;

namespace PawCare
{
    public ref class Veterinarian
    {
    private:
        int veterinarianID;
        String^ name;
        String^ telephone;
        String^ specialization;

    public:
        Veterinarian();

        property int VeterinarianID
        {
            int get();
            void set(int value);
        }

        property String^ Name
        {
            String^ get();
            void set(String^ value);
        }

        property String^ Telephone
        {
            String^ get();
            void set(String^ value);
        }

        property String^ Specialization
        {
            String^ get();
            void set(String^ value);
        }
    };
}
