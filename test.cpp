#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "AP_Project_1.cpp"

TEST_CASE("Passenger Registration and Retrieval") {
    AirlineSystem sys;
    Passenger p = {1, "Ali", "P1234", "N111", "0912", 1000};
    sys.addPassenger(p);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    sys.searchPassenger("Ali");
    std::cout.rdbuf(old);
    REQUIRE(buffer.str().find("Ali") != std::string::npos);
}

TEST_CASE("Flight Registration and Search") {
    AirlineSystem sys;
    Flight f = {1, "FL001", "Tehran", "Mashhad", stringToTime("2025-12-01 10:00"), 2, 500};
    sys.addFlight(f);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    sys.searchFlight("Tehran");
    std::cout.rdbuf(old);
    REQUIRE(buffer.str().find("Tehran") != std::string::npos);
}

TEST_CASE("Successful Reservation") {
    AirlineSystem sys;
    Passenger p = {1, "Ali", "P1234", "N111", "0912", 1000};
    Flight f = {1, "FL001", "Tehran", "Shiraz", stringToTime("2025-12-10 10:00"), 1, 500};
    sys.addPassenger(p);
    sys.addFlight(f);

    sys.makeReservation(1, 1);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    sys.listAll();
    std::cout.rdbuf(old);
    REQUIRE(buffer.str().find("Ali") != std::string::npos);
    REQUIRE(buffer.str().find("FL001") != std::string::npos);
}

TEST_CASE("Reservation Failure - FlightFull") {
    AirlineSystem sys;
    Passenger p1 = {1, "Ali", "P1234", "N111", "0912", 1000};
    Passenger p2 = {2, "Sara", "P5678", "N222", "0935", 1000};
    Flight f = {1, "FL002", "Tehran", "Tabriz", stringToTime("2025-12-15 15:00"), 1, 300};
    sys.addPassenger(p1);
    sys.addPassenger(p2);
    sys.addFlight(f);

    sys.makeReservation(1, 1);
    REQUIRE_THROWS_WITH(sys.makeReservation(2, 1), "FlightFull");
}

TEST_CASE("Reservation Refund and Balance Update") {
    AirlineSystem sys;
    Passenger p = {1, "Ali", "P1234", "N111", "0912", 1000};
    Flight f = {1, "FL003", "Tehran", "Ahvaz", time(nullptr) + 60 * 60 * 49, 1, 300}; // 49 hrs later
    sys.addPassenger(p);
    sys.addFlight(f);
    sys.makeReservation(1, 1);

    sys.cancelReservation(1);
    REQUIRE(sys.getPassenger(1).balance >= 970); // 90% of 300 refunded
} 
