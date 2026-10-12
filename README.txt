PAWCARE - OOP PART 4 FILES
===========================

This package contains the C++/CLI OOP model classes for the PawCare project.

IMPORTANT:
- This is Part 4 only.
- MySQL connection is NOT implemented yet. That is planned for Part 6.
- The code is intended for Visual Studio C++/CLI in a CLR Empty Project (.NET Framework).

FILES
-----
Models:
  Pet.h / Pet.cpp
  Dog.h / Dog.cpp
  Cat.h / Cat.cpp
  Owner.h / Owner.cpp
  Veterinarian.h / Veterinarian.cpp
  Appointment.h / Appointment.cpp
  MedicalRecord.h / MedicalRecord.cpp
  Payment.h / Payment.cpp

Database:
  DatabaseManager.h / DatabaseManager.cpp

Test:
  TestMain.cpp

HOW TO USE
----------
1. Open Visual Studio.
2. Create a CLR Empty Project (.NET Framework), for example: PawCare.
3. Add the .h and .cpp files to the project using Solution Explorer:
   Right-click Project -> Add -> Existing Item.
4. Keep the folders in Solution Explorer if you want, but the #include paths in
   TestMain.cpp assume the physical folders Models and Database.
5. If you add the files without folders, change the include paths in TestMain.cpp
   from "Models/Pet.h" to "Pet.h", etc.
6. Set TestMain.cpp as the source file containing main().
7. Project Properties:
   Linker -> Advanced -> Entry Point = main
   Linker -> System -> SubSystem = Windows
8. Build -> Build Solution.
9. Run.

EXPECTED TEST
-------------
The console test creates Max (Dog) and Luna (Cat), demonstrates inheritance and
polymorphism, creates an owner, veterinarian, appointment, medical record and
payment, and prints their information.

OOP CONCEPTS SHOWN
------------------
Class       -> Pet, Dog, Cat, Owner, Appointment, etc.
Object      -> max, luna, owner, vet, appointment, record, payment
Encapsulation -> private fields + public properties
Abstraction -> simple methods hide internal operations
Inheritance -> Dog and Cat inherit from Pet
Polymorphism -> Pet reference calls overridden Dog/Cat methods

NOTE
----
The consultation fee values in Dog/Cat are demonstration values for showing
polymorphism. They can be changed later according to your group's agreed clinic
rules.
