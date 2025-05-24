/*
Project: Rock, Paper, Scissors Game 

Description:
A console-based implementation of the classic Rock-Paper-Scissors game with extended features.
The user plays against the computer for a custom number of rounds, with results tracked and statistics displayed after each session.

Features:
- Choose number of rounds
- Select moves: Stone 🪨, Paper 📄, or Scissors ❌
- Computer opponent with randomized choices
- Per-round results and final game outcome
- Tracks cumulative statistics across multiple sessions:
  • Total games played
  • Number of wins, losses, draws
  • Per-round outcomes and overall winner
- Option to replay or view overall statistics

Technologies & Concepts Used:
- Enums for choice representation
- Struct for game statistics
- Input validation and error handling
- Random number generation (`rand()`)
- Cross-platform screen clearing
- Loops, branching, recursion, and user interaction

Goal:
Practice C++ fundamentals like structured logic, user input, condition handling, and game state tracking in a fun and engaging way.
*/

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <limits> // For handling input validation
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls"); // Windows command to clear the screen
#else
    system("clear"); // Linux & macOS command to clear the screen
#endif
}
// Enum representing the choices for the game
enum enChoices
{
    Stone = 1,
    Paper,
    Scissors
};

// Displays the menu and ensures the user enters a valid choice
enChoices displayOption()
{
    int userChoice;
    do
    {
        cout << "\nSelect your move:\n";
        cout << "1-Stone 🪨 | 2-Paper 📄 | 3-Scissors ❌\n";
        cout << "Enter your choice (1-3): ";
        cin >> userChoice;

        if (userChoice < 1 || userChoice > 3 || cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 3.\n";
        }

    } while (userChoice < 1 || userChoice > 3); // Repeat until a valid choice is entered

    return static_cast<enChoices>(userChoice);
}

// Tracks cumulative statistics across multiple games
struct GameStats
{
    int totalGames = 0;
    int totalUserWinningGames = 0;
    int totalComputerWinningGames = 0;
    int totalDrawGames = 0;
    int totalRounds = 0;
    int totalUserWins = 0;
    int totalComputerWins = 0;
    int totalDraws = 0;
};

void updateOverallStats(GameStats &overAllStats, int userWins, int computerWins, int draws)
{
    // Update overall states
    overAllStats.totalUserWins += userWins;
    overAllStats.totalComputerWins += computerWins;
    overAllStats.totalDraws += draws;
}

void printOverallStats(const GameStats &overAllStats)
{
    cout << "\n📊 Overall Game Statistics:\n";
    cout << "--------------------------------------\n";
    cout << "Total Games Played:       " << overAllStats.totalGames << endl;

    cout << "User Winning Games:       " << overAllStats.totalUserWinningGames << endl;
    cout << "Computer Winning Games:   " << overAllStats.totalComputerWinningGames << endl;
    cout << "Draw Games:               " << overAllStats.totalDrawGames << endl;
    cout << "\n";
    cout << "Total Rounds Played:   " << overAllStats.totalRounds << endl;
    cout << "Total User Wins:       " << overAllStats.totalUserWins << endl;
    cout << "Total Computer Wins:   " << overAllStats.totalComputerWins << endl;
    cout << "Total Draws:           " << overAllStats.totalDraws << endl;
    cout << "--------------------------------------\n";

    // Determine the overall winner
    if (overAllStats.totalUserWins > overAllStats.totalComputerWins)
    {
        cout << "🏆 Overall Winner: **YOU**! 🎉\n";
    }
    else if (overAllStats.totalComputerWins > overAllStats.totalUserWins)
    {
        cout << "🤖 Overall Winner: **Computer**! Better luck next time!\n";
    }
    else
    {
        cout << "🤝 It's an overall tie! Well played!\n";
    }

    cout << "--------------------------------------\n";
}

// Generates a random number within a given range
int RandomNumber(int start, int end)
{
    return (rand() % (end - start + 1)) + start;
}

// Prompts the user for the number of rounds and ensures valid input
int getNumOfRound()
{
    int numOfRound;
    do
    {
        cout << "\nEnter the number of rounds you want to play: ";
        cin >> numOfRound;

        if (cin.fail() || numOfRound <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive number.\n";
        }
        else
        {
            break;
        }
    } while (true);

    return numOfRound;
}

// Generates a random choice for the computer
enChoices getComputerChoice()
{
    return static_cast<enChoices>(RandomNumber(1, 3));
}

// Gets and returns the user's choice using the displayOption function
enChoices getUserChoice()
{
    return displayOption();
}

// Determines the result of the round (1 = User Wins, -1 = Computer Wins, 0 = Draw)
int checkRoundResult(enChoices userChoice, enChoices computerChoice)
{
    if (userChoice == computerChoice)
        return 0; // It's a draw

    // Winning conditions for the user
    if ((userChoice == Stone && computerChoice == Scissors) ||
        (userChoice == Paper && computerChoice == Stone) ||
        (userChoice == Scissors && computerChoice == Paper))
    {
        return 1; // User wins
    }

    return -1; // Computer wins
}

// Displays the overall winner of the game based on total wins
void gameWinner(int userWins, int computerWins, GameStats &overAllStats)
{
    cout << "\n ----Final Game Outcome----\n";
    if (userWins > computerWins)
    {
        cout << "🏆 Congratulations! You won the game!\n";
        overAllStats.totalUserWinningGames++;
    }
    else if (computerWins > userWins)
    {
        cout << "🤖 The computer wins this time. Better luck next time!\n";
        overAllStats.totalComputerWinningGames++;
    }
    else
    {
        cout << "🤝 It's a tie! Well played!\n";
        overAllStats.totalDrawGames++;
    }
}

// Converts an enChoices enum to a human-readable string
string choiceToString(enChoices choice)
{
    switch (choice)
    {
    case Stone:
        return "Stone 🪨";
    case Paper:
        return "Paper 📄";
    case Scissors:
        return "Scissors ❌";
    default:
        return "Unknown Choice";
    }
}

// Displays the final game results
void printResults(int userWins, int computerWins, int draws)
{
    cout << "\n----Game Summary----\n";
    cout << "Total Rounds: " << userWins + computerWins + draws << endl;
    cout << "✅ Your Wins: " << userWins << endl;
    cout << "🤖 Computer Wins: " << computerWins << endl;
    cout << "🤝 Draws: " << draws << endl;
}

// Asks the user if they want to play again and allows them to view stats
bool playAgain(GameStats &overAllStats)
{
    char playAgain;
    while (true)
    {
        cout << "\nWould you like to play again? (y/n): ";
        cout << "or press s to see overall stats\n";
        cin >> playAgain;

        if (playAgain == 'y' || playAgain == 'Y')
        {
            return true;
        }
        else if (playAgain == 'n' || playAgain == 'N')
        {
            return false;
        }
        else if (playAgain == 's' || playAgain == 'S')
        {
            printOverallStats(overAllStats); // Show stats without exiting
        }
        else
        {
            cout << "Invalid input. Please enter 'y' for Yes or 'n' for No.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Main game loop that runs for the specified number of rounds and updates stats after each session
void gamePlay(int numOfRounds, GameStats &overAllStats)
{
    int userWins = 0, computerWins = 0, draws = 0;

    // Increment the total number of games each time `gamePlay()` is called.
    overAllStats.totalGames++;
    // Add the number of rounds the user entered to the overall round count.
    overAllStats.totalRounds += numOfRounds;
    // this_thread::sleep_for(chrono::milliseconds(1000));
    clearScreen();
    for (int i = 1; i <= numOfRounds; i++)
    {

        cout << "\n🔹 Round " << i << " of " << numOfRounds << " 🔹\n";

        enChoices userChoice = getUserChoice();
        enChoices computerChoice = getComputerChoice();
        int result = checkRoundResult(userChoice, computerChoice);

        // Simulate the computer "thinking" for a moment
        cout << "Computer is making its move... 🤔" << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));

        // Display both choices
        cout << choiceToString(userChoice) << "  VS  " << choiceToString(computerChoice) << endl;

        // Determine the round outcome
        if (result == 1)
        {
            cout << "✅ You won this round!\n";
            userWins++;
        }
        else if (result == -1)
        {
            cout << "❌ Computer wins this round!\n";
            computerWins++;
        }
        else
        {
            cout << "🤝 This round is a draw!\n";
            draws++;
        }
    }
    // adding the result of the rounds to the overall stats
    updateOverallStats(overAllStats, userWins, computerWins, draws);

    // Display final results and overall winner
    printResults(userWins, computerWins, draws);
    gameWinner(userWins, computerWins, overAllStats);

    // Ask if the user wants to play again
    if (playAgain(overAllStats))
    {
        int newNumOfRounds = getNumOfRound();
        gamePlay(newNumOfRounds, overAllStats);
    }
    else
    {
        cout << "\nThanks for playing! See you next time! 👋\n";
        printOverallStats(overAllStats); // Show final stats before quitting
    }
}

int main()
{
    srand((unsigned)time(NULL)); // Seed the random number generator for randomness
    GameStats overAllStats;
    clearScreen();
    int numOfRounds = getNumOfRound();
    gamePlay(numOfRounds, overAllStats);
}
