#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_set>
using namespace std;
class FlightBase {
public:
    virtual void displayflight() = 0;
    virtual double calculateprice(const string& seatClass) = 0;
    virtual string getflightnumber() = 0;
    virtual string getflightdestination() = 0;
    virtual string getdeparturetime() = 0;
    virtual string getarrivaltime() = 0;
    virtual ~FlightBase() {}
};
struct Seat {
    bool isAvailable;
    string type;      
    string classType; 
    double price;
};
class Flight : public FlightBase {
private:
    string airline; string flightNumber;
    string destination;
    int totalSeats; int availableSeats;
    double basePrice;
    vector<Seat> seats;
    string departuretime; string arrivaltime;
    int operationaldays;
    chrono::system_clock::time_point startdate;
public:
    Flight(string airline, string flightNo, string dest, int seats, double price, string deptime, string arrtime, int operationaldays)
        : airline(airline), flightNumber(flightNo), destination(dest), totalSeats(seats), availableSeats(seats),
          basePrice(price), departuretime(deptime), arrivaltime(arrtime), operationaldays(operationaldays), startdate() {
        startdate = chrono::system_clock::now();
        initializeSeats();
    }
    void initializeSeats() {
        for (int i = 0; i < totalSeats; ++i) {
            Seat seat;
            seat.isAvailable = true;
            if (i % 3 == 0) seat.type = "Window";
            else if (i % 3 == 1) seat.type = "Middle";
            else seat.type = "Aisle";
            if (i < totalSeats / 3) {
                seat.classType = "Business";
                seat.price = basePrice + 1000.0;
            } else if (i < (2 * totalSeats) / 3) {
                seat.classType = "Premium Economy";
                seat.price = basePrice + 500.0;
            } else {
                seat.classType = "Economy";
                seat.price = basePrice;
            }
            seats.push_back(seat);
        }
    }
    bool isDateWithinOperationalPeriod(const string& dateOfJourney) {
        chrono::system_clock::time_point endDate = startdate + chrono::hours(24 * operationaldays);
        tm tmJourney = {};
        istringstream iss(dateOfJourney);
        iss >> get_time(&tmJourney, "%Y-%m-%d");
        time_t journeyTime = mktime(&tmJourney);
        chrono::system_clock::time_point journeyDate = chrono::system_clock::from_time_t(journeyTime);
        if (journeyDate < startdate) {
            //cout << "Tickets not opened\n";
            return false;
        }
        return (journeyDate >= startdate && journeyDate <= endDate);
    }
    double calculateprice(const string& seatClass) override {
        double baseClassPrice = basePrice;
        if (seatClass == "Business") baseClassPrice += 1000.0;
        else if (seatClass == "Premium Economy") baseClassPrice += 500.0;
        double seatUsage = 1.0 - static_cast<double>(availableSeats) / totalSeats;
        if (seatUsage >= 0.8) return baseClassPrice * 2.0;
        else if (seatUsage >= 0.5) return baseClassPrice * 1.5;
        return baseClassPrice;
    }
    bool bookseat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > totalSeats || !seats[seatNumber - 1].isAvailable) {
            return false;
        }
        seats[seatNumber - 1].isAvailable = false;
        --availableSeats;
        return true;
    }
    void displayAvailableSeats(const string& seatClass = "", const string& seatType = "") {
        cout << "\nAvailable Seats:\n";
        bool found = false;
        for (int i = 0; i < totalSeats; ++i) {
            if (seats[i].isAvailable &&
                (seatClass.empty() || seats[i].classType == seatClass) &&
                (seatType.empty() || seats[i].type == seatType)) {
                cout << "Seat " << i + 1 << " (" << seats[i].type << ", " << seats[i].classType
                     << ") - Price: Rs:" << seats[i].price << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No matching seats available.\n";
        }
    }
    void displayflight() override {
        cout << endl << "Airline: " << airline << " || Flight Number: " << flightNumber << endl
             << "Destination: " << destination << endl
             << "Available Seats: " << availableSeats << "/" << totalSeats
             << "Price: Rs:" << basePrice << endl
             << "Departure Time: " << departuretime << "   Arrival Time: " << arrivaltime << endl;
    }
    string getflightnumber() override { return flightNumber; }
    string getflightdestination() override { return destination; }
    string getdeparturetime() override { return departuretime; }
    string getarrivaltime() override { return arrivaltime; }
    string getairline() { return airline; }

    static bool isValidSeatType(const string& type) {
    static unordered_set<string> validTypes;
    if (validTypes.empty()) {
        validTypes.insert("Window");
        validTypes.insert("Middle");
        validTypes.insert("Aisle");
    }
    return validTypes.count(type) > 0;
}
static bool isValidSeatClass(const string& classType) {
    static unordered_set<string> validClasses;
    if (validClasses.empty()) {
        validClasses.insert("Business");
        validClasses.insert("Premium Economy");
        validClasses.insert("Economy");
    }
    return validClasses.count(classType) > 0;
}
};
class Passenger {
private:
    string name;
    FlightBase* bookedflight;
    int seatNumber;
public:
    Passenger(string name) : name(name), bookedflight(nullptr), seatNumber(-1) {}
    void bookflight(FlightBase* flight, int seatNum, const string& seatClass, const string& dateofjourney) {
        Flight* realFlight = dynamic_cast<Flight*>(flight);
        if (realFlight == nullptr) {
            cout << "Error: Flight type mismatch\n";
            return;
        }
        if (!realFlight->isDateWithinOperationalPeriod(dateofjourney)) {
            cout << "Tickets not opened for the selected date. \n";
            return;
        }
        if (realFlight->bookseat(seatNum)) {
            bookedflight = flight;
            seatNumber = seatNum;
            cout << "\nBooking successful\nBooking Details:\n";
            cout << "Name of the passenger: " << name << endl;
            cout << "Airline: " << realFlight->getairline() << endl;
            cout << "Flight Number: " << realFlight->getflightnumber() << endl;
            cout << "Seat Number: " << seatNum << " (" << seatClass << ")" << endl;
            cout << "Destination: " << realFlight->getflightdestination() << endl;
            cout << "Departure Time: " << realFlight->getdeparturetime() << endl;
            cout << "Arrival Time: " << realFlight->getarrivaltime() << endl;
            cout << "Date of Journey: " << dateofjourney << endl;
            cout << "Booking price: Rs:" << realFlight->calculateprice(seatClass) << endl;
        } else {
            cout << "Sorry, the seat " << seatNum << " is unavailable." << endl;
        }
    }
    string getname() {
        return name;
    }
};
class ReservationSystem {
private:
    vector<FlightBase*> flights;
public:
    void addFlight(FlightBase* flight) {
        flights.push_back(flight);
    }
    void displayflights() {
        cout << "All Available Flights:\n";
        for (FlightBase* flight : flights) {
            flight->displayflight();
        }
        cout << endl;
    }
    void displayFlightsByDestination(const string& destination) {
        cout << "\nFlights to " << destination << ":\n";
        bool found = false;
        for (FlightBase* flight : flights) {
            if (flight->getflightdestination() == destination) {
                flight->displayflight();
                found = true;
            }
        }
        if (!found) {
            cout << "No flights found for the given destination.\n";
        }
    }
    void bookflightforpassenger(Passenger& passenger, string flightNo, int seatNum, string seatClass, string dateofjourney) {
        for (FlightBase* flight : flights) {
            if (flight->getflightnumber() == flightNo) {
                passenger.bookflight(flight, seatNum, seatClass, dateofjourney);
                return;
            }
        }
        cout << "Invalid flight number.\n";
    }
    vector<FlightBase*> getFlights() {
        return flights;
    }
};
int main() {
    ReservationSystem system;
    system.addFlight(new Flight("Air India", "AI101", "Mumbai", 30, 500.0, "09:00 AM", "12:00 PM", 60));
    system.addFlight(new Flight("IndiGo", "IG102", "Mumbai", 20, 400.0, "07:00 AM", "09:00 AM", 65));
    system.addFlight(new Flight("Air India", "AI201", "Delhi", 30, 500.0, "09:00 AM", "12:00 PM", 60));
    system.addFlight(new Flight("IndiGo", "IG202", "Delhi", 20, 400.0, "07:00 AM", "09:00 AM", 90));
    system.addFlight(new Flight("SpiceJet", "SJ301", "Chennai", 30, 600.0, "01:00 PM", "04:00 PM", 365));
    int choice;
    while (choice != 4) { 
    cout <<"   -----Menu-----   "<< "\n1. Display all flights\n2. Filter flights by destination\n3. Book a flight\n4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    if (choice == 1) {
        system.displayflights();
    } else if (choice == 2) {
        string destination;
        cout << "Enter destination: ";
        cin >> destination;
        system.displayFlightsByDestination(destination);
    } else if (choice == 3) {
        string name, flightNo, dateofjourney;
        int seatNum;
        string seatClass, seatType;
        cout << "Enter your name: ";
        cin >> name;
        Passenger passenger(name);
        cout << "Enter destination : ";
        cin >> flightNo;
        cout << "Searching flights for the destination...\n";
        bool flightsFound = false;
        for (FlightBase* flight : system.getFlights()) {
            if (flight->getflightdestination() == flightNo) {
                flightsFound = true;
                break;
            }
        }
        if (!flightsFound) {
            cout << "No flights available for the given destination. \n";
            continue; 
        }
        system.displayFlightsByDestination(flightNo);
        cout << "Enter flight number to book: ";
        cin >> flightNo;
        bool flightExists = false;
        for (FlightBase* flight : system.getFlights()) {
            if (flight->getflightnumber() == flightNo) {
                flightExists = true;
                break;
            }
        }
        if (!flightExists) {
            cout << "Invalid flight number. \n";
            continue; 
        }
        cout << "Enter seat class (Business, Premium Economy, Economy): ";
        cin.ignore();
        getline(cin, seatClass);
        if (!Flight::isValidSeatClass(seatClass)) {
            cout << "Invalid seat class. \n";
            continue;
        }
        cout << "Enter seat type (Window, Middle, Aisle): ";
        cin >> seatType;
        if (!Flight::isValidSeatType(seatType)) {
            cout << "Invalid seat type.\n";
            continue;
        }
        cout << "Displaying seats for your selection...\n";
        for (FlightBase* flight : system.getFlights()) {
            if (flight->getflightnumber() == flightNo) {
                Flight* realFlight = dynamic_cast<Flight*>(flight);
                if (realFlight) {
                    realFlight->displayAvailableSeats(seatClass, seatType);
                }
                break;
            }
        }
        cout << "Enter seat number: ";
        cin >> seatNum;
        cout << "Enter the date of journey (YYYY-MM-DD): ";
        cin >> dateofjourney;
        system.bookflightforpassenger(passenger, flightNo, seatNum, seatClass, dateofjourney);
    } else if (choice == 4) {
        cout << "Exiting the system. Thank you!\n";
    }else if(choice != 4){
        cout<<"Invalid choice";
    }
  }
}