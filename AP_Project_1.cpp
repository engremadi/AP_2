#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <ctime>
#include <algorithm>
using namespace std;

int generateID() {
    static int id = 1;
    return id++;
}

// Helper function to get current time in timestamp format
time_t stringToTime(const string& dateTime) {
    struct tm t = {};
    stringstream ss(dateTime);
    ss >> get_time(&t, "%Y-%m-%d %H:%M");
    return mktime(&t);
}

string timeToString(time_t t) {
    char buffer[20];
    strftime(buffer, 20, "%Y-%m-%d %H:%M", localtime(&t));
    return string(buffer);
}

struct Passenger {
    int id;
    string name;
    string passport;
    string nationalCode;
    string phone;
    double balance;

    string toCSV() const {
        return to_string(id) + "," + name + "," + passport + "," + nationalCode + "," + phone + "," + to_string(balance);
    }

    static Passenger fromCSV(const string& line) {
        Passenger p;
        stringstream ss(line);
        string field;
        getline(ss, field, ','); p.id = stoi(field);
        getline(ss, p.name, ',');
        getline(ss, p.passport, ',');
        getline(ss, p.nationalCode, ',');
        getline(ss, p.phone, ',');
        getline(ss, field, ','); p.balance = stod(field);
        return p;
    }
};

struct Flight {
    int id;
    string number;
    string origin, destination;
    time_t dateTime;
    int seats;
    double price;

    string toCSV() const {
        return to_string(id) + "," + number + "," + origin + "," + destination + "," + timeToString(dateTime) + "," + to_string(seats) + "," + to_string(price);
    }

    static Flight fromCSV(const string& line) {
        Flight f;
        stringstream ss(line);
        string field;
        getline(ss, field, ','); f.id = stoi(field);
        getline(ss, f.number, ',');
        getline(ss, f.origin, ',');
        getline(ss, f.destination, ',');
        getline(ss, field, ','); f.dateTime = stringToTime(field);
        getline(ss, field, ','); f.seats = stoi(field);
        getline(ss, field, ','); f.price = stod(field);
        return f;
    }
};

struct Reservation {
    int id;
    int passenger_id;
    int flight_id;
    double amount_paid;
    bool refunded = false;

    string toCSV() const {
        return to_string(id) + "," + to_string(passenger_id) + "," + to_string(flight_id) + "," + to_string(amount_paid) + "," + (refunded ? "1" : "0");
    }

    static Reservation fromCSV(const string& line) {
        Reservation r;
        stringstream ss(line);
        string field;
        getline(ss, field, ','); r.id = stoi(field);
        getline(ss, field, ','); r.passenger_id = stoi(field);
        getline(ss, field, ','); r.flight_id = stoi(field);
        getline(ss, field, ','); r.amount_paid = stod(field);
        getline(ss, field, ','); r.refunded = field == "1";
        return r;
    }
};

class AirlineSystem {
    map<int, Passenger> passengers;
    map<int, Flight> flights;
    map<int, Reservation> reservations;

public:
    void loadAll();
    void saveAll();
    void addPassenger(const Passenger&);
    void addFlight(const Flight&);
    void makeReservation(int passenger_id, int flight_id);
    void cancelReservation(int reservation_id);
    void searchPassenger(string keyword);
    void searchFlight(string keyword);
    void listAll();
};