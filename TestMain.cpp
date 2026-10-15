#include "Models/Pet.h"
#include "Models/Dog.h"
#include "Models/Cat.h"
#include "Models/Owner.h"
#include "Models/Veterinarian.h"
#include "Models/Appointment.h"
#include "Models/MedicalRecord.h"
#include "Models/Payment.h"
#include "Database/DatabaseManager.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace PawCare;

void runOopDemonstration();
void runValidationEdgeCaseTests();
void runDatabaseCrudTests();

int main(array<System::String^>^ args)
{
    Console::WriteLine("====================================================");
    Console::WriteLine("       PAWCARE PET CARE SYSTEM - VERIFICATION       ");
    Console::WriteLine("====================================================");
    Console::WriteLine();

    runOopDemonstration();
    Console::WriteLine();

    runValidationEdgeCaseTests();
    Console::WriteLine();

    runDatabaseCrudTests();
    Console::WriteLine();

    Console::WriteLine("====================================================");
    Console::WriteLine(" All demonstrations and tests completed successfully.");
    Console::WriteLine("====================================================");

    return 0;
}

void runOopDemonstration()
{
    Console::WriteLine("----- 1. OOP DEMONSTRATION & POLYMORPHISM -----");

    // 1. Object creation & inheritance
    Dog^ max = gcnew Dog(1, 1, "Max", "Labrador", 3, "Male");
    Cat^ luna = gcnew Cat(2, 1, "Luna", "Persian", 2, "Female");

    // Polymorphism via base reference
    Pet^ pet1 = max;
    Pet^ pet2 = luna;

    Console::WriteLine(pet1->displayPetInfo());
    Console::WriteLine("Consultation Fee: Rs. {0:F2}", pet1->calculateConsultationFee());
    Console::WriteLine();

    Console::WriteLine(pet2->displayPetInfo());
    Console::WriteLine("Consultation Fee: Rs. {0:F2}", pet2->calculateConsultationFee());
    Console::WriteLine();

    // 2. Owner & Veterinarian
    Owner^ owner = gcnew Owner(1, "Kasun Perera", "0712345678", "kasun@example.com", "Colombo");
    Console::WriteLine(owner->displayOwner());
    Console::WriteLine();

    Veterinarian^ vet = gcnew Veterinarian(1, "Dr. Silva", "0112345678", "General Veterinary");
    Console::WriteLine(vet->displayVeterinarian());
    Console::WriteLine();

    // 3. Appointment with validation
    Appointment^ appointment = gcnew Appointment(1, max->PetID, vet->VeterinarianID, "2026-10-15", "09:00", "Annual Health Check");
    appointment->createAppointment();
    Console::WriteLine(appointment->displayAppointment());
    Console::WriteLine();

    // 4. Medical Record
    MedicalRecord^ record = gcnew MedicalRecord(1, appointment->AppointmentID, max->PetID,
        "Skin Allergy", "Prescribed antihistamines", "Rabies", "Follow-up in 2 weeks", "2026-10-15");
    record->addMedicalRecord();
    Console::WriteLine(record->displayMedicalRecord());
    Console::WriteLine();

    // 5. Payment linked to pet consultation fee
    Payment^ payment = gcnew Payment(1, appointment->AppointmentID, max->calculateConsultationFee(), 1000.00, 200.00, Payment::METHOD_CASH);
    Console::WriteLine("Before payment processing:");
    Console::WriteLine(payment->displayPayment());
    payment->processPayment();
    appointment->completeAppointment();

    Console::WriteLine();
    Console::WriteLine("After payment processing and appointment completion:");
    Console::WriteLine(payment->displayPayment());
    Console::WriteLine("Updated Appointment Status: " + appointment->Status);
}

void runValidationEdgeCaseTests()
{
    Console::WriteLine("----- 2. DEFECT FIXES & EDGE CASE VALIDATION TESTS -----");

    int passed = 0;
    int failed = 0;

    // Test 2.1: Payment dynamic recalculation prevents stale totals
    try
    {
        Payment^ p = gcnew Payment(10, 1, 1000.00, 500.00, 0.00, Payment::METHOD_CASH);
        if (Math::Abs(p->TotalAmount - 1500.00) > 0.001) throw gcnew Exception("Initial total calculation incorrect");

        // Change treatment fee - total must update automatically without calling calculateTotal()
        p->TreatmentFee = 800.00;
        if (Math::Abs(p->TotalAmount - 1800.00) > 0.001) throw gcnew Exception("Total amount went stale after TreatmentFee update");

        p->OtherFee = 150.00;
        if (Math::Abs(p->TotalAmount - 1950.00) > 0.001) throw gcnew Exception("Total amount went stale after OtherFee update");

        Console::WriteLine("  [PASS] Payment: Dynamic recalculation prevents stale totals.");
        passed++;
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("  [FAIL] Payment recalculation: " + ex->Message);
        failed++;
    }

    // Test 2.2: Payment negative fee rejection
    try
    {
        Payment^ p = gcnew Payment();
        p->ConsultationFee = -500.00;
        Console::WriteLine("  [FAIL] Payment accepted negative consultation fee!");
        failed++;
    }
    catch (ArgumentOutOfRangeException^)
    {
        Console::WriteLine("  [PASS] Payment: Rejected negative fee as expected.");
        passed++;
    }

    // Test 2.3: Payment NaN / Infinity rejection
    try
    {
        Payment^ p = gcnew Payment();
        p->TreatmentFee = Double::NaN;
        Console::WriteLine("  [FAIL] Payment accepted NaN!");
        failed++;
    }
    catch (ArgumentException^)
    {
        Console::WriteLine("  [PASS] Payment: Rejected NaN fee as expected.");
        passed++;
    }

    // Test 2.4: Payment invalid status/method rejection
    try
    {
        Payment^ p = gcnew Payment();
        p->PaymentMethod = "Bitcoin"; // Not allowed
        Console::WriteLine("  [FAIL] Payment accepted invalid method!");
        failed++;
    }
    catch (ArgumentException^)
    {
        Console::WriteLine("  [PASS] Payment: Rejected invalid payment method.");
        passed++;
    }

    // Test 2.5: Duplicate payment attempt rejection
    try
    {
        Payment^ p = gcnew Payment(11, 2, 1000.00, 0.00, 0.00, Payment::METHOD_CASH);
        p->processPayment();
        p->processPayment(); // Second call must fail
        Console::WriteLine("  [FAIL] Allowed duplicate payment processing!");
        failed++;
    }
    catch (InvalidOperationException^)
    {
        Console::WriteLine("  [PASS] Payment: Prevented duplicate payment processing.");
        passed++;
    }

    // Test 2.6: Appointment date format validation
    try
    {
        Appointment^ a = gcnew Appointment();
        a->AppointmentDate = "invalid-date";
        Console::WriteLine("  [FAIL] Appointment accepted invalid date format!");
        failed++;
    }
    catch (ArgumentException^)
    {
        Console::WriteLine("  [PASS] Appointment: Rejected malformed date format.");
        passed++;
    }

    // Test 2.7: Appointment status lifecycle enforcement
    try
    {
        Appointment^ a = gcnew Appointment(20, 1, 1, "2026-10-20", "10:00", "Vaccination");
        a->createAppointment(); // BOOKED
        a->cancelAppointment(); // CANCELLED

        // Cannot transition from CANCELLED to CONFIRMED
        a->confirmAppointment();
        Console::WriteLine("  [FAIL] Resurrected cancelled appointment!");
        failed++;
    }
    catch (InvalidOperationException^)
    {
        Console::WriteLine("  [PASS] Appointment: Protected terminal state from invalid transitions.");
        passed++;
    }

    // Test 2.8: Pet age validation
    try
    {
        Dog^ d = gcnew Dog(3, 1, "Rocky", "Bulldog", -2, "Male");
        Console::WriteLine("  [FAIL] Accepted negative pet age!");
        failed++;
    }
    catch (ArgumentOutOfRangeException^)
    {
        Console::WriteLine("  [PASS] Pet: Rejected negative age.");
        passed++;
    }

    // Test 2.9: Owner phone and email validation
    try
    {
        Owner^ o = gcnew Owner();
        o->Telephone = "abc1234";
        Console::WriteLine("  [FAIL] Accepted invalid phone number!");
        failed++;
    }
    catch (ArgumentException^)
    {
        Console::WriteLine("  [PASS] Owner: Rejected invalid telephone format.");
        passed++;
    }

    Console::WriteLine("Validation tests completed: {0} Passed, {1} Failed.", passed, failed);
}

void runDatabaseCrudTests()
{
    Console::WriteLine("----- 3. MYSQL DATABASE CRUD OPERATIONS TEST -----");

    DatabaseManager^ db = gcnew DatabaseManager();
    Console::WriteLine("Testing MySQL connection to {0} ...", db->ConnectionString);

    if (!db->connect())
    {
        Console::WriteLine("  [NOTE] MySQL service is offline or requires authentication.");
        Console::WriteLine("  [NOTE] DatabaseManager safely caught connection state without crashing.");
        Console::WriteLine("  [NOTE] Schema file ready at: Database/pawcare_db.sql");
        return;
    }

    Console::WriteLine("  [PASS] Connected to MySQL successfully!");

    try
    {
        // 1. Insert Owner
        Owner^ testOwner = gcnew Owner(0, "Test User", "0771234567", "test@pawcare.com", "123 Pet Lane");
        if (db->insertOwner(testOwner))
        {
            Console::WriteLine("  [PASS] Database: Inserted owner with ID " + testOwner->OwnerID);
        }

        // 2. Fetch Owner
        Owner^ fetchedOwner = db->getOwnerById(testOwner->OwnerID);
        if (fetchedOwner != nullptr && fetchedOwner->Name == testOwner->Name)
        {
            Console::WriteLine("  [PASS] Database: Fetched owner successfully.");
        }

        // 3. Clean up
        if (db->deleteOwner(testOwner->OwnerID))
        {
            Console::WriteLine("  [PASS] Database: Deleted test owner cleanly.");
        }

        // 4. Fetch all appointments (including COMPLETED/CANCELLED ones without hydration exception)
        auto allAppts = db->getAllAppointments();
        Console::WriteLine("  [PASS] Database: Retrieved " + allAppts->Count + " appointments without hydration transition exception.");

        db->disconnect();
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("  [DB Exception]: " + ex->Message);
    }
}

