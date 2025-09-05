# ✈️ Flight Booking System (C++)

📌 Overview

The Flight Booking System is a C++ console-based application that simulates a real-world airline reservation system.
It allows users to search for flights, view details, and calculate ticket prices based on seat class.
This project demonstrates Object-Oriented Programming (OOP) concepts such as inheritance, polymorphism, and abstraction.

🚀 Features

✅ Add and display available flights
✅ View flight details (number, destination, departure time, etc.)
✅ Calculate ticket prices by seat class (Economy, Business, First Class)
✅ Prevents duplicate flight entries
✅ Uses pure virtual classes for extensibility

🛠️ Tech Stack

Language: C++
Concepts Used:
Classes & Objects
Inheritance & Polymorphism
Virtual Functions
Vectors & STL
Date/Time handling

📂 Project Structure

FlightBookingSystem/
│── main.cpp              # Main entry point
│── FlightBase.h          # Abstract base class
│── Flight.h              # Flight class implementation
│── FlightManager.h       # Flight manager with vector-based storage
│── README.md             # Documentation

⚡ How to Run

1. Clone the repository 
 cd FlightBookingSystem

2. Compile the program
 g++ main.cpp -o flight_booking

3. Run the executable
  ./flight_booking

📸 Example Output

Available Flights:
------------------
Flight Number : AI101
Destination   : New York
Departure Time: 2025-09-05 14:30
Class         : Business
Price         : 45000 INR


🎯 Future Improvements

🔹 Add passenger booking system (name, age, ticket confirmation)
🔹 Store data in a file/DB instead of memory
🔹 Implement search & filter for flights
🔹 Add seat availability tracking

👨‍💻 Author
Gummadi Balaram
📧 gummadibu@gmail.com
