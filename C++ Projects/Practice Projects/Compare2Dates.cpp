/*
  Compare2Dates.cpp
  -----------------
  A simple console program that compares two dates and reports whether
  the first is BEFORE, AFTER, or EQUAL to the second.

  Features:
    • Reads and validates two dates (handles leap years and month lengths).
    • Compares dates by year → month → day.
    • Displays both a numeric result and a descriptive message.

  Result codes:
    -1 → First date is BEFORE second
     0 → Both dates are EQUAL
     1 → First date is AFTER second
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
    int NumberOfDays[13] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
        Days += (isLeapYear(date.year) ? 366 : 365);
    }

    for (int month = 1; month < date.month; ++month)
    {
        Days += daysInMonth(date.year, month);
    }

    Days += date.day;

    return Days;
}
//====================================================================================================

bool isD1BeforeD2(sDate d1, sDate d2)
{
    if (d1.year != d2.year)
        return d1.year < d2.year;

    if (d1.month != d2.month)
        return d1.month < d2.month;

    return d1.day < d2.day;
}

bool isD1AfterD2(sDate d1, sDate d2)
{
    if (d1.year != d2.year)
        return d1.year > d2.year;

    if (d1.month != d2.month)
        return d1.month > d2.month;

    return d1.day > d2.day;
}

bool isD1EqualToD2(sDate d1, sDate d2)
{
    return (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day);
}

// ====================================================================================================

enum enCompareResult
{
    Before = -1,
    After = 1,
    Equal = 0
};

enCompareResult compare2Dates(sDate d1, sDate d2)
{
    enCompareResult result;
    if (isD1BeforeD2(d1, d2))
        return enCompareResult::Before;

    if (isD1AfterD2(d1, d2))
        return enCompareResult::After;

    return enCompareResult::Equal;
}

void compareDatesResult(sDate d1, sDate d2)
{
    enCompareResult result = compare2Dates(d1, d2);

    cout << "\n==============================\n";
    cout << "         Date Compare\n";
    cout << "==============================\n";

    cout << "First  ";
    printDate(d1);

    cout << "Second ";
    printDate(d2);

    string code = (result == Before ? "-1" : result == After ? "1"
                                                             : "0");
    string text = (result == Before  ? "First date is BEFORE second."
                   : result == After ? "First date is AFTER second."
                                     : "Both dates are EQUAL.");

    cout << "--------------------------------\n";
    cout << "Result Code : " << code << "\n";
    cout << "Meaning     : " << text << "\n";
}

int main()
{
    cout << "==============================\n";
    cout << "      Compare Two Dates\n";
    cout << "==============================\n\n";

    cout << "Enter the FIRST date:\n";
    sDate d1 = fillDate();

    cout << "\nEnter the SECOND date:\n";
    sDate d2 = fillDate();

    compareDatesResult(d1, d2);
    return 0;

    return 0;
}