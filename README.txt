PAWCARE - PET CARE SYSTEM (C++/CLI + MySQL)
==============================================

PawCare is an Object-Oriented Pet Care and Clinic Management System implemented in
Visual Studio C++/CLI (.NET Framework / CLR) with MySQL / MariaDB database integration.

FEATURES & OOP PRINCIPLES
-------------------------
1. Classes & Objects:
   - Pet, Dog, Cat, Owner, Veterinarian, Appointment, MedicalRecord, Payment.
2. Encapsulation & Validation:
   - Robust property getters/setters.
   - Guard against negative, NaN, infinity fees, and empty required strings.
   - Stale total protection: Payment dynamically recalculates totals.
   - Lifecycle enforcement on Appointments (BOOKED -> CONFIRMED -> COMPLETED / CANCELLED).
   - Validations for phone numbers (Regex) and email formats.
3. Inheritance & Polymorphism:
   - Dog and Cat inherit from base Pet.
   - Overridden calculateConsultationFee() and displayPetInfo().
   - Polymorphic method invocation through base Pet^ handles.
4. Database Integration & Persistence:
   - Full MySQL ADO.NET integration via MySql.Data connector.
   - Parameterized SQL queries to prevent SQL injection.
   - Complete CRUD operations for all entities.
   - Transactional payment processing and appointment completion.
   - Veterinarian double-booking prevention.

PROJECT STRUCTURE
-----------------
PawCare.sln                 - Visual Studio Solution File
PawCare.vcxproj             - Visual Studio Project File (CLR .NET Framework 4.8)
PawCare.vcxproj.filters     - Visual Studio Filters File
TestMain.cpp                - Entry point: Demonstration, Validation Tests, and DB CRUD
Models/
  Pet.h / Pet.cpp           - Base Pet class
  Dog.h / Dog.cpp           - Dog subclass with specialized fee
  Cat.h / Cat.cpp           - Cat subclass with specialized fee
  Owner.h / Owner.cpp       - Pet owner entity with contact validation
  Veterinarian.h / .cpp     - Veterinarian entity
  Appointment.h / .cpp      - Appointment lifecycle entity
  MedicalRecord.h / .cpp    - Medical history and diagnosis
  Payment.h / .cpp          - Monetary calculations and payment state
Database/
  DatabaseManager.h / .cpp  - ADO.NET MySQL database manager with full CRUD
  pawcare_db.sql            - MySQL schema, tables, foreign keys, and seed data
packages/
  MySql.Data/MySql.Data.dll - MySQL Connector/NET ADO.NET library

HOW TO RUN IN VISUAL STUDIO
---------------------------
1. Open Visual Studio 2022 / 2026.
2. File -> Open -> Project/Solution -> Select "PawCare.sln".
3. Ensure the project configuration is set to:
   - Configuration: Debug or Release
   - Platform: x64
4. Ensure the C++/CLI workload and .NET Framework 4.8 targeting pack are installed in VS:
   - In VS Installer: "Desktop development with C++" -> check "C++/CLI support for v143 build tools".
5. Build Solution (Ctrl + Shift + B).
6. Press F5 or Ctrl + F5 to run.

MYSQL DATABASE SETUP
--------------------
1. Start your MySQL or MariaDB server (e.g. via XAMPP, MySQL Workbench, or Windows Service).
2. Open a MySQL terminal or administration tool:
   mysql -u root -p < Database/pawcare_db.sql
3. Verify that the database "pawcare_db" is created with tables:
   - owners
   - veterinarians
   - pets
   - appointments
   - medical_records
   - payments
4. If your MySQL root user has a password or runs on a non-default port,
   configure the connection parameters when instantiating DatabaseManager:
   DatabaseManager^ db = gcnew DatabaseManager("localhost", 3306, "pawcare_db", "root", "your_password");

