#include "Dog.h"

using namespace PawCare;

Dog::Dog() : Pet()
{
    PetType = "Dog";
}

Dog::Dog(int id, int owner, String^ petName, String^ petBreed,
         int petAge, String^ petGender)
    : Pet(id, owner, petName, "Dog", petBreed, petAge, petGender)
{
}

double Dog::calculateConsultationFee()
{
    return 1200.00;
}

String^ Dog::displayPetInfo()
{
    return "[DOG]\r\n" + Pet::displayPetInfo();
}
