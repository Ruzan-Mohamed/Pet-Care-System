#include "MedicalRecord.h"

using namespace PawCare;

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

int MedicalRecord::RecordID::get() { return recordID; }
void MedicalRecord::RecordID::set(int value) { recordID = value; }

int MedicalRecord::AppointmentID::get() { return appointmentID; }
void MedicalRecord::AppointmentID::set(int value) { appointmentID = value; }

int MedicalRecord::PetID::get() { return petID; }
void MedicalRecord::PetID::set(int value) { petID = value; }

String^ MedicalRecord::Diagnosis::get() { return diagnosis; }
void MedicalRecord::Diagnosis::set(String^ value) { diagnosis = value; }

String^ MedicalRecord::Treatment::get() { return treatment; }
void MedicalRecord::Treatment::set(String^ value) { treatment = value; }

String^ MedicalRecord::Vaccination::get() { return vaccination; }
void MedicalRecord::Vaccination::set(String^ value) { vaccination = value; }

String^ MedicalRecord::Notes::get() { return notes; }
void MedicalRecord::Notes::set(String^ value) { notes = value; }

String^ MedicalRecord::RecordDate::get() { return recordDate; }
void MedicalRecord::RecordDate::set(String^ value) { recordDate = value; }

void MedicalRecord::addMedicalRecord()
{
    if (recordDate == "")
        recordDate = DateTime::Now.ToString("yyyy-MM-dd");
}

void MedicalRecord::updateMedicalRecord(String^ newDiagnosis,
                                        String^ newTreatment,
                                        String^ newVaccination,
                                        String^ newNotes)
{
    diagnosis = newDiagnosis;
    treatment = newTreatment;
    vaccination = newVaccination;
    notes = newNotes;
}

String^ MedicalRecord::displayMedicalRecord()
{
    return "Diagnosis: " + diagnosis +
           "\r\nTreatment: " + treatment +
           "\r\nVaccination: " + vaccination +
           "\r\nNotes: " + notes +
           "\r\nRecord Date: " + recordDate;
}
