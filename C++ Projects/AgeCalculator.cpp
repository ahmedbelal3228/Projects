#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

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

struct sDate
{

    int year;
    short month;
    short day;
};

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

bool isD1EarlierThanD2(sDate date1, sDate date2)
{
    if (date1.year != date2.year)
        return date1.year < date2.year;
    if (date1.month != date2.month)
        return date1.month < date2.month;
    return date1.day < date2.day;
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
        // daysSum += static_cast<long>(daysInMonth(date.year, month));
    }

    // Step 3: Add the days in the current month
    daysSum += date.day;

    return daysSum;
}

// Purpose: Calculates the number of days between two dates by converting each date
//          into its total days since 1/1/0001 and then finding the difference.
long daysBetweenDates(sDate d1, sDate d2)
{

    if (!isD1EarlierThanD2(d1, d2))
    {
        swap(d1, d2);
    }
    // Convert both dates to their total days count and subtract
    return countDaysInDate(d2) - countDaysInDate(d1);
}

long getAgeInDays(sDate birthDate)
{
    sDate currentDate = getCurrentDate();

    // Calculate the difference in days between the birth date and today's date.
    // This is the foundation for breaking the age into years, months, and days.
    long ageInDays = daysBetweenDates(currentDate, birthDate);
    return ageInDays;
}

struct sAge
{
    // Initialize to 0 to avoid garbage values
    int years = 0;
    short months = 0;
    short days = 0;
};

// Calculates age in years, months, and days from the given birth date.
sAge getAgeFromBirthDate(sDate birthDate)
{
    sAge age; // Starts at 0 years, 0 months, 0 days

    long AgeInDays = getAgeInDays(birthDate);

    // --- YEARS ---
    // Subtract complete years from the total days.
    // We check leap years dynamically to ensure correct day count.
    while (AgeInDays >= (isLeapYear(birthDate.year) ? 366 : 365))
    {
        AgeInDays -= (isLeapYear(birthDate.year) ? 366 : 365);
        age.years++;
        birthDate.year++; // Move forward year by year
    }

    // --- MONTHS ---
    // Subtract complete months from the remaining days.
    // We increment month and adjust the year when passing December.
    while (AgeInDays >= daysInMonth(birthDate.year, birthDate.month))
    {
        AgeInDays -= daysInMonth(birthDate.year, birthDate.month);
        age.months++;
        birthDate.month++;

        // Wrap month > 12 back to January and increment year
        if (birthDate.month > 12)
        {
            birthDate.month = 1;
            birthDate.year++;
        }
    }

    // --- DAYS ---
    // Whatever remains is the leftover days after full years and months are removed.
    age.days = AgeInDays;

    return age;
}

// Prints the age in a clear, user-friendly format with proper pluralization.
// Why: Ensures output is grammatically correct and professional for any age values.
void printAge(sAge age)
{
    cout << "==============================" << endl;
    cout << "       AGE CALCULATION        " << endl;
    cout << "==============================" << endl;

    // Years: add 's' only if more than one year
    cout << "Years : " << age.years
         << " Year" << (age.years != 1 ? "s" : "") << endl;

    // Months: same pluralization logic
    cout << "Months: " << age.months
         << " Month" << (age.months != 1 ? "s" : "") << endl;

    // Days: same pluralization logic
    cout << "Days  : " << age.days
         << " Day" << (age.days != 1 ? "s" : "") << endl;
}
int main()
{
    cout << "Entering birth date...\n";
    sDate birthDate = fillDate();
    sAge age;
    age = getAgeFromBirthDate(birthDate);
    printAge(age);

    return 0;
}