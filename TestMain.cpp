#include "Models/Pet.h"
#include "Models/Dog.h"
#include "Models/Cat.h"
#include "Models/Owner.h"
#include "Models/Veterinarian.h"
#include "Models/Appointment.h"
#include "Models/MedicalRecord.h"
#include "Models/Payment.h"

using namespace System;
using namespace PawCare;

int main(array<System::String^>^ args)
{
    // Object creation
    Dog^ max = gcnew Dog(1, 1, "Max", "Labrador", 3, "Male");
    Cat^ luna = gcnew Cat(2, 1, "Luna", "Persian", 2, "Female");

    // Inheritance + polymorphism
    Pet^ pet1 = max;
    Pet^ pet2 = luna;

    Console::WriteLine("===== PAWCARE OOP TEST =====");
    Console::WriteLine(pet1->displayPetInfo());
    Console::WriteLine("Consultation Fee: Rs. {0:F2}", pet1->calculateConsultationFee());
    Console::WriteLine();

    Console::WriteLine(pet2->displayPetInfo());
    Console::WriteLine("Consultation Fee: Rs. {0:F2}", pet2->calculateConsultationFee());
    Console::WriteLine();

    Owner^ owner = gcnew Owner();
    owner->OwnerID = 1;
    owner->Name = "Kasun Perera";
    owner->Telephone = "0712345678";
    owner->Email = "kasun@example.com";

    Veterinarian^ vet = gcnew Veterinarian();
    vet->VeterinarianID = 1;
    vet->Name = "Dr. Silva";
    vet->Specialization = "General Veterinary";

    Appointment^ appointment = gcnew Appointment();
    appointment->AppointmentID = 1;
    appointment->PetID = max->PetID;
    appointment->VeterinarianID = vet->VeterinarianID;
    appointment->AppointmentDate = "2026-10-07";
    appointment->AppointmentTime = "09:00";
    appointment->Reason = "General Checkup";
    appointment->createAppointment();

    Console::WriteLine("===== APPOINTMENT =====");
    Console::WriteLine(appointment->displayAppointment());
    Console::WriteLine();

    MedicalRecord^ record = gcnew MedicalRecord();
    record->RecordID = 1;
    record->AppointmentID = appointment->AppointmentID;
    record->PetID = max->PetID;
    record->Diagnosis = "Skin Allergy";
    record->Treatment = "Prescribed medication";
    record->Vaccination = "Rabies";
    record->Notes = "Follow-up in 2 weeks";
    record->addMedicalRecord();

    Console::WriteLine("===== MEDICAL RECORD =====");
    Console::WriteLine(record->displayMedicalRecord());
    Console::WriteLine();

    Payment^ payment = gcnew Payment();
    payment->PaymentID = 1;
    payment->AppointmentID = appointment->AppointmentID;
    payment->ConsultationFee = 1500.00;
    payment->TreatmentFee = 1000.00;
    payment->OtherFee = 0.00;
    payment->PaymentMethod = "Cash";
    payment->processPayment();

    Console::WriteLine("===== PAYMENT =====");
    Console::WriteLine(payment->displayPayment());
    Console::WriteLine();

    Console::WriteLine("OOP Part 4 test completed successfully.");
    Console::ReadKey();

    return 0;
}
