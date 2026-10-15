#include "Pet.h"

using namespace PawCare;

Pet::Pet()
{
    petID = 0;
    ownerID = 0;
    name = "";
    petType = "General";
    breed = "";
    age = 0;
    gender = "Unknown";
}

Pet::Pet(int id, int owner, String^ petName, String^ type,
         String^ petBreed, int petAge, String^ petGender)
{
    PetID = id;
    OwnerID = owner;
    Name = petName;
    PetType = type;
    Breed = petBreed;
    Age = petAge;
    Gender = petGender;
}

void Pet::validateId(int id, String^ fieldName)
{
    if (id < 0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, fieldName + " cannot be negative.");
    }
}

int Pet::PetID::get() { return petID; }
void Pet::PetID::set(int value)
{
    validateId(value, "PetID");
    petID = value;
}

int Pet::OwnerID::get() { return ownerID; }
void Pet::OwnerID::set(int value)
{
    validateId(value, "OwnerID");
    ownerID = value;
}

String^ Pet::Name::get() { return name; }
void Pet::Name::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Pet name cannot be null or whitespace.");
    }
    name = value->Trim();
}

String^ Pet::PetType::get() { return petType; }
void Pet::PetType::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Pet type cannot be null or whitespace.");
    }
    petType = value->Trim();
}

String^ Pet::Breed::get() { return breed; }
void Pet::Breed::set(String^ value)
{
    breed = (value == nullptr) ? "" : value->Trim();
}

int Pet::Age::get() { return age; }
void Pet::Age::set(int value)
{
    if (value < 0 || value > 100)
    {
        throw gcnew ArgumentOutOfRangeException("Age", "Pet age must be between 0 and 100 years.");
    }
    age = value;
}

String^ Pet::Gender::get() { return gender; }
void Pet::Gender::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value) || !isValidGender(value))
    {
        throw gcnew ArgumentException("Gender must be 'Male', 'Female', or 'Unknown'.");
    }
    gender = value->Trim();
}

double Pet::calculateConsultationFee()
{
    return BASE_CONSULTATION_FEE;
}

String^ Pet::displayPetInfo()
{
    return "Pet ID: " + petID.ToString() +
           "\r\nPet: " + name +
           "\r\nType: " + petType +
           "\r\nBreed: " + breed +
           "\r\nAge: " + age.ToString() + " years" +
           "\r\nGender: " + gender;
}

bool Pet::isValidGender(String^ g)
{
    if (String::IsNullOrWhiteSpace(g)) return false;
    String^ trimmed = g->Trim();
    return (String::Equals(trimmed, "Male", StringComparison::OrdinalIgnoreCase) ||
            String::Equals(trimmed, "Female", StringComparison::OrdinalIgnoreCase) ||
            String::Equals(trimmed, "Unknown", StringComparison::OrdinalIgnoreCase));
}

