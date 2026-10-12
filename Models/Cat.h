#pragma once
#include "Pet.h"

namespace PawCare
{
    public ref class Cat : public Pet
    {
    public:
        Cat();
        Cat(int id, int owner, String^ petName, String^ petBreed,
            int petAge, String^ petGender);

        virtual double calculateConsultationFee() override;
        virtual String^ displayPetInfo() override;
    };
}
