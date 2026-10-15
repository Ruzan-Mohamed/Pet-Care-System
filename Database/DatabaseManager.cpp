#include "DatabaseManager.h"

using namespace PawCare;

DatabaseManager::DatabaseManager()
{
    connectionString = buildConnectionString(DEFAULT_SERVER, DEFAULT_PORT, DEFAULT_DATABASE, DEFAULT_UID, DEFAULT_PASSWORD);
    connection = nullptr;
}

DatabaseManager::DatabaseManager(String^ connStr)
{
    connectionString = connStr;
    connection = nullptr;
}

DatabaseManager::DatabaseManager(String^ server, int port, String^ database, String^ uid, String^ password)
{
    connectionString = buildConnectionString(server, port, database, uid, password);
    connection = nullptr;
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
    this->!DatabaseManager();
}

DatabaseManager::!DatabaseManager()
{
    if (connection != nullptr)
    {
        delete connection;
        connection = nullptr;
    }
}

String^ DatabaseManager::ConnectionString::get() { return connectionString; }
void DatabaseManager::ConnectionString::set(String^ value)
{
    if (isConnected())
    {
        disconnect();
    }
    connectionString = value;
}

String^ DatabaseManager::buildConnectionString(String^ server, int port, String^ database, String^ uid, String^ password)
{
    MySqlConnectionStringBuilder^ builder = gcnew MySqlConnectionStringBuilder();
    builder->Server = server;
    builder->Port = (unsigned int)port;
    builder->Database = database;
    builder->UserID = uid;
    builder->Password = password;
    builder->SslMode = MySqlSslMode::Disabled;
    builder->CharacterSet = "utf8mb4";
    return builder->ConnectionString;
}

bool DatabaseManager::connect()
{
    try
    {
        if (connection == nullptr)
        {
            connection = gcnew MySqlConnection(connectionString);
        }

        if (connection->State != ConnectionState::Open)
        {
            connection->Open();
        }
        return true;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] Failed to connect: " + ex->Message);
        return false;
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[System Error] Connection error: " + ex->Message);
        return false;
    }
}

void DatabaseManager::disconnect()
{
    try
    {
        if (connection != nullptr && connection->State == ConnectionState::Open)
        {
            connection->Close();
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Warning] Disconnect warning: " + ex->Message);
    }
}

bool DatabaseManager::isConnected()
{
    return (connection != nullptr && connection->State == ConnectionState::Open);
}

MySqlConnection^ DatabaseManager::getConnection()
{
    return connection;
}

MySqlTransaction^ DatabaseManager::beginTransaction()
{
    if (!isConnected() && !connect()) return nullptr;
    return connection->BeginTransaction();
}

// =====================================================================
// OWNER CRUD
// =====================================================================

bool DatabaseManager::insertOwner(Owner^ owner)
{
    if (owner == nullptr) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "INSERT INTO owners (name, telephone, email, address) VALUES (@name, @phone, @email, @addr);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@name", owner->Name);
    cmd->Parameters->AddWithValue("@phone", owner->Telephone);
    cmd->Parameters->AddWithValue("@email", (owner->Email == nullptr) ? "" : owner->Email);
    cmd->Parameters->AddWithValue("@addr", (owner->Address == nullptr) ? "" : owner->Address);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            owner->OwnerID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertOwner failed: " + ex->Message);
        return false;
    }
}

Owner^ DatabaseManager::getOwnerById(int ownerId)
{
    if (ownerId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT owner_id, name, telephone, email, address FROM owners WHERE owner_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", ownerId);

    MySqlDataReader^ reader = nullptr;
    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Owner^ owner = gcnew Owner();
            owner->OwnerID = reader->GetInt32("owner_id");
            owner->Name = reader->GetString("name");
            owner->Telephone = reader->GetString("telephone");
            owner->Email = reader->IsDBNull(reader->GetOrdinal("email")) ? "" : reader->GetString("email");
            owner->Address = reader->IsDBNull(reader->GetOrdinal("address")) ? "" : reader->GetString("address");
            reader->Close();
            return owner;
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getOwnerById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

List<Owner^>^ DatabaseManager::getAllOwners()
{
    List<Owner^>^ list = gcnew List<Owner^>();
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT owner_id, name, telephone, email, address FROM owners ORDER BY owner_id ASC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Owner^ o = gcnew Owner();
            o->OwnerID = reader->GetInt32("owner_id");
            o->Name = reader->GetString("name");
            o->Telephone = reader->GetString("telephone");
            o->Email = reader->IsDBNull(reader->GetOrdinal("email")) ? "" : reader->GetString("email");
            o->Address = reader->IsDBNull(reader->GetOrdinal("address")) ? "" : reader->GetString("address");
            list->Add(o);
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getAllOwners failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

bool DatabaseManager::updateOwner(Owner^ owner)
{
    if (owner == nullptr || owner->OwnerID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE owners SET name = @name, telephone = @phone, email = @email, address = @addr WHERE owner_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@name", owner->Name);
    cmd->Parameters->AddWithValue("@phone", owner->Telephone);
    cmd->Parameters->AddWithValue("@email", (owner->Email == nullptr) ? "" : owner->Email);
    cmd->Parameters->AddWithValue("@addr", (owner->Address == nullptr) ? "" : owner->Address);
    cmd->Parameters->AddWithValue("@id", owner->OwnerID);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updateOwner failed: " + ex->Message);
        return false;
    }
}

bool DatabaseManager::deleteOwner(int ownerId)
{
    if (ownerId <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "DELETE FROM owners WHERE owner_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", ownerId);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] deleteOwner failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// VETERINARIAN CRUD
// =====================================================================

bool DatabaseManager::insertVeterinarian(Veterinarian^ vet)
{
    if (vet == nullptr) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "INSERT INTO veterinarians (name, telephone, specialization) VALUES (@name, @phone, @spec);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@name", vet->Name);
    cmd->Parameters->AddWithValue("@phone", (vet->Telephone == nullptr) ? "" : vet->Telephone);
    cmd->Parameters->AddWithValue("@spec", vet->Specialization);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            vet->VeterinarianID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertVeterinarian failed: " + ex->Message);
        return false;
    }
}

Veterinarian^ DatabaseManager::getVeterinarianById(int vetId)
{
    if (vetId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT veterinarian_id, name, telephone, specialization FROM veterinarians WHERE veterinarian_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", vetId);

    MySqlDataReader^ reader = nullptr;
    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Veterinarian^ v = gcnew Veterinarian();
            v->VeterinarianID = reader->GetInt32("veterinarian_id");
            v->Name = reader->GetString("name");
            v->Telephone = reader->IsDBNull(reader->GetOrdinal("telephone")) ? "" : reader->GetString("telephone");
            v->Specialization = reader->GetString("specialization");
            reader->Close();
            return v;
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getVeterinarianById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

List<Veterinarian^>^ DatabaseManager::getAllVeterinarians()
{
    List<Veterinarian^>^ list = gcnew List<Veterinarian^>();
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT veterinarian_id, name, telephone, specialization FROM veterinarians ORDER BY veterinarian_id ASC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Veterinarian^ v = gcnew Veterinarian();
            v->VeterinarianID = reader->GetInt32("veterinarian_id");
            v->Name = reader->GetString("name");
            v->Telephone = reader->IsDBNull(reader->GetOrdinal("telephone")) ? "" : reader->GetString("telephone");
            v->Specialization = reader->GetString("specialization");
            list->Add(v);
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getAllVeterinarians failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

bool DatabaseManager::updateVeterinarian(Veterinarian^ vet)
{
    if (vet == nullptr || vet->VeterinarianID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE veterinarians SET name = @name, telephone = @phone, specialization = @spec WHERE veterinarian_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@name", vet->Name);
    cmd->Parameters->AddWithValue("@phone", (vet->Telephone == nullptr) ? "" : vet->Telephone);
    cmd->Parameters->AddWithValue("@spec", vet->Specialization);
    cmd->Parameters->AddWithValue("@id", vet->VeterinarianID);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updateVeterinarian failed: " + ex->Message);
        return false;
    }
}

bool DatabaseManager::deleteVeterinarian(int vetId)
{
    if (vetId <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "DELETE FROM veterinarians WHERE veterinarian_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", vetId);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] deleteVeterinarian failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// PET CRUD
// =====================================================================

bool DatabaseManager::insertPet(Pet^ pet)
{
    if (pet == nullptr || pet->OwnerID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "INSERT INTO pets (owner_id, name, pet_type, breed, age, gender) VALUES (@ownerId, @name, @type, @breed, @age, @gender);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@ownerId", pet->OwnerID);
    cmd->Parameters->AddWithValue("@name", pet->Name);
    cmd->Parameters->AddWithValue("@type", pet->PetType);
    cmd->Parameters->AddWithValue("@breed", (pet->Breed == nullptr) ? "" : pet->Breed);
    cmd->Parameters->AddWithValue("@age", pet->Age);
    cmd->Parameters->AddWithValue("@gender", pet->Gender);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            pet->PetID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertPet failed: " + ex->Message);
        return false;
    }
}

static Pet^ createPetInstance(int id, int ownerId, String^ name, String^ type, String^ breed, int age, String^ gender)
{
    if (String::Equals(type, "Dog", StringComparison::OrdinalIgnoreCase))
    {
        return gcnew Dog(id, ownerId, name, breed, age, gender);
    }
    else if (String::Equals(type, "Cat", StringComparison::OrdinalIgnoreCase))
    {
        return gcnew Cat(id, ownerId, name, breed, age, gender);
    }
    else
    {
        return gcnew Pet(id, ownerId, name, type, breed, age, gender);
    }
}

Pet^ DatabaseManager::getPetById(int petId)
{
    if (petId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT pet_id, owner_id, name, pet_type, breed, age, gender FROM pets WHERE pet_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", petId);

    MySqlDataReader^ reader = nullptr;
    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Pet^ pet = createPetInstance(
                reader->GetInt32("pet_id"),
                reader->GetInt32("owner_id"),
                reader->GetString("name"),
                reader->GetString("pet_type"),
                reader->IsDBNull(reader->GetOrdinal("breed")) ? "" : reader->GetString("breed"),
                reader->GetInt32("age"),
                reader->GetString("gender")
            );
            reader->Close();
            return pet;
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getPetById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

List<Pet^>^ DatabaseManager::getPetsByOwnerId(int ownerId)
{
    List<Pet^>^ list = gcnew List<Pet^>();
    if (ownerId <= 0) return list;
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT pet_id, owner_id, name, pet_type, breed, age, gender FROM pets WHERE owner_id = @ownerId ORDER BY pet_id ASC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@ownerId", ownerId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Pet^ pet = createPetInstance(
                reader->GetInt32("pet_id"),
                reader->GetInt32("owner_id"),
                reader->GetString("name"),
                reader->GetString("pet_type"),
                reader->IsDBNull(reader->GetOrdinal("breed")) ? "" : reader->GetString("breed"),
                reader->GetInt32("age"),
                reader->GetString("gender")
            );
            list->Add(pet);
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getPetsByOwnerId failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

List<Pet^>^ DatabaseManager::getAllPets()
{
    List<Pet^>^ list = gcnew List<Pet^>();
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT pet_id, owner_id, name, pet_type, breed, age, gender FROM pets ORDER BY pet_id ASC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Pet^ pet = createPetInstance(
                reader->GetInt32("pet_id"),
                reader->GetInt32("owner_id"),
                reader->GetString("name"),
                reader->GetString("pet_type"),
                reader->IsDBNull(reader->GetOrdinal("breed")) ? "" : reader->GetString("breed"),
                reader->GetInt32("age"),
                reader->GetString("gender")
            );
            list->Add(pet);
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getAllPets failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

bool DatabaseManager::updatePet(Pet^ pet)
{
    if (pet == nullptr || pet->PetID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE pets SET owner_id = @ownerId, name = @name, pet_type = @type, breed = @breed, age = @age, gender = @gender WHERE pet_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@ownerId", pet->OwnerID);
    cmd->Parameters->AddWithValue("@name", pet->Name);
    cmd->Parameters->AddWithValue("@type", pet->PetType);
    cmd->Parameters->AddWithValue("@breed", (pet->Breed == nullptr) ? "" : pet->Breed);
    cmd->Parameters->AddWithValue("@age", pet->Age);
    cmd->Parameters->AddWithValue("@gender", pet->Gender);
    cmd->Parameters->AddWithValue("@id", pet->PetID);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updatePet failed: " + ex->Message);
        return false;
    }
}

bool DatabaseManager::deletePet(int petId)
{
    if (petId <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "DELETE FROM pets WHERE pet_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", petId);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] deletePet failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// APPOINTMENT CRUD
// =====================================================================

bool DatabaseManager::isVeterinarianAvailable(int vetId, String^ date, String^ time, int excludeApptId)
{
    if (vetId <= 0 || String::IsNullOrWhiteSpace(date) || String::IsNullOrWhiteSpace(time)) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "SELECT COUNT(*) FROM appointments WHERE veterinarian_id = @vetId AND appointment_date = @date AND appointment_time = @time AND status != 'CANCELLED'";
    if (excludeApptId > 0)
    {
        query += " AND appointment_id != @excludeId";
    }

    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@vetId", vetId);
    cmd->Parameters->AddWithValue("@date", date);
    cmd->Parameters->AddWithValue("@time", time);
    if (excludeApptId > 0)
    {
        cmd->Parameters->AddWithValue("@excludeId", excludeApptId);
    }

    try
    {
        long long count = Convert::ToInt64(cmd->ExecuteScalar());
        return (count == 0);
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] isVeterinarianAvailable failed: " + ex->Message);
        return false;
    }
}

bool DatabaseManager::insertAppointment(Appointment^ appt)
{
    if (appt == nullptr || appt->PetID <= 0 || appt->VeterinarianID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    // Check vet availability first
    if (!isVeterinarianAvailable(appt->VeterinarianID, appt->AppointmentDate, appt->AppointmentTime, 0))
    {
        Console::WriteLine("[Booking Conflict] Veterinarian already has an active appointment at this date and time.");
        return false;
    }

    String^ query = "INSERT INTO appointments (pet_id, veterinarian_id, appointment_date, appointment_time, reason, status) "
                    "VALUES (@petId, @vetId, @date, @time, @reason, @status);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@petId", appt->PetID);
    cmd->Parameters->AddWithValue("@vetId", appt->VeterinarianID);
    cmd->Parameters->AddWithValue("@date", appt->AppointmentDate);
    cmd->Parameters->AddWithValue("@time", appt->AppointmentTime);
    cmd->Parameters->AddWithValue("@reason", appt->Reason);
    cmd->Parameters->AddWithValue("@status", appt->Status);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            appt->AppointmentID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertAppointment failed: " + ex->Message);
        return false;
    }
}

Appointment^ DatabaseManager::getAppointmentById(int apptId)
{
    if (apptId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT appointment_id, pet_id, veterinarian_id, appointment_date, appointment_time, reason, status "
                    "FROM appointments WHERE appointment_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", apptId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Appointment^ a = gcnew Appointment();
            a->AppointmentID = reader->GetInt32("appointment_id");
            a->PetID = reader->GetInt32("pet_id");
            a->VeterinarianID = reader->GetInt32("veterinarian_id");
            a->AppointmentDate = reader->GetDateTime("appointment_date").ToString("yyyy-MM-dd");
            a->AppointmentTime = reader->GetTimeSpan(reader->GetOrdinal("appointment_time")).ToString("hh\\:mm");
            a->Reason = reader->GetString("reason");
            a->setPersistedStatus(reader->IsDBNull(reader->GetOrdinal("status")) ? Appointment::STATUS_BOOKED : reader->GetString("status"));
            reader->Close();
            return a;
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getAppointmentById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

List<Appointment^>^ DatabaseManager::getAllAppointments()
{
    List<Appointment^>^ list = gcnew List<Appointment^>();
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT appointment_id, pet_id, veterinarian_id, appointment_date, appointment_time, reason, status FROM appointments ORDER BY appointment_date DESC, appointment_time DESC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Appointment^ a = gcnew Appointment();
            a->AppointmentID = reader->GetInt32("appointment_id");
            a->PetID = reader->GetInt32("pet_id");
            a->VeterinarianID = reader->GetInt32("veterinarian_id");
            a->AppointmentDate = reader->GetDateTime("appointment_date").ToString("yyyy-MM-dd");
            a->AppointmentTime = reader->GetTimeSpan(reader->GetOrdinal("appointment_time")).ToString("hh\\:mm");
            a->Reason = reader->GetString("reason");
            a->setPersistedStatus(reader->IsDBNull(reader->GetOrdinal("status")) ? Appointment::STATUS_BOOKED : reader->GetString("status"));
            list->Add(a);
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getAllAppointments failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

List<Appointment^>^ DatabaseManager::getAppointmentsByPetId(int petId)
{
    List<Appointment^>^ list = gcnew List<Appointment^>();
    if (petId <= 0) return list;
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT appointment_id, pet_id, veterinarian_id, appointment_date, appointment_time, reason, status FROM appointments WHERE pet_id = @petId ORDER BY appointment_date DESC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@petId", petId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            Appointment^ a = gcnew Appointment();
            a->AppointmentID = reader->GetInt32("appointment_id");
            a->PetID = reader->GetInt32("pet_id");
            a->VeterinarianID = reader->GetInt32("veterinarian_id");
            a->AppointmentDate = reader->GetDateTime("appointment_date").ToString("yyyy-MM-dd");
            a->AppointmentTime = reader->GetTimeSpan(reader->GetOrdinal("appointment_time")).ToString("hh\\:mm");
            a->Reason = reader->GetString("reason");
            a->setPersistedStatus(reader->IsDBNull(reader->GetOrdinal("status")) ? Appointment::STATUS_BOOKED : reader->GetString("status"));
            list->Add(a);
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getAppointmentsByPetId failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

bool DatabaseManager::updateAppointment(Appointment^ appt)
{
    if (appt == nullptr || appt->AppointmentID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    if (!isVeterinarianAvailable(appt->VeterinarianID, appt->AppointmentDate, appt->AppointmentTime, appt->AppointmentID))
    {
        Console::WriteLine("[Booking Conflict] Update failed: Veterinarian has another active booking at that time.");
        return false;
    }

    String^ query = "UPDATE appointments SET appointment_date = @date, appointment_time = @time, reason = @reason, status = @status WHERE appointment_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@date", appt->AppointmentDate);
    cmd->Parameters->AddWithValue("@time", appt->AppointmentTime);
    cmd->Parameters->AddWithValue("@reason", appt->Reason);
    cmd->Parameters->AddWithValue("@status", appt->Status);
    cmd->Parameters->AddWithValue("@id", appt->AppointmentID);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updateAppointment failed: " + ex->Message);
        return false;
    }
}

bool DatabaseManager::changeAppointmentStatus(int apptId, String^ newStatus)
{
    if (apptId <= 0 || String::IsNullOrWhiteSpace(newStatus)) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE appointments SET status = @status WHERE appointment_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@status", newStatus);
    cmd->Parameters->AddWithValue("@id", apptId);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] changeAppointmentStatus failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// MEDICAL RECORD CRUD
// =====================================================================

bool DatabaseManager::insertMedicalRecord(MedicalRecord^ record)
{
    if (record == nullptr || record->PetID <= 0 || record->AppointmentID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "INSERT INTO medical_records (appointment_id, pet_id, diagnosis, treatment, vaccination, notes, record_date) "
                    "VALUES (@apptId, @petId, @diag, @treat, @vacc, @notes, @rdate);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@apptId", record->AppointmentID);
    cmd->Parameters->AddWithValue("@petId", record->PetID);
    cmd->Parameters->AddWithValue("@diag", record->Diagnosis);
    cmd->Parameters->AddWithValue("@treat", record->Treatment);
    cmd->Parameters->AddWithValue("@vacc", (record->Vaccination == nullptr) ? "" : record->Vaccination);
    cmd->Parameters->AddWithValue("@notes", (record->Notes == nullptr) ? "" : record->Notes);
    cmd->Parameters->AddWithValue("@rdate", String::IsNullOrWhiteSpace(record->RecordDate) ? DateTime::Now.ToString("yyyy-MM-dd") : record->RecordDate);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            record->RecordID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertMedicalRecord failed: " + ex->Message);
        return false;
    }
}

MedicalRecord^ DatabaseManager::getMedicalRecordById(int recordId)
{
    if (recordId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT record_id, appointment_id, pet_id, diagnosis, treatment, vaccination, notes, record_date "
                    "FROM medical_records WHERE record_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", recordId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            MedicalRecord^ r = gcnew MedicalRecord();
            r->RecordID = reader->GetInt32("record_id");
            r->AppointmentID = reader->GetInt32("appointment_id");
            r->PetID = reader->GetInt32("pet_id");
            r->Diagnosis = reader->GetString("diagnosis");
            r->Treatment = reader->GetString("treatment");
            r->Vaccination = reader->IsDBNull(reader->GetOrdinal("vaccination")) ? "" : reader->GetString("vaccination");
            r->Notes = reader->IsDBNull(reader->GetOrdinal("notes")) ? "" : reader->GetString("notes");
            r->RecordDate = reader->GetDateTime("record_date").ToString("yyyy-MM-dd");
            reader->Close();
            return r;
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getMedicalRecordById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

List<MedicalRecord^>^ DatabaseManager::getMedicalRecordsByPetId(int petId)
{
    List<MedicalRecord^>^ list = gcnew List<MedicalRecord^>();
    if (petId <= 0) return list;
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT record_id, appointment_id, pet_id, diagnosis, treatment, vaccination, notes, record_date "
                    "FROM medical_records WHERE pet_id = @petId ORDER BY record_date DESC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@petId", petId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            MedicalRecord^ r = gcnew MedicalRecord();
            r->RecordID = reader->GetInt32("record_id");
            r->AppointmentID = reader->GetInt32("appointment_id");
            r->PetID = reader->GetInt32("pet_id");
            r->Diagnosis = reader->GetString("diagnosis");
            r->Treatment = reader->GetString("treatment");
            r->Vaccination = reader->IsDBNull(reader->GetOrdinal("vaccination")) ? "" : reader->GetString("vaccination");
            r->Notes = reader->IsDBNull(reader->GetOrdinal("notes")) ? "" : reader->GetString("notes");
            r->RecordDate = reader->GetDateTime("record_date").ToString("yyyy-MM-dd");
            list->Add(r);
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getMedicalRecordsByPetId failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

List<MedicalRecord^>^ DatabaseManager::getAllMedicalRecords()
{
    List<MedicalRecord^>^ list = gcnew List<MedicalRecord^>();
    if (!isConnected() && !connect()) return list;

    String^ query = "SELECT record_id, appointment_id, pet_id, diagnosis, treatment, vaccination, notes, record_date "
                    "FROM medical_records ORDER BY record_date DESC, record_id DESC;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        while (reader->Read())
        {
            MedicalRecord^ r = gcnew MedicalRecord();
            r->RecordID = reader->GetInt32("record_id");
            r->AppointmentID = reader->GetInt32("appointment_id");
            r->PetID = reader->GetInt32("pet_id");
            r->Diagnosis = reader->GetString("diagnosis");
            r->Treatment = reader->GetString("treatment");
            r->Vaccination = reader->IsDBNull(reader->GetOrdinal("vaccination")) ? "" : reader->GetString("vaccination");
            r->Notes = reader->IsDBNull(reader->GetOrdinal("notes")) ? "" : reader->GetString("notes");
            r->RecordDate = reader->GetDateTime("record_date").ToString("yyyy-MM-dd");
            list->Add(r);
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[DB Error] getAllMedicalRecords failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return list;
}

bool DatabaseManager::updateMedicalRecord(MedicalRecord^ record)
{
    if (record == nullptr || record->RecordID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE medical_records SET diagnosis = @diag, treatment = @treat, vaccination = @vacc, notes = @notes WHERE record_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@diag", record->Diagnosis);
    cmd->Parameters->AddWithValue("@treat", record->Treatment);
    cmd->Parameters->AddWithValue("@vacc", (record->Vaccination == nullptr) ? "" : record->Vaccination);
    cmd->Parameters->AddWithValue("@notes", (record->Notes == nullptr) ? "" : record->Notes);
    cmd->Parameters->AddWithValue("@id", record->RecordID);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updateMedicalRecord failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// PAYMENT CRUD
// =====================================================================

bool DatabaseManager::insertPayment(Payment^ payment)
{
    if (payment == nullptr || payment->AppointmentID <= 0) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "INSERT INTO payments (appointment_id, consultation_fee, treatment_fee, other_fee, total_amount, payment_method, payment_status) "
                    "VALUES (@apptId, @cFee, @tFee, @oFee, @total, @method, @status);";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@apptId", payment->AppointmentID);
    cmd->Parameters->AddWithValue("@cFee", payment->ConsultationFee);
    cmd->Parameters->AddWithValue("@tFee", payment->TreatmentFee);
    cmd->Parameters->AddWithValue("@oFee", payment->OtherFee);
    cmd->Parameters->AddWithValue("@total", payment->TotalAmount);
    cmd->Parameters->AddWithValue("@method", payment->PaymentMethod);
    cmd->Parameters->AddWithValue("@status", payment->PaymentStatus);

    try
    {
        int rows = cmd->ExecuteNonQuery();
        if (rows > 0)
        {
            payment->PaymentID = (int)cmd->LastInsertedId;
            return true;
        }
        return false;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] insertPayment failed: " + ex->Message);
        return false;
    }
}

Payment^ DatabaseManager::getPaymentById(int paymentId)
{
    if (paymentId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT payment_id, appointment_id, consultation_fee, treatment_fee, other_fee, total_amount, payment_method, payment_status "
                    "FROM payments WHERE payment_id = @id LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@id", paymentId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Payment^ p = gcnew Payment(
                reader->GetInt32("payment_id"),
                reader->GetInt32("appointment_id"),
                Convert::ToDouble(reader->GetDecimal("consultation_fee")),
                Convert::ToDouble(reader->GetDecimal("treatment_fee")),
                Convert::ToDouble(reader->GetDecimal("other_fee")),
                reader->GetString("payment_method")
            );
            p->PaymentStatus = reader->GetString("payment_status");
            reader->Close();
            return p;
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getPaymentById failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

Payment^ DatabaseManager::getPaymentByAppointmentId(int apptId)
{
    if (apptId <= 0) return nullptr;
    if (!isConnected() && !connect()) return nullptr;

    String^ query = "SELECT payment_id, appointment_id, consultation_fee, treatment_fee, other_fee, total_amount, payment_method, payment_status "
                    "FROM payments WHERE appointment_id = @apptId LIMIT 1;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@apptId", apptId);
    MySqlDataReader^ reader = nullptr;

    try
    {
        reader = cmd->ExecuteReader();
        if (reader->Read())
        {
            Payment^ p = gcnew Payment(
                reader->GetInt32("payment_id"),
                reader->GetInt32("appointment_id"),
                Convert::ToDouble(reader->GetDecimal("consultation_fee")),
                Convert::ToDouble(reader->GetDecimal("treatment_fee")),
                Convert::ToDouble(reader->GetDecimal("other_fee")),
                reader->GetString("payment_method")
            );
            p->PaymentStatus = reader->GetString("payment_status");
            reader->Close();
            return p;
        }
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] getPaymentByAppointmentId failed: " + ex->Message);
    }
    finally
    {
        if (reader != nullptr && !reader->IsClosed) reader->Close();
    }
    return nullptr;
}

bool DatabaseManager::updatePaymentStatus(int paymentId, String^ newStatus)
{
    if (paymentId <= 0 || String::IsNullOrWhiteSpace(newStatus)) return false;
    if (!isConnected() && !connect()) return false;

    String^ query = "UPDATE payments SET payment_status = @status WHERE payment_id = @id;";
    MySqlCommand^ cmd = gcnew MySqlCommand(query, connection);
    cmd->Parameters->AddWithValue("@status", newStatus);
    cmd->Parameters->AddWithValue("@id", paymentId);

    try
    {
        return cmd->ExecuteNonQuery() > 0;
    }
    catch (MySqlException^ ex)
    {
        Console::WriteLine("[DB Error] updatePaymentStatus failed: " + ex->Message);
        return false;
    }
}

// =====================================================================
// TRANSACTIONAL OPERATION: PROCESS PAYMENT & COMPLETE APPOINTMENT
// =====================================================================

bool DatabaseManager::processAppointmentPayment(int apptId, Payment^ payment)
{
    if (apptId <= 0 || payment == nullptr) return false;
    if (!isConnected() && !connect()) return false;

    payment->AppointmentID = apptId;
    if (!payment->processPayment()) return false;

    MySqlTransaction^ trans = connection->BeginTransaction();
    try
    {
        // 1. Insert payment record
        String^ payQuery = "INSERT INTO payments (appointment_id, consultation_fee, treatment_fee, other_fee, total_amount, payment_method, payment_status) "
                           "VALUES (@apptId, @cFee, @tFee, @oFee, @total, @method, @status);";
        MySqlCommand^ payCmd = gcnew MySqlCommand(payQuery, connection, trans);
        payCmd->Parameters->AddWithValue("@apptId", payment->AppointmentID);
        payCmd->Parameters->AddWithValue("@cFee", payment->ConsultationFee);
        payCmd->Parameters->AddWithValue("@tFee", payment->TreatmentFee);
        payCmd->Parameters->AddWithValue("@oFee", payment->OtherFee);
        payCmd->Parameters->AddWithValue("@total", payment->TotalAmount);
        payCmd->Parameters->AddWithValue("@method", payment->PaymentMethod);
        payCmd->Parameters->AddWithValue("@status", payment->PaymentStatus);
        payCmd->ExecuteNonQuery();
        payment->PaymentID = (int)payCmd->LastInsertedId;

        // 2. Update appointment status to COMPLETED
        String^ apptQuery = "UPDATE appointments SET status = 'COMPLETED' WHERE appointment_id = @id;";
        MySqlCommand^ apptCmd = gcnew MySqlCommand(apptQuery, connection, trans);
        apptCmd->Parameters->AddWithValue("@id", apptId);
        apptCmd->ExecuteNonQuery();

        trans->Commit();
        return true;
    }
    catch (Exception^ ex)
    {
        trans->Rollback();
        payment->markFailed();
        Console::WriteLine("[Transaction Error] processAppointmentPayment rolled back: " + ex->Message);
        return false;
    }
}

