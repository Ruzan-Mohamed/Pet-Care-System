#include "Veterinarian.h"

using namespace PawCare;

Veterinarian::Veterinarian()
{
    veterinarianID = 0;
    name = "";
    telephone = "";
    specialization = "";
}

int Veterinarian::VeterinarianID::get() { return veterinarianID; }
void Veterinarian::VeterinarianID::set(int value) { veterinarianID = value; }

String^ Veterinarian::Name::get() { return name; }
void Veterinarian::Name::set(String^ value) { name = value; }

String^ Veterinarian::Telephone::get() { return telephone; }
void Veterinarian::Telephone::set(String^ value) { telephone = value; }

String^ Veterinarian::Specialization::get() { return specialization; }
void Veterinarian::Specialization::set(String^ value) { specialization = value; }
