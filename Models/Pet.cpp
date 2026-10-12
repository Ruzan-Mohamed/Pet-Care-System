#include "Pet.h"

using namespace PawCare;

Pet::Pet()
{
    petID = 0;
    ownerID = 0;
    name = "";
    petType = "";
    breed = "";
    age = 0;
    gender = "";
}

Pet::Pet(int id, int owner, String^ petName, String^ type,
         String^ petBreed, int petAge, String^ petGender)
{
    petID = id;
    ownerID = owner;
    name = petName;
    petType = type;
    breed = petBreed;
    Age = petAge;
    gender = petGender;
}

int Pet::PetID::get() { return petID; }
void Pet::PetID::set(int value) { petID = value; }

int Pet::OwnerID::get() { return ownerID; }
void Pet::OwnerID::set(int value) { ownerID = value; }

String^ Pet::Name::get() { return name; }
void Pet::Name::set(String^ value) { name = value; }

String^ Pet::PetType::get() { return petType; }
void Pet::PetType::set(String^ value) { petType = value; }

String^ Pet::Breed::get() { return breed; }
void Pet::Breed::set(String^ value) { breed = value; }

int Pet::Age::get() { return age; }
void Pet::Age::set(int value)
{
    if (value >= 0)
        age = value;
    else
        age = 0;
}

String^ Pet::Gender::get() { return gender; }
void Pet::Gender::set(String^ value) { gender = value; }

double Pet::calculateConsultationFee()
{
    return 1000.00;
}

String^ Pet::displayPetInfo()
{
    return "Pet: " + name +
           "\r\nType: " + petType +
           "\r\nBreed: " + breed +
           "\r\nAge: " + age.ToString() +
           "\r\nGender: " + gender;
}
