/*
  checkOverlapPeriods.cpp
  -----------------------
  Purpose:
    Read two date periods and report whether they overlap.

  What it does:
    - Reads two periods (start/end dates).
    - Normalizes each period so start <= end.
    - Orders periods by start date.
    - Checks overlap inclusively (touching endpoints counts).

  Algorithm (inclusive overlap):
    Two closed intervals [a1,a2] and [b1,b2] overlap iff:
      a1 <= b2  AND  b1 <= a2

  I/O:
    Input : Period 1 and Period 2 (YYYY, MM, DD for start and end).
    Output: Echo of both periods + “overlap / no overlap” message.

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

short readNum(string message = "Please enter a number: ", int start = 0, int end = 1000000)
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

long convertDateToDays(const sDate &date)
{
    long Days = 0;

    for (int year = 1; year < date.year; ++year)
    {
        Days += (isLeapYear(year) ? 366 : 365);
    }

    for (int month = 1; month < date.month; ++month)
    {
        Days += daysInMonth(date.year, month);
    }

    Days += date.day;

    return Days;
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

bool isP1StartBeforeP2(const sPeriod &p1, const sPeriod &p2)
{

    return (isD1BeforeD2(p1.startDate, p2.startDate));
}

bool checkOverlapPeriods(sPeriod p1, sPeriod p2)
{
    normalizePeriodOrder(p1);
    normalizePeriodOrder(p2);

    if (isP1StartBeforeP2(p2, p1))
        swap(p1, p2);

    return (isD1BeforeD2(p2.startDate, p1.endDate) || isDateEqual(p2.startDate, p1.endDate));
}

int main()
{
    cout << "==============================\n";
    cout << "     Period Overlap Checker\n";
    cout << "==============================\n\n";

    cout << "Enter Period 1\n";
    sPeriod p1 = fillPeriod();

    cout << "\nEnter Period 2\n";
    sPeriod p2 = fillPeriod();

    cout << "\n--------------------------------\n";
    cout << "You entered:\n";
    cout << "Period 1: ";
    printDate(p1.startDate);
    cout << " to ";
    printDate(p1.endDate);
    cout << "Period 2: ";
    printDate(p2.startDate);
    cout << " to ";
    printDate(p2.endDate);
    cout << "--------------------------------\n";

    // Overlap is checked INCLUSIVELY (touching endpoints counts)
    bool overlap = checkOverlapPeriods(p1, p2);

    cout << (overlap
                 ? "Result: The two periods OVERLAP (inclusive of endpoints).\n"
                 : "Result: No overlap between the two periods.\n");

    return 0;
}