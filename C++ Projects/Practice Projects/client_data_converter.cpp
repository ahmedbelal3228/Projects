#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

/*
=======================================
Client Data Converter - C++ Program
=======================================

This utility allows you to:
1. Convert delimited client record lines into structured C++ objects.
2. Convert structured client data into delimited text lines.

Supported Features:
- Interactive input and output using standard console
- Customizable field delimiter
- Modular, readable, and reusable code

Data Fields per Client:
- Account Number
- PIN Code
- Full Name
- Phone Number
- Account Balance

Author: [Your Name]
Purpose: Practice project for C++ string processing, struct handling, and console I/O

*/

// Reads a full line string from standard input
string readString(string message = "Please enter a string: ")
{

    cout << message;
    string s;
    getline(cin, s);
    return s;
}

short readNum(string message = "Please enter a number: ")
{
    while (true)
    {

        cout << message;
        short n;
        cin >> n;

        if (!cin.fail())
            return n;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number.\n";
    }
}

/*
 * This function splits the input string `s` into substrings separated by the specified `delimiter`.
 * Each non-empty substring is added to the vector `vWords`.
 */
void splitString(string s, vector<string> &vWords, string delimiter)
{

    short pos = 0;   // Position of the delimiter in the string
    string tempWord; // Temporary string to hold the current word

    // Loop as long as the delimiter is found in the string
    while ((pos = s.find(delimiter)) != std::string::npos)

    {
        // Extract the substring from the start up to the delimiter position
        tempWord = s.substr(0, pos);

        // Add the substring to the vector only if it is not empty
        if (tempWord != "")
            vWords.push_back(tempWord);

        // Remove the extracted word and the delimiter from the original string
        s.erase(0, pos + delimiter.length());
    }

    // After the loop ends, add any remaining part of the string as the last word if not empty
    if (s != "")
        vWords.push_back(s);
}
// Represents a bank client with basic account and contact information
struct sClient
{
    string accountNumber;
    string pinCode;
    string fullName;
    string phone;
    double accountBalance;
};

// Prints all fields of a single client in a formatted layout
void displayClientsAsLinestruct(const sClient client)
{
    cout << "Account Number : " << client.accountNumber << "\n";
    cout << "Pin Code       : " << client.pinCode << "\n";
    cout << "Full Name      : " << client.fullName << "\n";
    cout << "Phone          : " << client.phone << "\n";
    cout << "Balance        : " << client.accountBalance << "\n";
}

// Iterates through a list of clients and prints each one using displayClientsAsLinestruct
void displayClientsAsLinesVector(vector<sClient> &vClients)
{
    short n = 1;
    for (sClient &client : vClients)
    {
        cout << "----------------------------------\n";
        cout << "• Client [" << n << "]\n";
        displayClientsAsLinestruct(client);
        n++;
    }
    cout << "----------------------------------\n";
}

// Converts a vector of strings (representing client fields) into a structured sClient
sClient parseClientRecord(const vector<string> &vClient)
{
    // Assumes the vector contains exactly 5 elements in the correct order
    sClient client;
    client.accountNumber = vClient[0];
    client.pinCode = vClient[1];
    client.fullName = vClient[2];
    client.phone = vClient[3];
    client.accountBalance = stod(vClient[4]); // Convert string to double
    return client;
}
// Converts a client struct into a delimited string representation for output or storage
string formatClientAsLine(sClient client, string delim)
{
    return client.accountNumber + delim +
           client.pinCode + delim +
           client.fullName + delim +
           client.phone + delim +
           to_string(client.accountBalance);
}

// Reads client details from user input to construct a complete sClient record
sClient readClientInfo(short n)
{
    sClient client;
    cout << "\nEntering details for Client [" << n << "]\n";

    cout << "Account Number : ";
    getline(cin, client.accountNumber);

    cout << "PIN Code       : ";
    getline(cin, client.pinCode);

    cout << "Full Name      : ";
    getline(cin, client.fullName);

    cout << "Phone Number   : ";
    getline(cin, client.phone);

    cout << "Account Balance: ";
    cin >> client.accountBalance;

    // Clear remaining newline to prepare for the next getline()
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return client;
}

// Reads multiple clients from user input and stores them in the vClients vector
void inputMultipleClients(int numOfClients, vector<sClient> &vClients)
{
    for (short n = 1; n <= numOfClients; ++n)
    {
        // Prompt user to input details for client n
        sClient client = readClientInfo(n);
        vClients.push_back(client);
    }

    // Display all collected client records in formatted structure
    displayClientsAsLinesVector(vClients);
}

// Outputs all clients in delimited-line format for data export or file writing
void displayClientsAsLines(vector<sClient> &vClients, string delim)
{

    short n = 1;
    for (sClient &client : vClients)
    {
        cout << "----------------------------------\n";
        cout << "Client [" << n++ << "]\n";
        string clientRecord = formatClientAsLine(client, delim);
        cout << clientRecord << endl;
        cout << "----------------------------------\n";
    }
}

// Prompts the user to enter client data as delimited strings,
// then parses and stores them as structured client records
void convertLineToRecord(short numOfClients, vector<sClient> &vClients)
{
    string delim = readString("Enter the field delimiter used in client records: ");
    for (short n = 1; n <= numOfClients; ++n)
    {
        string line = readString("Enter full client record line: ");
        vector<string> vClientString;
        splitString(line, vClientString, delim);
        sClient client = parseClientRecord(vClientString);
        vClients.push_back(client);
    }

    // Display parsed records for verification
    displayClientsAsLinesVector(vClients);
}

// Collects client data through separate prompts, stores them,
// then prints all records in a single delimited-line format
void ConvertRecordToLine(int numOfClients, vector<sClient> &vClients)
{
    inputMultipleClients(numOfClients, vClients); // field-by-field entry
    string delim = readString("Enter the delimiter to use for output: ");
    displayClientsAsLines(vClients, delim);
}

// Displays the main menu and reads user selection
short displayMainMenuAndGetChoice()
{
    cout << "\n========== Client Data Converter ==========\n";
    cout << "1. Convert line-based input to structured client records\n";
    cout << "2. Convert client records to delimited text lines\n";
    cout << "===========================================\n";
    return readNum("Select an option (1 or 2): ");
}

int main()
{

    vector<sClient> vClients;
    short userChoice = displayMainMenuAndGetChoice();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    short numOfClients = readNum("Enter the number of clients: ");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (userChoice == 1)
        convertLineToRecord(numOfClients, vClients);
    else if (userChoice == 2)
        ConvertRecordToLine(numOfClients, vClients);
    else
        cout << "Ivalid Choice !\n ";
    return 0;
}