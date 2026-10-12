#pragma once
using namespace System;

namespace PawCare
{
    public ref class Owner
    {
    private:
        int ownerID;
        String^ name;
        String^ telephone;
        String^ email;
        String^ address;

    public:
        Owner();

        property int OwnerID
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

        property String^ Email
        {
            String^ get();
            void set(String^ value);
        }

        property String^ Address
        {
            String^ get();
            void set(String^ value);
        }
    };
}
