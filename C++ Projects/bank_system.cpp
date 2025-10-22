#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <optional>
#include <algorithm>

using namespace std;
/*
=======================================
Bank Client Manager — C++ Console Project
=======================================

Overview
- Manage bank clients stored as delimited text records ("#||#").
- Convert between delimited lines and structured C++ objects (sClient).
- Read, add, find, update, and delete clients with full input validation.
- Persist changes to disk using simple, predictable file I/O.

Key Features
- Interactive console UI with a main menu.
- Customizable field delimiter and modular helpers for parsing/formatting.
- Safe deletion using the remove–erase idiom for std::vector.
- On-demand reload: client vector is loaded from file when empty to avoid stale or missing data.
- In-session add: newly added clients are appended to both vNewClients and vAllClients to prevent duplicate account numbers during the same session.

Validation Rules (applied on Add and Update)
- Account Number: non-empty and unique across all clients.
- PIN Code: digits only, length = 4.
- Phone Number: non-empty, digits only, must start with "01", length = 11.
- Account Balance: valid numeric (digits with at most one '.'), value >= 0.

Data Fields per Client
- Account Number
- PIN Code
- Full Name
- Phone Number
- Account Balance

Author: Ahmed Belal
Purpose: Practice project for C++ string processing, struct handling, validation, and file I/O with a simple console interface.
*/

const string fileName = "Clients.txt";
const string delim = "#||#";

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Reads a full line string from standard input
string readString(string message = "Please enter a string: ")
{

    cout << message;
    string s;
    getline(cin, s);
    return s;
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

// take the confirmation from the user with yes/no
bool isSure(string message = "Are you sure? (y/n) : ")
{

    cout << message << endl;
    string answer;
    getline(cin, answer);
    string lowerAnswer = sToLower(answer);
    if (!lowerAnswer.empty() && lowerAnswer[0] == 'y')
        return true;

    else
        return false;
}

// reading an integer from the user with input validation
short readNum(string message = "Please enter a number: ")
{
    while (true)
    {
        cout << message;
        short n;
        cin >> n;

        if (!cin.fail() && n >= 0)
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
    bool markedForDelete = false;
};

bool operator==(const sClient &a, const sClient &b)
{
    return a.accountNumber == b.accountNumber;
}

void addClientsToFile(string fileName, string delim, vector<sClient> &vClients);

// ------------------------------------------------------ MAIN MENU ------------------------------------------------------
// ********************************************************************************************************************************

enum enMainMenuOption
{
    ShowClients = 1,
    AddClient,
    DeleteClient,
    UpdateClient,
    FindClient,
    Exit,
};

// function forward declaration (the main game functoin)
void handleProgram(enMainMenuOption enUserChoice, vector<sClient> &vClients);

void showMainMenuOptions()
{
    cout << "\n========== Bank Client Manager ==========\n";
    cout << "1. Show Clients\n";
    cout << "2. Add New Client\n";
    cout << "3. Delete Client\n";
    cout << "4. Update Client\n";
    cout << "5. Find Client\n";
    cout << "6. Exit\n";
    cout << "=========================================\n";
}

enMainMenuOption getMainMenuUserChoice()
{
    short choice;

    short firstOptionNum = static_cast<short>(enMainMenuOption::ShowClients);
    short lastOptionNum = static_cast<short>(enMainMenuOption::Exit);

    while (true)
    {
        choice = readNum("Choose an option : ");

        if (choice >= firstOptionNum && choice <= lastOptionNum)
        {
            break;
        }

        cout << "Invalid choice. Please enter a number between " << firstOptionNum << " and " << lastOptionNum << endl;
    }
    enMainMenuOption enChoice = static_cast<enMainMenuOption>(choice);
    return enChoice;
}

enMainMenuOption showMainScreenAndGetUserOption()
{
    clearScreen();
    showMainMenuOptions();

    enMainMenuOption userChoice = getMainMenuUserChoice();
    return userChoice;
}

void goBackToMainMenu(vector<sClient> &vClients)
{

    cout << "\nPress Enter to return to the main menu...";
    cin.get(); // Pause}
    handleProgram(showMainScreenAndGetUserOption(), vClients);
}

// ********************************************************************************************************************************

// ------------------------------------------------------ INPUT VALIDATION ------------------------------------------------------
// ********************************************************************************************************************************

bool isAllStringDigit(const string &str)
{
    return (!str.empty() && all_of(str.begin(), str.end(), [](const char &c)
                                   { return isdigit(c); }));
}

bool isValidDouble(const string &s)
{
    bool decimalFound = false;

    if (s.empty())
        return false;

    for (char c : s)
    {
        if (c == '.')
        {
            if (decimalFound)
                return false; // second decimal point
            decimalFound = true;
        }
        else if (!isdigit(c))
            return false; // not a digit or '.'
    }
    return true;
}

// ------------- Account Number -------------
// ------------- ------------- -------------

// ensure that the entered account number is not already exists
bool isAccountNumberExist(string accountNumber, vector<sClient> &vClients)
{

    for (sClient &client : vClients)
    {
        if (accountNumber == client.accountNumber)
            return true;
    }

    return false;
}

bool isValidAccountNumber(const string accountNum, vector<sClient> &vClients)
{
    if (accountNum.empty())
    {
        cout << "Account number cannot be empty.\n";
        return false;
    }
    if (isAccountNumberExist(accountNum, vClients))
    {
        cout << "Account number already exists. Please choose another.\n";
        return false;
    }
    return true;
}

string readUniqueAccountNumber(vector<sClient> &vClients)
{
    string accountNum;
    // Read the account number from the user and ensure that it does not already exist.
    do
    {
        cout << "Account Number : ";
        getline(cin, accountNum);

    } while (!isValidAccountNumber(accountNum, vClients));

    return accountNum;
}
// ------------- ------------- -------------

// ------------- Phone Number -------------
// ------------- ------------- -------------
bool isValidPhoneNumber(const string &phoneNum)
{
    if (phoneNum.empty())
    {
        cout << "Phone Number cannot be empty.\n";
        return false;
    }
    if (!isAllStringDigit(phoneNum))
    {
        cout << "Phone Number should contain only digits.\n";
        return false;
    }
    if (!(phoneNum[0] == '0' && phoneNum[1] == '1'))
    {
        cout << "Phone number should start with : 01\n";
        return false;
    }
    if (phoneNum.length() != 11)
    {
        cout << "Phone number most be 12 digits.\n";
        return false;
    }

    return true;
}

string readPhoneNumber()
{
    string phoneNum;
    do
    {
        cout << "Phone Number   : ";
        getline(cin, phoneNum);

    } while (!isValidPhoneNumber(phoneNum));

    return phoneNum;
}
// ------------- ------------- -------------

// ------------- Pin Number -------------
// ------------- ------------- -------------
bool isPinCodeValid(const string &pinCode)
{
    if (pinCode.empty())
    {
        cout << "Phone Number cannot be empty.\n";
        return false;
    }
    if (!isAllStringDigit(pinCode))
    {
        cout << "Pin Number should contain only digits.\n";
        return false;
    }
    if (pinCode.length() != 4)
    {
        cout << "Pin Number must be only 4 digits.\n";
        return false;
    }

    return true;
}

string readPinCode()
{
    string pinCode;

    do
    {
        cout << "PIN Code       : ";
        getline(cin, pinCode);

    } while (!isPinCodeValid(pinCode));

    return pinCode;
}
// ------------- ------------- -------------

// ------------- Balance -------------
// ------------- ------------- -------------

bool isAccountBalanceValid(const string &accountBalance)
{

    if (accountBalance.empty())
    {
        cout << "Balance cannot be empty.\n";
        return false;
    }
    if (!isValidDouble(accountBalance))
    {
        cout << "Balance must be a valid number (digits and at most one decimal point).\n";
        return false;
    }

    double balanceVal = stod(accountBalance);
    if (balanceVal < 0)
    {
        cout << "Balance cannot be negative.\n";
        return false;
    }

    return true;
}

double readAccountBalance()
{
    string accountBalance;
    do
    {
        cout << "Account Balance: ";
        getline(cin, accountBalance);

    } while (!isAccountBalanceValid(accountBalance));

    return stod(accountBalance);
}
// ------------- ------------- -------------
// ********************************************************************************************************************************

// ------------------------------------------------------ DISPLAYING CLIENTS ------------------------------------------------------
// ********************************************************************************************************************************

// Prints all fields of a single client in a formatted layout
void displayClientCard(const sClient client)
{
    cout << "---------------------------------------------\n";
    cout << "Account Number : " << client.accountNumber << "\n";
    cout << "Pin Code       : " << client.pinCode << "\n";
    cout << "Full Name      : " << client.fullName << "\n";
    cout << "Phone          : " << client.phone << "\n";
    // Set output format: fixed point, 3 decimal places
    cout << fixed << setprecision(3);
    cout << "Balance        : " << client.accountBalance << "\n";
    cout << "---------------------------------------------\n";
}

void displayClientRecord(const sClient client, int n)
{
    cout << "| " << setw(5) << left << n;
    cout << "| " << setw(15) << left << client.accountNumber;
    cout << "| " << setw(10) << left << client.pinCode;
    cout << "| " << setw(40) << left << client.fullName;
    cout << "| " << setw(12) << left << client.phone;

    cout << "| " << setw(12) << left << fixed << setprecision(3) << client.accountBalance;
}

void printHorizontalTableBorder()
{
    cout << "\n_______________________________________________________";
    cout << "__________________________________________________\n\n";
}

void printTableHeader()
{
    printHorizontalTableBorder();
    cout << "| " << left << setw(5) << "Num";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    printHorizontalTableBorder();
}

// Iterates through a list of clients and prints each one using displayClientCard
void displayClientsStructFromVector(vector<sClient> &vClients)
{

    int n = 1;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";

    printTableHeader();

    // print clients inside the table
    for (sClient &client : vClients)
    {

        displayClientRecord(client, n);
        if (n < vClients.size())
            cout << endl;
        n++;
    }
    printHorizontalTableBorder();
}
// ********************************************************************************************************************************

// Reads client details from user input to construct a complete sClient record
// returns a Client object
sClient readClientInfoFromUser(short n, vector<sClient> &vClients)
{
    sClient client;
    cout << "\nEntering details for Client [" << n << "]\n";

    // Read the account number from the user and ensure that it does not already exist.
    client.accountNumber = readUniqueAccountNumber(vClients);

    client.pinCode = readPinCode();

    cout << "Full Name      : ";
    getline(cin, client.fullName);

    client.phone = readPhoneNumber();

    client.accountBalance = readAccountBalance();

    return client;
}

// returns a Client object
sClient ChangeClientInfoFromUser(string accountNumber)

{
    sClient client;
    cout << "Updating client details for client:[ " << accountNumber << " ]\n";

    // Read the account number from the user and ensure that it does not already exist.
    client.accountNumber = accountNumber;

    client.pinCode = readPinCode();

    cout << "Full Name      : ";
    getline(cin, client.fullName);

    client.phone = readPhoneNumber();

    client.accountBalance = readAccountBalance();

    return client;
}

// DISPLAYING CLIENTS AS LINES NOT CARDS (POTENTIAL DELETION)
// ********************************************************************************************

// Converts a client struct into a delimited string representation for output or storage
string formatClientAsLine(sClient client, string delim)
{
    return client.accountNumber + delim +
           client.pinCode + delim +
           client.fullName + delim +
           client.phone + delim +
           to_string(client.accountBalance);
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

// ********************************************************************************************

// ------------------------------------------------------ ADDING NEW CLIENTS  ------------------------------------------------------
// *****************************************************************************************************************

//  multiple clients from user input and stores them in the vClients vector
void inputMultipleClients(int numOfClients, vector<sClient> &vClients)
{
    for (short n = 1; n <= numOfClients; ++n)
    {
        // Prompt user to input details for client n
        sClient client = readClientInfoFromUser(n, vClients);
        vClients.push_back(client);
    }

    // Display all collected client records in formatted structure
    displayClientsStructFromVector(vClients);
}

sClient addIndividualClient(vector<sClient> &vAllClients, vector<sClient> &vNewClients, int n)
{
    // Prompt user to input details for client n
    sClient client = readClientInfoFromUser(n, vAllClients);
    vNewClients.push_back(client);
    // add this client to the big clients vector
    vAllClients.push_back(client);
    return client;
}

// Collects client data through separate prompts, stores them,
// then prints all records in a single delimited-line format
void AddNewClient(string fileName, string delim, vector<sClient> &vAllClients)
{

    vector<sClient> vNewClients;
    int n = 0;
    do
    {
        sClient client = addIndividualClient(vAllClients, vNewClients, n + 1);
        n++;

    } while (isSure("Do you want to add a new client?:(y/n): "));

    addClientsToFile(fileName, delim, vNewClients);
    cout << "Client" << (n == 1 ? "" : "s") << " " << (n == 1 ? "has" : "have") << " been added successfully. " << endl;
    cout << "\n\t\t\t\t\t----[ADDED CLIENTS]----\n";
    displayClientsStructFromVector(vNewClients);
}

// *****************************************************************************************************************

// Appends a list of client records to a text file in a delimited format.
// If the file doesn't exist, it will be created automatically.
void addClientsToFile(string fileName, string delim, vector<sClient> &vClients)
{

    // Open file for output in append mode to avoid overwriting existing records
    ofstream myFile(fileName, ios::out | ios::app);

    if (!myFile.is_open())
    {
        cerr << "Error: Could not open file '" << fileName << "' for writing.\n";
        return;
    }

    // Write each client's data to the file, one line per client
    for (const sClient &client : vClients)
    {
        // Each line is a delimited string created from the client's fields
        if (client.markedForDelete == false)
            myFile << formatClientAsLine(client, delim) << endl;
    }

    myFile.close();
}

void readClientsFromFile(string fileName, string delim, vector<sClient> &vClients)
{
    vClients.clear();
    ifstream myFile(fileName);

    string line;

    if (myFile.is_open())
    {

        while (getline(myFile, line))
        {
            vector<string> vClientString;
            splitString(line, vClientString, delim);
            sClient client = parseClientRecord(vClientString);
            vClients.push_back(client);
        }
    }

    else
    {
        cout << "Error: Could not open file '" << fileName << "' for writing.\n";
        return;
    }
}

// Searches the file for a client by account number and returns it through 'foundClient'.
// Returns true if found, false otherwise.
bool findClientInFileByAccountNum(string fileName, string delim, string accountNumber, vector<sClient> &vClients, sClient &foundClient)
{

    if (vClients.empty())
    {
        // Load all clients from the file into the vector
        readClientsFromFile(fileName, delim, vClients);
    };

    // Scan for a client with a matching account number
    for (const sClient &client : vClients)
    {
        if (client.accountNumber == accountNumber)
        {
            foundClient = client; // Output the found client
            return true;          // Match found
        }
    }

    // No match found after checking all clients
    return false;
}

bool markClientAsDeletedByAccNum(string accountNumber, vector<sClient> &vClients)
{
    for (sClient &client : vClients)
    {
        if (client.accountNumber == accountNumber)
        {
            client.markedForDelete = true;
            return true;
        }
    }
    return false;
}

void removeClientFromFileByAccNum(string fileName, string delim, string accountNumber, vector<sClient> &vClients)
{
    sClient client;
    if (findClientInFileByAccountNum(fileName, delim, accountNumber, vClients, client))
    {

        cout << "\n- Client Details:\n";
        displayClientCard(client);

        if (isSure("Are you sure you want to delete this client? (y/n) : "))
        {

            if (markClientAsDeletedByAccNum(accountNumber, vClients))
            {
                cout << "Client with account number: [" << client.accountNumber << "] has been deleted successfully!\n";

                // Clear the file first
                ofstream clearFile(fileName, ios::out); // Truncates file
                clearFile.close();

                // rewrite the file again after deleting client from the vector
                addClientsToFile(fileName, delim, vClients);
            }
            else
                cout << "Client [" << accountNumber << "] not found!\n";
        }
    }

    else
        cout << "No client found with account number: " << accountNumber << "\n";
}

// removing a client from the vector without affecting the file .
void removeClientFromVector(vector<sClient> &vClients, string accountNumber)
{
    for (sClient &client : vClients)
    {
        if (client.accountNumber == accountNumber)
        {
            auto it = remove(vClients.begin(), vClients.end(), client);
            vClients.erase(it, vClients.end());
        }
    }
}

// Updates a client record in the file by account number
void updateClientInFileByAccountNumber(string accountNumber, string fileName, string delim, vector<sClient> &vClients)
{
    // Step 1: Search for the client in the file and load all clients into the vector
    sClient client;
    if (findClientInFileByAccountNum(fileName, delim, accountNumber, vClients, client))
    {
        cout << "\n- Client Details:\n";
        displayClientCard(client);

        // Confirm with the user before applying changes
        if (!isSure("Are you sure you want to update this client? (y/n): "))
            return;

        // Step 2: Remove the old record from the vector
        removeClientFromVector(vClients, accountNumber);

        // Step 3: Prompt for and collect updated client information
        sClient updatedClient = ChangeClientInfoFromUser(accountNumber);
        vClients.push_back(updatedClient);

        // Step 4: Clear the file to prepare for rewriting updated data
        ofstream clearFile(fileName, ios::out); // Truncates the file
        clearFile.close();

        // Step 5: Rewrite the entire updated list of clients to the file
        addClientsToFile(fileName, delim, vClients);

        cout << "Client updated successfully.\n";
    }
    else
    {
        cout << "No client found with account number: " << accountNumber << "\n";
    }
}

// -------------------------------------------------- DISPLAYING SCREEN FOR EACH OPTION ------------------------------------------------------
// ********************************************************************************************************************************
void showClientsRecordScreen(vector<sClient> &vClients, string fileName, string delim)
{

    cout << "\n\t\t\t\t==========================================\n";
    cout << "\t\t\t\t === Bank Client Manager: ALL CLIENTS ===\n";
    cout << "\t\t\t\t==========================================\n";
}

void showAddClientScreen()

{
    cout << "\n\t\t\t\t==========================================\n";
    cout << "\t\t\t\t === Bank Client Manager: ADDING CLIENT ===\n";
    cout << "\t\t\t\t==========================================\n";
}

void showDeleteClientScreen()
{

    cout << "\n\t\t\t\t==========================================\n";
    cout << "\t\t\t\t === Bank Client Manager: DELETE CLIENT ===\n";
    cout << "\t\t\t\t==========================================\n";
}

void showUpdateClientScreen()
{
    cout << "\n\t\t\t\t==========================================\n";
    cout << "\t\t\t\t === Bank Client Manager: UPDATE CLIENT ===\n";
    cout << "\t\t\t\t==========================================\n";
}

void showFindClientScreen()
{
    cout << "\n\t\t\t\t==========================================\n";
    cout << "\t\t\t\t === Bank Client Manager: FIND CLIENT ===\n";
    cout << "\t\t\t\t==========================================\n\n";
}

// ********************************************************************************************************************************

void showEndScreen()
{
    cout << "\n___________________________\n\n";
    cout << "Program Ends..\n";
    cout << "___________________________\n";
}
void handleProgram(enMainMenuOption enUserChoice, vector<sClient> &vClients)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (enUserChoice)
    {

    case ShowClients:
        clearScreen();
        showAddClientScreen();
        readClientsFromFile(fileName, delim, vClients);
        displayClientsStructFromVector(vClients);
        goBackToMainMenu(vClients);
        break;

    case AddClient:
        clearScreen();
        showAddClientScreen();
        // adding existing clients to vClients for checking account Number
        readClientsFromFile(fileName, delim, vClients);
        AddNewClient(fileName, delim, vClients);
        goBackToMainMenu(vClients);
        break;

    case DeleteClient:
    {
        clearScreen();

        showDeleteClientScreen();
        string accountNumber = readString("Please enter account number: ");
        removeClientFromFileByAccNum(fileName, delim, accountNumber, vClients);
        goBackToMainMenu(vClients);
        break;
    }

    case UpdateClient:
    {
        clearScreen();
        showUpdateClientScreen();

        string accountNumber = readString("Please enter account number: ");
        updateClientInFileByAccountNumber(accountNumber, fileName, delim, vClients);
        goBackToMainMenu(vClients);
        break;
    }

    case FindClient:
    {
        clearScreen();
        showFindClientScreen();
        string accountNumber = readString("Please enter account number: ");
        sClient client;
        if (findClientInFileByAccountNum(fileName, delim, accountNumber, vClients, client))
        {

            cout << "• Client Details: \n";
            displayClientCard(client);
        }
        else
        {
            cout << "No client found with account number: " << accountNumber << "\n";
        }
        goBackToMainMenu(vClients);
        break;
    }

    case Exit:
        clearScreen();
        showEndScreen();
        break;

    default:
        cout << "Ivalid Choice !\n ";
    }
}

int main()
{

    vector<sClient> vClients;

    handleProgram(showMainScreenAndGetUserOption(), vClients);
    clearScreen();

    return 0;
}