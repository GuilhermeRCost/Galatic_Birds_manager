Galactic_Bird_Manager: 
Final project of the course ALGORITHMS AND COMPUTER PROGRAMMING developed in C language

Context: 
It's the year 2030. Billionaire Jeff Elon Branson begins suborbital voyages of his most recent company Galatic Birds, in which ordinary citizens can take trips to the Kármán Line, located 100 km above sea level.

As passenger demand is large, a computer program that can manage flights and bookings made is needed.

The flights are represented by the following data: flight number, maximum number of passengers, departure date and time, flight status (active or canceled).

Reservations are represented by the following data: booking number, flight number, CPF passenger, passenger's full name, gender, date of birth, booking status (active or cancelled).

The must program provides the options menu below: Options menu:

Register Flight
Register Booking
Consult Flight
Consult Booking
Consult Passenger
Cancel Flight
Cancel Booking
Delete Flight
Exit the program
Each menu item must be performed by a specific procedure. Additional functions can be developed to make the program modularized

Regarding Data Integrity
a) do not allow the inclusion of incorrect date and time.
b) do not allow the inclusion of an incorrect or repeated flight number. The flight number must always be in the JEB-XXXX format, where X represents a numeric digit.
c) do not allow the inclusion of an incorrect or repeated booking number. The booking number must always be in GB-XXXXXX format, where X represents a numeric digit.
d) do not allow bookings to flights that do not exist.
e) do not allow the inclusion of incorrect passenger data, such as CPF, name, date of birth and sex.
f) do not allow two or more active bookings for the same passenger on the same flight.
g) do not allow the cancellation of a flight or booking that is from a date prior to the current date.

Regarding Functionalities
a) the program must keep the data stored in file.
b) when consulting a flight, the occupancy percentage and the list of passengers with active bookings for that flight must be shown.
c) when consulting bookings, all data from that reservation must be shown.
d) when consulting passengers, the data of all reservations made by that passenger must be shown, whether they are active or canceled.
e) on cancelation of a flight, all reservations linked to it must also be canceled.
f) in the exclusion of a flight, all reservations linked to it must also be excluded.
