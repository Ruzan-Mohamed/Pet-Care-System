#include "Veterinarian.h"

using namespace PawCare;
using namespace System::Text::RegularExpressions;

Veterinarian::Veterinarian()
{
    veterinarianID = 0;
    name = "";
    telephone = "";
    specialization = "General Veterinary";
}

Veterinarian::Veterinarian(int id, String^ vetName, String^ phone, String^ spec)
{
    VeterinarianID = id;
    Name = vetName;
    Telephone = phone;
    Specialization = spec;
}

void Veterinarian::validateId(int id, String^ fieldName)
{
    if (id < 0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, fieldName + " cannot be negative.");
    }
}

int Veterinarian::VeterinarianID::get() { return veterinarianID; }
void Veterinarian::VeterinarianID::set(int value)
{
    validateId(value, "VeterinarianID");
    veterinarianID = value;
}

String^ Veterinarian::Name::get() { return name; }
void Veterinarian::Name::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Veterinarian name cannot be null or whitespace.");
    }
    name = value->Trim();
}

String^ Veterinarian::Telephone::get() { return telephone; }
void Veterinarian::Telephone::set(String^ value)
{
    if (!String::IsNullOrWhiteSpace(value) && !isValidTelephone(value))
    {
        throw gcnew ArgumentException("Invalid telephone number format.");
    }
    telephone = (value == nullptr) ? "" : value->Trim();
}

String^ Veterinarian::Specialization::get() { return specialization; }
void Veterinarian::Specialization::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Specialization cannot be null or whitespace.");
    }
    specialization = value->Trim();
}

String^ Veterinarian::displayVeterinarian()
{
    return "Veterinarian ID: " + veterinarianID.ToString() +
           "\r\nName: " + name +
           "\r\nTelephone: " + (String::IsNullOrWhiteSpace(telephone) ? "None" : telephone) +
           "\r\nSpecialization: " + specialization;
}

bool Veterinarian::isValidTelephone(String^ phone)
{
    if (String::IsNullOrWhiteSpace(phone)) return false;
    String^ trimmed = phone->Trim();
    return Regex::IsMatch(trimmed, "^\\+?[0-9]{9,15}$");
}

