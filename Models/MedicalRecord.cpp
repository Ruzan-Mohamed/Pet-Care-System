#include "MedicalRecord.h"

using namespace PawCare;
using namespace System::Globalization;

MedicalRecord::MedicalRecord()
{
    recordID = 0;
    appointmentID = 0;
    petID = 0;
    diagnosis = "";
    treatment = "";
    vaccination = "";
    notes = "";
    recordDate = "";
}

MedicalRecord::MedicalRecord(int id, int apptId, int pId, String^ diag,
                             String^ treat, String^ vacc, String^ note, String^ date)
{
    RecordID = id;
    AppointmentID = apptId;
    PetID = pId;
    Diagnosis = diag;
    Treatment = treat;
    Vaccination = vacc;
    Notes = note;
    RecordDate = date;
}

void MedicalRecord::validateId(int id, String^ fieldName)
{
    if (id < 0)
    {
        throw gcnew ArgumentOutOfRangeException(fieldName, fieldName + " cannot be negative.");
    }
}

int MedicalRecord::RecordID::get() { return recordID; }
void MedicalRecord::RecordID::set(int value)
{
    validateId(value, "RecordID");
    recordID = value;
}

int MedicalRecord::AppointmentID::get() { return appointmentID; }
void MedicalRecord::AppointmentID::set(int value)
{
    validateId(value, "AppointmentID");
    appointmentID = value;
}

int MedicalRecord::PetID::get() { return petID; }
void MedicalRecord::PetID::set(int value)
{
    validateId(value, "PetID");
    petID = value;
}

String^ MedicalRecord::Diagnosis::get() { return diagnosis; }
void MedicalRecord::Diagnosis::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Diagnosis cannot be null or whitespace.");
    }
    diagnosis = value->Trim();
}

String^ MedicalRecord::Treatment::get() { return treatment; }
void MedicalRecord::Treatment::set(String^ value)
{
    if (String::IsNullOrWhiteSpace(value))
    {
        throw gcnew ArgumentException("Treatment cannot be null or whitespace.");
    }
    treatment = value->Trim();
}

String^ MedicalRecord::Vaccination::get() { return vaccination; }
void MedicalRecord::Vaccination::set(String^ value)
{
    vaccination = (value == nullptr) ? "" : value->Trim();
}

String^ MedicalRecord::Notes::get() { return notes; }
void MedicalRecord::Notes::set(String^ value)
{
    notes = (value == nullptr) ? "" : value->Trim();
}

String^ MedicalRecord::RecordDate::get() { return recordDate; }
void MedicalRecord::RecordDate::set(String^ value)
{
    if (!String::IsNullOrWhiteSpace(value) && !isValidDateFormat(value))
    {
        throw gcnew ArgumentException("Invalid date format for Medical Record. Expected yyyy-MM-dd.");
    }
    recordDate = (value == nullptr) ? "" : value->Trim();
}

void MedicalRecord::addMedicalRecord()
{
    if (petID <= 0 || appointmentID <= 0)
    {
        throw gcnew InvalidOperationException("Medical Record requires valid Pet ID and Appointment ID.");
    }
    if (String::IsNullOrWhiteSpace(diagnosis) || String::IsNullOrWhiteSpace(treatment))
    {
        throw gcnew InvalidOperationException("Diagnosis and Treatment must be specified before adding a medical record.");
    }
    if (String::IsNullOrWhiteSpace(recordDate))
    {
        recordDate = DateTime::Now.ToString("yyyy-MM-dd");
    }
}

void MedicalRecord::updateMedicalRecord(String^ newDiagnosis,
                                        String^ newTreatment,
                                        String^ newVaccination,
                                        String^ newNotes)
{
    Diagnosis = newDiagnosis;
    Treatment = newTreatment;
    Vaccination = newVaccination;
    Notes = newNotes;
}

String^ MedicalRecord::displayMedicalRecord()
{
    return "Record ID: " + recordID.ToString() +
           "\r\nAppointment ID: " + appointmentID.ToString() +
           "\r\nPet ID: " + petID.ToString() +
           "\r\nDiagnosis: " + diagnosis +
           "\r\nTreatment: " + treatment +
           "\r\nVaccination: " + (String::IsNullOrWhiteSpace(vaccination) ? "None" : vaccination) +
           "\r\nNotes: " + (String::IsNullOrWhiteSpace(notes) ? "None" : notes) +
           "\r\nRecord Date: " + recordDate;
}

bool MedicalRecord::isValidDateFormat(String^ dateStr)
{
    if (String::IsNullOrWhiteSpace(dateStr)) return false;
    DateTime parsed;
    return DateTime::TryParseExact(dateStr->Trim(), "yyyy-MM-dd",
        CultureInfo::InvariantCulture, DateTimeStyles::None, parsed);
}

