/*
  CalcPeriodBetween2Dates.cpp
  ---------------------------
  Purpose:
    Read two calendar dates and compute the period length between them:
      - Calendar days (exclusive)
      - Calendar days (inclusive = exclusive + 1)

  How it works:
    - Validates input per month length (handles leap years).
    - Normalizes the period so start <= end.
    - Converts each date to "days since 1/1/0001" and subtracts.

  I/O:
    Input  : Start date, End date (YYYY, MM, DD).
    Output : Echo of the entered period and both day counts,
             with a note that "inclusive" counts both endpoints.


  Notes:
    - Inclusive count = exclusive + 1 (e.g., 1/3 to 1/3 → 0 vs 1).
    - Uses a 12-entry static const month-days table; February handled via isLeapYear().
*/

#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

//====================================================================================================
// Utility Functions

struct sDate
{
    short day;
    short month;
    int year;
};

int readNum(string message = "Please enter a number: ", int start = 0, int end = 1000000)
{

    while (true)
    {

        cout << message;
        short n;
        cin >> n;

        if (!cin.fail() && start <= n && n <= end)
            return n;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number ! \n";
    }
}

// Checks whether a given year is a leap year
bool isLeapYear(int year)
{
    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}

// Returns the number of days in a given month, accounting for leap years
short daysInMonth(int year, short month)
{
    static const short NumberOfDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return month == 2 ? (isLeapYear(year) ? 29 : 28) : NumberOfDays[month - 1];
}

sDate fillDate()
{
    cout << "-------------------------\n";
    int year = readNum("Enter year: ");
    short month = readNum("Enter month: ", 1, 12);
    short maxMonthDays = daysInMonth(year, month);
    short day = readNum("Enter day: ", 1, maxMonthDays);

    sDate date;

    date.day = day;
    date.month = month;

    date.year = year;

    return date;
}

// Output a date in the format: DD / MM / YYYY
void printDate(sDate date, string sep = " / ")
{
    cout << "Date: ";
    cout << date.day << sep << date.month << sep << date.year << endl;
}

// Function: countDaysInDate
// Purpose: Converts a given date into the total number of days that have passed
//          since 1/1/0001. This is useful for comparing two dates or calculating
//          differences (e.g., finding age in days).
// Parameters:
//      date - The date to convert (year, month, day).
// Returns:
//      Total number of days from 1/1/0001 up to the given date.
long countDaysInDate(sDate date)
{
    long daysSum = 0;

    // Step 1: Add days for all full years before the given year
    for (int year = 1; year < date.year; ++year)
    {
        // Leap years have 366 days, normal years have 365
        daysSum += isLeapYear(year) ? 366 : 365;
    }

    // Step 2: Add days for all full months before the given month in the current year
    for (int month = 1; month < date.month; month++)
    {
        // Add days in each completed month
        daysSum += daysInMonth(date.year, month);
    }

    // Step 3: Add the days in the current month
    daysSum += date.day;

    return daysSum;
}

long countDaysInDate(sDate date)
{
    long daysSum = 0;

    // Step 1: Add days for all full years before the given year
    for (int year = 1; year < date.year; ++year)
    {
        // Leap years have 366 days, normal years have 365
        daysSum += isLeapYear(year) ? 366 : 365;
    }

    // Step 2: Add days for all full months before the given month in the current year
    for (int month = 1; month < date.month; month++)
    {
        // Add days in each completed month
        daysSum += daysInMonth(date.year, month);
    }

    // Step 3: Add the days in the current month
    daysSum += date.day;

    return daysSum;
}

//====================================================================================================
// Returns true if date1 is earlier than date2
bool isD1BeforeD2(sDate date1, sDate date2)
{
    if (date1.year != date2.year)
        return date1.year < date2.year;
    if (date1.month != date2.month)
        return date1.month < date2.month;
    return date1.day < date2.day;
}

// Returns true if both dates are exactly equal (same year, month, and day)
bool isDateEqual(sDate date1, sDate date2)
{

    return date1.year == date2.year && date1.month == date2.month && date1.day == date2.day;
}

struct sPeriod
{
    sDate startDate;
    sDate endDate;
};

sPeriod fillPeriod()
{

    sPeriod period;
    cout << "Enter the start date: \n";
    period.startDate = fillDate();

    cout << "Enter the end date: \n";
    period.endDate = fillDate();

    return period;
}
void normalizePeriodOrder(sPeriod &p)
{
    if (isD1BeforeD2(p.endDate, p.startDate))
        swap(p.startDate, p.endDate);
}

// Purpose: Calculates the number of days between two dates by converting each date
//          into its total days since 1/1/0001 and then finding the difference.
double daysBetweenDates(sDate d1, sDate d2)
{
    // Convert both dates to their total days count and subtract
    return countDaysInDate(d2) - countDaysInDate(d1);
}

long getPeriodLength(sPeriod &p)
{
    normalizePeriodOrder(p); // make sure the order of the dates is correct
    long periodLength = daysBetweenDates(p.startDate, p.endDate);
    return periodLength;
}

int main()
{
    cout << "==============================\n";
    cout << "     Period Length Program\n";
    cout << "==============================\n\n";

    cout << "Enter Period\n";
    sPeriod p1 = fillPeriod();

    cout << "\n--------------------------------\n";
    cout << "You entered:\n";
    cout << "Period: ";
    printDate(p1.startDate);
    cout << " to ";
    printDate(p1.endDate);
    long periodLength = getPeriodLength(p1);

    cout << "Period Length: " << periodLength << endl;

    cout << "Period Length: (including end date): " << periodLength + 1 << endl;

    return 0;
}