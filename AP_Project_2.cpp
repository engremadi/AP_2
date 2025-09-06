#include "AP_Project_1.cpp"

void AirlineSystem::loadAll() {
    ifstream pf("passengers.txt");
    string line;
    while (getline(pf, line)) {
        Passenger p = Passenger::fromCSV(line);
        passengers[p.id] = p;
    }
    pf.close();

    ifstream ff("flights.txt");
    while (getline(ff, line)) {
        Flight f = Flight::fromCSV(line);
        flights[f.id] = f;
    }
    ff.close();

    ifstream rf("reservations.txt");
    while (getline(rf, line)) {
        Reservation r = Reservation::fromCSV(line);
        reservations[r.id] = r;
    }
    rf.close();
}

void AirlineSystem::saveAll() {
    ofstream pf("passengers.txt");
    for (const auto& [id, p] : passengers)
        pf << p.toCSV() << endl;
    pf.close();

    ofstream ff("flights.txt");
    for (const auto& [id, f] : flights)
        ff << f.toCSV() << endl;
    ff.close();

    ofstream rf("reservations.txt");
    for (const auto& [id, r] : reservations)
        rf << r.toCSV() << endl;
    rf.close();
}

void AirlineSystem::addPassenger(const Passenger& p) {
    passengers[p.id] = p;
}

void AirlineSystem::addFlight(const Flight& f) {
    flights[f.id] = f;
}

void AirlineSystem::makeReservation(int pid, int fid) {
    if (!passengers.count(pid)) throw runtime_error("PassengerNotFound");
    if (!flights.count(fid)) throw runtime_error("FlightNotFound");

    Flight& f = flights[fid];
    if (f.seats <= 0) throw runtime_error("FlightFull");
    if (f.dateTime < time(nullptr)) throw runtime_error("FlightExpired");

    Passenger& p = passengers[pid];
    if (p.balance < f.price) throw runtime_error("InsufficientBalance");

    Reservation r;
    r.id = generateID();
    r.passenger_id = pid;
    r.flight_id = fid;
    r.amount_paid = f.price;

    f.seats--;
    p.balance -= f.price;
    reservations[r.id] = r;
}

void AirlineSystem::cancelReservation(int rid) {
    if (!reservations.count(rid)) throw runtime_error("ReservationNotFound");
    Reservation& r = reservations[rid];
    if (r.refunded) throw runtime_error("RefundNotAllowed");

    Flight& f = flights[r.flight_id];
    time_t now = time(nullptr);
    double hoursLeft = difftime(f.dateTime, now) / 3600.0;
    if (f.dateTime < now) throw runtime_error("FlightCompleted");

    double refund = 0;
    if (hoursLeft >= 48) refund = r.amount_paid * 0.9;
    else if (hoursLeft >= 24) refund = r.amount_paid * 0.5;
    else refund = 0;

    passengers[r.passenger_id].balance += refund;
    r.refunded = true;
    f.seats++;
}

void AirlineSystem::searchPassenger(string keyword) {
    for (const auto& [id, p] : passengers) {
        if (to_string(p.id) == keyword ||
            p.name.find(keyword) != string::npos ||
            p.passport == keyword ||
            p.nationalCode == keyword) {
            cout << "Passenger: " << p.toCSV() << endl;
        }
    }
}

void AirlineSystem::searchFlight(string keyword) {
    for (const auto& [id, f] : flights) {
        if (to_string(f.id) == keyword ||
            f.number == keyword ||
            f.origin == keyword ||
            f.destination == keyword) {
            cout << "Flight: " << f.toCSV() << endl;
        }
    }
}

void AirlineSystem::listAll() {
    cout << "\n--- All Passengers ---\n";
    for (const auto& [id, p] : passengers) cout << p.toCSV() << endl;
    cout << "\n--- All Flights ---\n";
    for (const auto& [id, f] : flights) cout << f.toCSV() << endl;
    cout << "\n--- All Reservations ---\n";
    for (const auto& [id, r] : reservations) cout << r.toCSV() << endl;
}
