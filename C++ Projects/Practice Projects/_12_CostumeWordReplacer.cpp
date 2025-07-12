#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Removes leading and trailing spaces from a string
string trimString(string s)
{
    size_t firstNonSpace = s.find_first_not_of(' ');
    if (firstNonSpace == string::npos)
        return ""; // Return empty string if all spaces

    size_t lastNonSpace = s.find_last_not_of(' ');
    // Return the substring without leading/trailing spaces
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

string tolowerString(string s)
{

    for (char &c : s)
        c = tolower(c);

    return s;
}

/*
 * This function splits the input string `s` into substrings separated by the specified `delimiter`.
 * Each non-empty substring is added to the vector `vWords`.
 */
void splitStringToWords(string s, vector<string> &vWords, string delimiter)
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

void replaceWordInVector(vector<string> &vWords, string targetWord, string replacementWord, bool matchCase = false)
{

    for (string &word : vWords)
    {
        if (matchCase) // Case-sensitive
        {
            if (word == targetWord)
                word = replacementWord;
        }

        else // Case-insensitive (default)
        {
            if (tolowerString(word) == tolowerString(targetWord))
                word = replacementWord;
        }
    }
}

void printStringVector(vector<string> &v)
{
    vector<string>::iterator first = v.begin();
    vector<string>::iterator last = v.end();

    for (auto it = first; it != last; ++it)
    {
        cout << *it;
        if (next(it) != v.end())
            cout << " ";
    }
    cout << endl;
}

void replaceWordInString(string inputString, string targetWord, string replacementWord, bool matchCase = false)

{
    vector<string> vWords;
    splitStringToWords(inputString, vWords, " ");
    replaceWordInVector(vWords, targetWord, replacementWord, matchCase);
    printStringVector(vWords);
}

int main()
{

    // Read and trim the input string and replacement terms from the user
    string inputString = readString();
    string targetWord = readString("Enter the word you want to replace: ");
    string replacementWord = readString("Enter the word you want to replace with: ");

    replaceWordInString(inputString, targetWord, replacementWord);

    return 0;
}