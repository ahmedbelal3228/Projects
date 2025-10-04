
/*
   Vacation Days Calculator
   -------------------------
   This program calculates vacation days for an employee.
   The user enters the start and end date of their vacation, and the program:
     - Computes the total calendar days
     - Computes the actual working (lean) vacation days (excluding weekends)

   Author: Ahmed Belal
*/

#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

//====================================================================================================
// Basic Definitions

struct sDate
{
    int year;
    short month;
    short day;
};

//====================================================================================================

//====================================================================================================
// Utility Functions

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

//====================================================================================================
// Key Functions

// Calculates the weekday index (1 = Sunday, 7 = Saturday) using Zeller's Congruence
short getDayOrderAtWeek(const sDate &date)
{
    int a = (14 - date.month) / 12;
    int y = date.year - a;
    int m = date.month + (12 * a) - 2;

    // Core of Zeller’s formula for Gregorian calendars
    short d = (date.day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;

    return d + 1;
}

string getDayName(const sDate &date)
{
    string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    // getDayOrderAtWeek() return the order starts from 1 not 0
    int dayOrder = getDayOrderAtWeek(date) - 1;
    string dayName = days[dayOrder];
    return dayName;
}

void increaseDateByOneDay(sDate &date)
{
    int numOfDaysAtMonth = daysInMonth(date.year, date.month);

    if (date.day >= numOfDaysAtMonth)
    {
        if (date.month >= 12)
        {
            date.year++;
            date.month = 1;
            date.day = 1;
        }

        else
        {
            date.month++;
            date.day = 1;
        }
    }
    else
        date.day++;
}

int countBusinessVacationDays(sDate date, int BulkDays)
{
    int LeanDays = 0;

    for (int i = 1; i <= BulkDays; i++)
    {
        string dayName = getDayName(date);
        if (!(dayName == "Saturday" || dayName == "Friday"))
        {
            LeanDays++;
        }
        increaseDateByOneDay(date);
    }

    return LeanDays;
}

//====================================================================================================

int main()
{

    cout << "==============================\n";
    cout << "       Vacation Days Calculator\n";
    cout << "==============================\n\n";

    cout << "Enter your vacation start date:\n";
    sDate date1 = fillDate();

    cout << "\nEnter your vacation end date:\n";
    sDate date2 = fillDate();

    long daysOfDate1 = convertDateToDays(date1);
    long daysOfDate2 = convertDateToDays(date2);
    long dayBetweenTwoDates = daysOfDate2 - daysOfDate1;

    string date1Name = getDayName(date1);
    string date2Name = getDayName(date2);

    cout << "\n-----------------------------------\n";
    cout << "Vacation Start: " << date1Name << ", ";
    printDate(date1);

    cout << "Vacation End:   " << date2Name << ", ";
    printDate(date2);
    cout << "-----------------------------------\n";

    int leanDays = countBusinessVacationDays(date1, dayBetweenTwoDates);

    cout << "Total Calendar Days: " << dayBetweenTwoDates << endl;
    cout << "Actual Working (Lean) Vacation Days: " << leanDays << endl;

    return 0;
}