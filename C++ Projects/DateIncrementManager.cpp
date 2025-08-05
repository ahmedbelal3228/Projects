#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

/*
    DateIncrementManager.cpp
    -------------------------
    A C++ program to increase a user-provided date by days, weeks, months, or years.

    Concepts practiced in this project:
      - enum (menu options for increments)
      - struct (sDate, sAddedXValToDate for organizing data)
      - Functions & modular design (clear separation of tasks)
      - Input validation (numeric ranges, correct days per month)
      - Leap year calculations
      - Loops & conditional logic (while, switch, if)
      - Clear and maintainable code formatting
      - User interaction & formatted console output

    Author: [Ahmed Belal]

*/

struct sDate
{

    int year;
    short month;
    short day;
};

// ******************************************* UTILITY FUNCTIONS *******************************************
// ************************************************************************************************************************

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

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string sToLower(string s)
{
    for (char &c : s)
    {
        if (isupper(c))
            tolower(c);
    }

    return s;
}

bool isSure(string message = "Are you sure?: ")
{

    string answer;
    string lowerAnswer;
    while (true)
    {
        cout << message;
        getline(cin >> ws, answer);
        lowerAnswer = sToLower(answer);

        if (!lowerAnswer.empty())
        {
            char firstLowerChar = lowerAnswer[0];
            if (firstLowerChar == 'y')
                return true;
            if (firstLowerChar == 'n')
                return false;
        }

        cout << "Please answer with Yes or No (y/n).\n";
    }
}

// Output a date in the format: DD / MM / YYYY
void printDate(sDate date, string sep = " / ")
{

    cout << date.day << sep << date.month << sep << date.year << endl;
}

// Checks whether a given year is a leap year
bool isLeapYear(int year)
{
    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}

sDate getCurrentDate()
{
    using namespace chrono;

    // Get current time
    auto now = system_clock::now();

    // Convert to time_t
    time_t now_time_t = system_clock::to_time_t(now);

    // Convert to local time
    tm localTime{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&localTime, &now_time_t); // Windows safe version
#else
    localtime_r(&now_time_t, &localTime); // Linux/Mac safe version
#endif

    sDate currentDate;
    currentDate.year = 1900 + localTime.tm_year; // Adjust year
    currentDate.month = 1 + localTime.tm_mon;    // Adjust month (0–11 → 1–12)
    currentDate.day = localTime.tm_mday;

    return currentDate;
}

// Returns the number of days in a given month, accounting for leap years
short daysInMonth(int year, short month)
{
    int NumberOfDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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

// Returns true if date1 is earlier than date2
bool isD1EarlierThanD2(sDate date1, sDate date2)

{

    return date1.year < date2.year ? true : date1.month < date2.month ? true
                                        : date1.day < date2.day       ? true
                                                                      : false;
}

// Function: countDaysInDate
// Purpose: Converts a given date into the total number of days that have passed
//          since 1/1/0001. This is useful for comparing two dates or calculating
//          differences (e.g., finding age in days).
// Parameters:
//      date - The date to convert (year, month, day).
// Returns:
//      Total number of days from 1/1/0001 up to the given date.
double countDaysInDate(sDate date)
{
    double daysSum = 0;

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
        daysSum += static_cast<double>(daysInMonth(date.year, month));
    }

    // Step 3: Add the days in the current month
    daysSum += date.day;

    return daysSum;
}

// Calculates age in years, months, and days from the given birth date.
sDate convertDaysToDate(long Days)
{
    sDate date;
    sDate standardDate = {0001, 1, 1};
    // --- YEARS ---
    // Subtract complete years from the total days.
    // We check leap years dynamically to ensure correct day count.
    while (Days >= (isLeapYear(standardDate.year) ? 366 : 365))
    {
        Days -= (isLeapYear(standardDate.year) ? 366 : 365);
        standardDate.year++; // Move forward year by year
    }

    // --- MONTHS ---
    // Subtract complete months from the remaining days.
    // We increment month and adjust the year when passing December.
    while (Days > daysInMonth(standardDate.year, standardDate.month))
    {
        Days -= daysInMonth(standardDate.year, standardDate.month);
        standardDate.month++;

        // Wrap month > 12 back to January and increment year
        if (standardDate.month > 12)
        {
            standardDate.month = 1;
            standardDate.year++;
        }
    }

    // --- DAYS ---
    // Whatever remains is the leftover days after full years and months are removed.
    standardDate.day = Days;
    date = standardDate;

    return date;
}

// *********************************** INCREASING DATE ************************************
// ************************************************************************************************************************

enum enIncreaseByWhat
{
    Day = 1,
    Week,
    Month,
    Year,
    Exit
};

struct sAddedXValToDate
{
    int xDays = 0;
    int xWeeks = 0;
    int xMonths = 0;
    int xYears = 0;
};

sDate increaseDateByXDays(sDate &date, int xDays)
{

    long Days = countDaysInDate(date);

    long totalDays = Days + xDays;
    date = convertDaysToDate(totalDays);
    return date;
}

sDate increaseDateByXWeeks(sDate &date, int xWeeks)
{
    int xDays = xWeeks * 7;
    date = increaseDateByXDays(date, xDays);
    return date;
}

sDate increaseDateByXMonths(sDate &date, int xMonths)
{

    for (int month = 1; month <= xMonths; ++month)
    {
        date.month++;

        if (date.month > 12)
        {
            date.month = 1;
            date.year++;
        }
    }

    return date;
}

sDate increaseDateByXYears(sDate &date, int xYears)
{

    for (int year = 1; year <= xYears; ++year)
    {
        date.year++;
    }
    return date;
}

void exitMessage()
{
    cout << "\n=========================================\n";
    cout << " Thank you for using Date Increment Manager!\n";
    cout << " Have a great day!\n";
    cout << "=========================================\n";
}

void displayOption()
{
    cout << "-----------------------------\n";
    cout << "Choose how you want to increase the date:\n";
    cout << "1. Increase by Days\n";
    cout << "2. Increase by Weeks\n";
    cout << "3. Increase by Months\n";
    cout << "4. Increase by Years\n";
    cout << "5. Exist\n";
    cout << "-----------------------------\n";
}

enIncreaseByWhat readUserInput()

{

    short firstOptionNum = static_cast<short>(enIncreaseByWhat::Day);
    short lastOptionNum = static_cast<short>(enIncreaseByWhat::Exit);
    short userChoice;
    while (true)
    {
        userChoice = readNum("Choose option: ", firstOptionNum, lastOptionNum);
        break;
    }
    return static_cast<enIncreaseByWhat>(userChoice);
}

void showUpdateInDate(sDate newDate, int addedVal, string unitName = " Day", string sep = " / ")
{
    cout << "New Date After Adding " << addedVal << unitName << (addedVal > 1 ? "s" : "") << endl;
    cout << newDate.day << sep << newDate.month << sep << newDate.year << endl;
}

sDate increaseDate(sDate date, sDate &newDate, enIncreaseByWhat enChoice, sAddedXValToDate &xVal)
{

    switch (enChoice)
    {
    case (enIncreaseByWhat::Day):
    {
        xVal.xDays = readNum("Enter the number of Days: ");
        newDate = increaseDateByXDays(newDate, xVal.xDays);
        showUpdateInDate(newDate, xVal.xDays, " Day");
        break;
    }

    case (enIncreaseByWhat::Week):
    {
        xVal.xWeeks = readNum("Enter the number of Weeks: ");
        newDate = increaseDateByXWeeks(newDate, xVal.xWeeks);
        showUpdateInDate(newDate, xVal.xWeeks, " Week");

        break;
    }
    case (enIncreaseByWhat::Month):
    {
        xVal.xMonths = readNum("Enter the number of Months: ");
        newDate = increaseDateByXMonths(newDate, xVal.xMonths);
        showUpdateInDate(newDate, xVal.xMonths, " Month");

        break;
    }
    case (enIncreaseByWhat::Year):
    {
        xVal.xYears = readNum("Enter the number of Years: ");
        newDate = increaseDateByXYears(newDate, xVal.xYears);
        showUpdateInDate(newDate, xVal.xYears, " Year");
        break;
    }

    case Exit:
        exitMessage();
        break;

    default:
        break;
    }

    return newDate;
}

void compareOldAndNewDate(const sDate oldDate, const sDate newDate, const sAddedXValToDate xVal)
{
    cout << "\n================= DATE COMPARISON =================\n\n";

    cout << "•Original Date  : ";
    printDate(oldDate);

    cout << "•Updated Date   : ";
    printDate(newDate);

    cout << "\n•Added Values: \n"
         << "["
         << xVal.xYears << "] Year(s)\n"
         << "["
         << xVal.xMonths << "] Month(s)\n"
         << "["
         << xVal.xWeeks << "] Week(s)\n"
         << "["
         << xVal.xDays << "] Day(s)" << endl;

    cout << "==================================================\n";
}

void runDateIncrementManager(const sDate date, sDate &newDate, sAddedXValToDate &xVal)
{
    enIncreaseByWhat enChoice;

    while (true)
    {
        cout << "\n-----------------------------\n";
        cout << "Updated Date: ";
        printDate(newDate);

        displayOption();

        enChoice = readUserInput();
        newDate = increaseDate(date, newDate, enChoice, xVal);

        if (enChoice != Exit)
        {
            if (!isSure("Do you want continue adding on the date: "))
            {
                break;
            }
            clearScreen();
        }
        else
            break;
    }

    compareOldAndNewDate(date, newDate, xVal);
}
int main()
{
    cout << "\nPlease Enter Date You Want To Increment: \n";
    sDate date = fillDate();
    sDate newDate = date;
    sAddedXValToDate xVal;
    runDateIncrementManager(date, newDate, xVal);

    return 0;
}
