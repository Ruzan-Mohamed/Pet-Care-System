#include "Owner.h"

using namespace PawCare;
using namespace System::Text::RegularExpressions;

Owner::Owner()
{
    ownerID = 0;
    name = "";
    telephone = "";
    email = "";
    address = "";
}

Owner::Owner(int id, String^ ownerName, String^ phone, String^ ownerEmail, String^ addr)
{
    OwnerID = id;
    Name = ownerName;
    Telephone = phone;
    Email = ownerEmail;
    Address = addr;
}

void Owner::validateId(int id, String^ fieldName)
{
    if (id < 0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, fieldName + " cannot be negative.");
    }
}

int Owner::OwnerID::get() { return ownerID; }
void Owner::OwnerID::set(int value)
{
    validateId(value, "OwnerID");
    ownerID = value;
}

String^ Owner::Name::get() { return name; }
void Owner::Name::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Owner name cannot be null or whitespace.");
    }
    name = value->Trim();
}

String^ Owner::Telephone::get() { return telephone; }
void Owner::Telephone::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value) || !isValidTelephone(value))
    {
        throw gcnew ArgumentException("Invalid telephone number. Expected a 10-digit number or valid international format.");
    }
    telephone = value->Trim();
}

String^ Owner::Email::get() { return email; }
void Owner::Email::set(String^ value)
{
    if (!String::IsNullOrWhiteSpace(value) && !isValidEmail(value))
    {
        throw gcnew ArgumentException("Invalid email address format.");
    }
    email = (value == nullptr) ? "" : value->Trim();
}

String^ Owner::Address::get() { return address; }
void Owner::Address::set(String^ value)
{
    address = (value == nullptr) ? "" : value->Trim();
}

String^ Owner::displayOwner()
{
    return "Owner ID: " + ownerID.ToString() +
           "\r\nName: " + name +
           "\r\nTelephone: " + telephone +
           "\r\nEmail: " + (String::IsNullOrWhiteSpace(email) ? "None" : email) +
           "\r\nAddress: " + (String::IsNullOrWhiteSpace(address) ? "None" : address);
}

bool Owner::isValidTelephone(String^ phone)
{
    if (String::IsNullOrWhiteSpace(phone)) return false;
    String^ trimmed = phone->Trim();
    return Regex::IsMatch(trimmed, "^\\+?[0-9]{9,15}$");
}

bool Owner::isValidEmail(String^ emailStr)
{
    if (String::IsNullOrWhiteSpace(emailStr)) return false;
    String^ trimmed = emailStr->Trim();
    return Regex::IsMatch(trimmed, "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
}

