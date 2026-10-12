#include "Owner.h"

using namespace PawCare;

Owner::Owner()
{
    ownerID = 0;
    name = "";
    telephone = "";
    email = "";
    address = "";
}

int Owner::OwnerID::get() { return ownerID; }
void Owner::OwnerID::set(int value) { ownerID = value; }

String^ Owner::Name::get() { return name; }
void Owner::Name::set(String^ value) { name = value; }

String^ Owner::Telephone::get() { return telephone; }
void Owner::Telephone::set(String^ value) { telephone = value; }

String^ Owner::Email::get() { return email; }
void Owner::Email::set(String^ value) { email = value; }

String^ Owner::Address::get() { return address; }
void Owner::Address::set(String^ value) { address = value; }
