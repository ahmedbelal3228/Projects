#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Removes leading and trailing spaces from a string
string trimString(string s)
{
    // Find index of first non-space character
    size_t firstNonSpace = s.find_first_not_of(' ');
    if (firstNonSpace == string::npos)
        return ""; // If the string is all spaces, return an empty string

    // Find index of last non-space character
    size_t lastNonSpace = s.find_last_not_of(' ');

    // Return substring without leading/trailing spaces
    return s.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
}

// Reads a full line string from standard input and trims it automatically
string readString(string message = "Please enter a string: ")
{
    cout << message;
    string s;
    getline(cin, s);
    return trimString(s); // Automatically remove leading/trailing spaces
}

// Replaces all occurrences of 'targetWord' in 'inputString' with 'replacementWord'
string replaceString(string inputString, string targetWord, string replacementWord)
{
    // Prevent infinite loop if targetWord is empty
    if (targetWord.empty())
    {
        cout << "Error: Target word cannot be empty." << endl;
        return inputString;
    }

    string s2 = inputString; // Create a copy to preserve the original input
    size_t pos = 0;          // Start search from the beginning

    // Loop to find and replace all occurrences
    while ((pos = s2.find(targetWord, pos)) != string::npos)
    {
        s2.replace(pos, targetWord.length(), replacementWord);
        pos += replacementWord.length(); // Move past the replaced word to continue searching
    }

    if (inputString == s2)
        cout << "Target word not found. No replacements made." << endl;

    return s2;
}

// Prints the input, replacement action, and output in a clean structured format
void printReplacementResult(string inputString, string targetWord, string replacementWord, string outputString)
{
    cout << "- - - - - - - - - - - - - \n";
    // Construct clear replacement statement for display
    string replacingStatement = "[" + targetWord + "] with [" + replacementWord + "]...";
    if (targetWord.empty())
    {
        replacingStatement = "Nothing"; // Handle empty target display gracefully
    }

    cout << "Input String: " << inputString << "\n";
    cout << "Replacing: " << replacingStatement << "\n";
    cout << "Output String: " << outputString << endl;
}

// Entry point of the program
int main()
{
    // Read and trim the input string and replacement terms from the user
    string inputString = readString();
    string targetWord = readString("Enter the word you want to replace: ");
    string replacementWord = readString("Enter the word you want to replace with: ");

    // Replace targetWord with replacementWord in the input string
    string outputString = replaceString(inputString, targetWord, replacementWord);

    // Print the result clearly
    printReplacementResult(inputString, targetWord, replacementWord, outputString);

    return 0;
}
