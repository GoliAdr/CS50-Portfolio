-- Find the crime scene report for July 28 on Humphrey Street.
SELECT description FROM crime_scene_reports
WHERE year = 2024 AND month = 7 AND day = 28 AND street = "Humphrey Street";

/*The report says the theft happened at 10:15am at the bakery,
-- and that three witnesses mentioned the bakery.
-- So I check all interviews from that day.*/

SELECT name, transcript FROM interviews
WHERE year = 2024 AND month = 7 AND day = 28;

/*One witness saw the thief get into a car and leave the bakery parking lot
-- within minutes of the crime. I check the bakery security logs around that time.*/

SELECT * FROM bakery_security_logs
WHERE year = 2024 AND month = 7 AND day = 28
  AND hour = 10 AND minute BETWEEN 15 AND 25;

/*Another witness said the thief withdrew money earlier that morning
-- at the ATM on Leggett Street. I check withdrawals at that ATM.*/

SELECT * FROM atm_transactions
WHERE year = 2024 AND month = 7 AND day = 28
  AND atm_location = "Leggett Street"
  AND transaction_type = "withdraw";

-- To identify the people who made those transactions,
-- I join ATM transactions with bank accounts and people.

SELECT people.name, people.license_plate, atm_transactions.account_number
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.year = 2024
  AND atm_transactions.month = 7
  AND atm_transactions.day = 28
  AND atm_transactions.atm_location = "Leggett Street"
  AND atm_transactions.transaction_type = "withdraw";

/*By comparing these names and license plates with the bakery exit logs,
-- the suspects are narrowed down to Bruce, Diana, Iman, and Luca.

-- A witness reported that the thief made a phone call lasting less than a minute
-- right after leaving the bakery. I now check for short phone calls from these suspects
-- on the day of the crime.*/

SELECT * FROM phone_calls
WHERE year = 2024 AND month = 7 AND day = 28
  AND duration < 60
  AND caller IN (
    "(829) 555-5269",
    "(389) 555-5198",
    "(770) 555-1861",
    "(367) 555-5533"
  );

-- Bruce's phone number is (367) 555-5533. I now check who he called.

SELECT receiver
FROM phone_calls
WHERE caller = "(367) 555-5533"
  AND year = 2024 AND month = 7 AND day = 28;

-- I look up who owns the receiver number.

SELECT name, phone_number
FROM people
WHERE phone_number = "(375) 555-8161";

/*The phone number belongs to Robin, so Robin is the accomplice.

-- Another witness says the thief planned to take the earliest flight
-- out of Fiftyville on July 29. I check flights for that day.*/

SELECT * FROM flights
WHERE year = 2024 AND month = 7 AND day = 29
ORDER BY hour, minute;

-- The earliest flight is flight 36. I check who was on that flight.

SELECT passengers.passport_number, people.name
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = 36;

-- Bruce is the only suspect on the earliest flight, so Bruce is the thief.

-- Finally, I check the destination city of flight 36.

SELECT city
FROM airports
WHERE id = 4;


/*
Conclusion:

I determined that Bruce was the thief because he withdrew money at the Leggett Street ATM on the morning of the crime,
his license plate was recorded leaving the bakery during the time window identified by a witness, he made a short phone call right after the theft,
and he appeared as a passenger on the earliest flight leaving Fiftyville on July 29. The thief said he would take the earliest flight, and Bruce was the only suspect who did.

I identified Robin as the accomplice because Bruce called him during the short phone call after the theft, and Robin’s phone number area code matched the city Bruce fled to.

The earliest flight left for New York City, so that is where Bruce escaped.*/

