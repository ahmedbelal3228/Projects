/*
Project: Math Quiz Game 

Description:
This console-based interactive game helps users practice arithmetic by solving timed math questions.
The player chooses:
- The number of questions
- A difficulty level (Easy, Medium, Hard, or Mixed)
- A question type (Addition, Subtraction, Multiplication, Division, or Mixed)

Features:
- Generates random math problems based on chosen difficulty and type
- Mix modes for randomizing difficulties and question types
- Real-time answer checking with detailed feedback
- Tracks player performance (correct/wrong answers, accuracy, and progress by difficulty)
- Clear UI and automatic screen clearing between rounds
- Replay option to keep practicing

Technologies & Concepts:
- C++ OOP principles using `struct` and `enum`
- Random number generation
- Time-based input handling and timers
- User input validation
- Multithreading (planned for time-limited input)
- Cross-platform terminal clearing

Goal:
Enhance C++ problem-solving skills by building a real-time math quiz system with structured design, enums, functions, and interactive user experience.
*/


#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <iomanip>
using namespace std;
using namespace std::chrono;

// Clears the console screen to improve readability between questions.
// Uses system-specific commands for Windows and Linux/macOS.
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // Windows command to clear the screen
#else
    system("clear"); // Linux & macOS command to clear the screen
#endif
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -   User Input Handling  - - - - - - - - - - - - - - - - - - - - -

// Function to generate a random number within a specified range
int RandomNumber(int start, int end)
{
    /*
        rand() % (end - start + 1) generates a number between 0 and (end - start)
        Adding 'start' shifts the range to [start, end].
    */
    int RandInt = (rand() % (end - start + 1)) + start;
    return RandInt; // Convert and return as a character
}

// Reads a positive integer from the user and ensures valid input.
int readPositiveNum(string message)
{
    int Num;

    while (true) // Keep asking until valid input is provided
    {
        cout << message;
        cin >> Num;

        // Check if input is not a valid number (non-integer, characters, etc.)
        if (cin.fail())
        {
            cin.clear();                                         // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "❌ Invalid input! Please enter a valid non-negative number.\n";
            continue; // Ask again
        }

        // Ensure the number is non-negative
        if (Num < 0)
        {
            cout << "⚠ Please enter a non-negative number.\n";
            continue; // Ask again
        }

        return Num; // Valid input, return the number
    }
}

// Prompts the user to enter the number of questions they want to answer.
int readNumOfQuestions()
{
    while (true) // Keep asking until a valid number is entered
    {
        int numOfQuestions = readPositiveNum("🔢Enter the number of questions you want to solve: ");

        // Ensure at least 1 question is selected
        if (numOfQuestions == 0)
        {
            cout << "❌ Invalid input! Number of questions should be at least one.\n";
            continue; // Ask again
        }

        return numOfQuestions; // Valid input, return the value
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// - - - - - - - - - - - - - - - - - - - - - - LEVEL OF DIFFICULTY - - - - - - - - - - - - - - - - - - - - -
// Enum defining difficulty levels for the game.
// 'mixDifficulty' allows each question to have a randomly selected difficulty.
enum enLevelOfDifficulty
{
    easy = 1,
    med,
    hard,
    mixDifficulty
};

// Prompts the user to select a difficulty level and ensures valid input.
// Prevents invalid choices and restricts selection to valid difficulty levels.
enLevelOfDifficulty chooseDifficulty()
{
    int levelChoice;

    while (true) // Keep prompting until valid input is provided
    {
        cout << "\nChoose the level of difficulty...\n";
        cout << "1 - Easy | 2 - Med | 3 - Hard | 4 - Mix" << endl;
        cout << "Enter your choice: ";
        cin >> levelChoice;

        // Validate input: ensure it's a number between 1 and 4
        if (cin.fail() || levelChoice < 1 || levelChoice > 4)
        {
            cin.clear();             // Clear input error state
            cin.ignore(10000, '\n'); // Discard invalid input
            cout << "❌ Invalid choice! Please enter a number between 1 and 4.\n";
            continue; // Ask again
        }

        return static_cast<enLevelOfDifficulty>(levelChoice); // Convert to enum and return
    }
}
// Generates a random difficulty level when 'Mix' mode is chosen.
// Ensures each question in Mix Mode has a randomly assigned difficulty.
enLevelOfDifficulty mixLevelOfDifficulty()
{
    int level = RandomNumber(1, 3);                 // Generates a number corresponding to a difficulty level (1 = Easy, 2 = Med, 3 = Hard)
    return static_cast<enLevelOfDifficulty>(level); // Convert the number to its respective enum value
}

// Converts a randomly selected difficulty level into a string representation.
// This is used for displaying the difficulty level when 'Mix' mode is active.
string convertMixLevelToString()
{
    enLevelOfDifficulty level = mixLevelOfDifficulty(); // Get a random difficulty level

    switch (level)
    {
    case easy:
        return "Easy";
    case med:
        return "Med";
    case hard:
        return "Hard";
    default:
        return "default: Med"; // Default fallback (should never happen in normal cases)
    }
}

// Determines the actual difficulty level based on user selection.
// If 'Mix' mode is selected, a new random difficulty is assigned for each question.
enLevelOfDifficulty DifficultyResult(enLevelOfDifficulty levelChoice)
{
    switch (levelChoice)
    {
    case easy:
        return easy;
    case med:
        return med;
    case hard:
        return hard;
    case mixDifficulty:
        return mixLevelOfDifficulty(); // Assign a new random difficulty each time
    default:
        return mixLevelOfDifficulty(); // Fallback case (acts like Mix mode)
    }
}

// Converts a difficulty level enum into a string for display.
// This ensures clear UI feedback regarding the difficulty of each question.
string DifficultyResultToString(enLevelOfDifficulty levelChoice)
{
    switch (levelChoice)
    {
    case easy:
        return "Easy";
    case med:
        return "Med";
    case hard:
        return "Hard";
    case mixDifficulty:
        return convertMixLevelToString(); // Retrieve a random difficulty string for Mix mode
    default:
        return convertMixLevelToString(); // Default fallback (should not normally happen)
    }
}

// Displays the selected difficulty level to the user before starting the game.
// Provides feedback to confirm the user's choice.
void printDifficultyResult(enLevelOfDifficulty levelChoice)
{
    cout << "----------------------\n";
    cout << "Difficulty Level: " << DifficultyResultToString(levelChoice) << endl;
    cout << "----------------------\n";
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -   QUESTION TYPES  - - - - - - - - - - - - - - - - - - - - -
enum enQuestionsTypes
{
    addition = 1,
    subtraction,
    multiplication,
    division,
    mixQuestions
};

enQuestionsTypes chooseQType()
{
    int QType;
    while (true)
    {
        cout << "\nChoose the type of questions...\n";
        cout << "1-Addition | 2-Subtraction | 3-Multiplication | 4-Division | 5-Mix" << endl;
        cout << "Enter you choice: ";
        cin >> QType;
        // If input is invalid (not an integer) or out of range
        if (cin.fail() || QType < 1 || QType > 5)
        {
            cin.clear();             // Clear error state
            cin.ignore(10000, '\n'); // Discard invalid input
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }
        return static_cast<enQuestionsTypes>(QType);
    }
}

// Generates a random question type when 'Mix' mode is chosen.
// Ensures that each question in Mix Mode has a randomly assigned type.
enQuestionsTypes mixQTypes()
{
    int QType = RandomNumber(1, 4);              // Generates a number corresponding to a question type (1 = Addition, 2 = Subtraction, etc.)
    return static_cast<enQuestionsTypes>(QType); // Convert the number to its respective enum value
}

// Converts a randomly selected question type into a string for display.
// Ensures clear UI feedback when 'Mix' mode is selected.
string convertMixQTypesToString()
{
    enQuestionsTypes QType = mixQTypes(); // Get a random question type

    switch (QType)
    {
    case enQuestionsTypes::addition:
        return "Addition";
    case enQuestionsTypes::subtraction:
        return "Subtraction";
    case enQuestionsTypes::multiplication:
        return "Multiplication";
    case enQuestionsTypes::division:
        return "Division";
    default:
        return "default: Addition"; // Default fallback (should not normally happen)
    }
}

// Determines the actual question type based on user selection.
// If 'Mix' mode is selected, assigns a new random question type for each question.
enQuestionsTypes QTypeResult(enQuestionsTypes QType)
{
    switch (QType)
    {
    case enQuestionsTypes::addition:
        return enQuestionsTypes::addition;
    case enQuestionsTypes::subtraction:
        return enQuestionsTypes::subtraction;
    case enQuestionsTypes::multiplication:
        return enQuestionsTypes::multiplication;
    case enQuestionsTypes::division:
        return enQuestionsTypes::division;
    case enQuestionsTypes::mixQuestions:
        return mixQTypes(); // Assign a new random question type each time
    default:
        return mixQTypes(); // Default to Mix mode behavior
    }
}

// Converts a question type enum into a string for user-friendly display.
// Ensures the question type is presented clearly in the game UI.
string QTypeResultToString(enQuestionsTypes QType)
{
    switch (QType)
    {
    case enQuestionsTypes::addition:
        return "Addition";
    case enQuestionsTypes::subtraction:
        return "Subtraction";
    case enQuestionsTypes::multiplication:
        return "Multiplication";
    case enQuestionsTypes::division:
        return "Division";
    case enQuestionsTypes::mixQuestions:
        return convertMixQTypesToString(); // Retrieve a random type for Mix mode
    default:
        return convertMixQTypesToString(); // Default fallback (should not normally happen)
    }
}

// Displays the selected question type to the user before the game starts.
// Helps confirm their selection before proceeding.
void printQTypesResult(enQuestionsTypes QType)
{
    cout << "----------------------\n";
    cout << "Questions Type: " << QTypeResultToString(QType) << endl;
    cout << "----------------------\n";
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -   Game Statistics Tracking   - - - - - - - - - - - - - - - - - - - - - -

// Tracks overall game performance, including correct and incorrect answers.
// Helps calculate the final accuracy percentage and user performance ranking.
struct stGameStats
{
    // Number of correctly & incorrectly answered questions
    int correctAnswers = 0;
    int incorrectAnswers = 0;
    int totalAnswers = 0; // Total number of questions attempted
};

// Tracks the number of questions attempted and correctly solved per difficulty level.
// Provides a breakdown of user performance across Easy, Medium, and Hard difficulties.
struct stDifficultyStats
{
    // Total number of hard questions attempted
    int totalHard = 0;
    // Number of hard questions answered correctly
    int solvedHard = 0;

    int totalMed = 0;
    int solvedMed = 0;
    int totalEasy = 0;
    int solvedEasy = 0;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -  Generating Math Problems by Difficulty   - - - - - - - - - - - - - - - - - - - - -

// Defines the structure of a math question.
// Stores the question text and its correct answer.
struct stQuestion
{
    string questionText; // The formatted question to be displayed (e.g., "5 + 3 = ?")
    int correctAnswer;   // The expected correct answer to compare with user input
};

// Represents the operands and result used in a generated math question.
// Used to store the two numbers and their computed result before forming a question.
struct stOperands
{
    int a;      // First operand (left-hand side of the equation)
    int b;      // Second operand (right-hand side of the equation)
    int result; // Computed result based on the selected operation
};

// Generates operands and computes the result for Hard difficulty questions.
// Ensures appropriate constraints for division and subtraction.
stOperands hardLevel(enQuestionsTypes QType)
{
    int a = RandomNumber(50, 100);
    int b = RandomNumber(50, 100);
    int result = 0;

    if (QTypeResult(QType) == enQuestionsTypes::division)
    {
        // Ensure `a` and `b` form a valid division question:
        // - `b` should not be 0 (avoid division by zero).
        // - `a % b == 0` ensures the result is a whole number.
        // - `a != b` prevents cases like "80 / 80" (too easy).
        // - `a / b <= 5` filters out easy division problems like "100 / 50 = 2",
        //   ensuring that results are larger and more challenging.
        while (b == 0 || a % b != 0 || a / b <= 5)
        {
            a = RandomNumber(50, 100);
            b = RandomNumber(50, 100);
        }
        result = a / b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::subtraction)
    {
        // Ensure subtraction does not result in a negative number.
        if (a < b)
            swap(a, b);
        result = a - b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::addition)
    {
        result = a + b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::multiplication)
    {
        result = a * b;
    }
    return {a, b, result};
}

// Generates operands and computes the result for Medium difficulty questions.
// Applies similar constraints as Hard level but with a reduced number range.
stOperands medLevel(enQuestionsTypes QType)
{
    int a = RandomNumber(10, 50);
    int b = RandomNumber(10, 50);
    int result = 0;

    if (QTypeResult(QType) == enQuestionsTypes::division)
    {
        // Ensure `a` and `b` form a valid division question.
        while (b == 0 || a % b != 0 || a / b <= 5)
        {
            a = RandomNumber(10, 50);
            b = RandomNumber(10, 50);
        }
        result = a / b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::subtraction)
    {
        if (a < b)
            swap(a, b);
        result = a - b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::addition)
    {
        result = a + b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::multiplication)
    {
        result = a * b;
    }
    return {a, b, result};
}

// Generates operands and computes the result for Easy difficulty questions.
// Ensures simple calculations with smaller numbers.
stOperands easyLevel(enQuestionsTypes QType)
{
    int a = RandomNumber(1, 10);
    int b = RandomNumber(1, 10);
    int result = 0;

    if (QTypeResult(QType) == enQuestionsTypes::division)
    {
        // Ensure `a` and `b` form a valid division question.
        while (b == 0 || a % b != 0)
        {
            a = RandomNumber(1, 10);
            b = RandomNumber(1, 10);
        }
        result = a / b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::subtraction)
    {
        if (a < b)
            swap(a, b);
        result = a - b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::addition)
    {
        result = a + b;
    }
    else if (QTypeResult(QType) == enQuestionsTypes::multiplication)
    {
        result = a * b;
    }
    return {a, b, result};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -  Timer   - - - - - - - - - - - - - - - - - - - - -
// This section handles user input timing using a separate thread , Ensures that user input is recorded within a given time limit.

// Global atomic flag to check if input was received.
// Used to track whether the user answered within the allotted time.
atomic<bool> answeredInTime(false);

// Runs in a separate thread to handle user input asynchronously.
// Allows the main thread to continue running the countdown timer.
void getUserAnswer(int &userAnswer)
{
    cin >> userAnswer;
    answeredInTime = true; // Mark input as received
}

// Ensures that the input thread is safely detached if still running.
// Prevents potential memory leaks or undefined behavior.
void stopInputThread(thread &inputThread)
{
    if (inputThread.joinable()) // Check if the thread is still running
    {
        try
        {
            inputThread.detach(); // Ensure the thread is detached safely
        }
        catch (...)
        {
            cerr << "⚠ Error stopping input thread!\n"; // Handle any unexpected errors
        }
    }
}

// Returns the time limit (in seconds) based on the selected difficulty level.
// Harder difficulties have more time compared to easier levels.
int getTimeLimit(enLevelOfDifficulty difficulty)
{
    switch (difficulty)
    {
    case hard:
        return 20;
    case med:
        return 15;
    case easy:
        return 5;
    default:
        return 10; // Default fallback value
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -  QUESTION PHASE AND INTERACTION WITH THE USER   - - - - - - - - - - - - - - - - - - - - -

// Generates a question based on difficulty and question type.
// Retrieves operands, calculates the result, and formats the question text.
stQuestion generateQuestion(enLevelOfDifficulty difficulty, enQuestionsTypes QType)
{
    stQuestion q;
    stOperands operands;

    switch (difficulty)
    {
    case hard:
        operands = hardLevel(QType);
        break;
    case med:
        operands = medLevel(QType);
        break;
    case easy:
        operands = easyLevel(QType);
        break;
    case mixDifficulty:
        // Handle mixDifficulty by randomly selecting a level
        return generateQuestion(mixLevelOfDifficulty(), QType);
    }

    q.correctAnswer = operands.result; // Store the correct answer

    // Determine the operator symbol based on question type.
    string op;
    switch (QType)
    {
    case addition:
        op = " + ";
        break;
    case subtraction:
        op = " - ";
        break;
    case multiplication:
        op = " * ";
        break;
    case division:
        op = " / ";
        break;
    case mixQuestions:
        // Handle mixQuestions by randomly selecting a question type
        return generateQuestion(difficulty, mixQTypes());
    }
    // Format the final question text.
    q.questionText = to_string(operands.a) + op + to_string(operands.b) + " = ? ";
    return q;
}

// Displays question details before presenting the problem.
// Shows difficulty, type, question text, and available time.
void printQuestionInfo(enQuestionsTypes questionType, enLevelOfDifficulty questionDifficulty, stQuestion question)
{
    cout << "🎯 Difficulty: " << DifficultyResultToString(questionDifficulty) << endl;
    cout << "🔢 Type: " << QTypeResultToString(questionType) << endl;
    cout << "----------------------\n";

    cout << "📝 " << question.questionText << endl; // Display the question
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -  USER INPUT HANDLING AND ANSWER CHECKING   - - - - - - - - - - - - - - - - - - - - -

// Checks if the user's answer is correct and updates game statistics accordingly.
void checkUserAnswer(stQuestion question, int &userAnswer, stGameStats &stats)
{
    if (userAnswer == question.correctAnswer)
    {
        cout << "✅ Correct! Well done! 🎉\n";
        stats.correctAnswers++;
    }
    else
    {
        cout << "❌ Incorrect! The correct answer was: " << question.correctAnswer << "\n";
        stats.incorrectAnswers++;
    }
}

// Introduces a short delay before displaying the next question to improve user experience.
void waitBeforeNextQuestion(int numOfQuestions, int currentQuestion, int waitTime)
{
    if (currentQuestion < numOfQuestions)
    {
        cout << "\n⌛ Moving to the next question...\n";
        this_thread::sleep_for(chrono::seconds(waitTime));
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  - - - - - - - - - - - - - - - - - - - - -   PRINTING STATS  - - - - - - - - - - - - - - - - - - - - -

// Print Game Over message
void printGameOver()
{
    cout << "\n🏆 ---------------- GAME OVER ---------------- 🏆\n";
}

// Prints a summary of the user's performance, including total, correct, incorrect answers, and accuracy percentage.
void printPerformanceSummary(stGameStats stats, double percentage)
{
    cout << "\n📊 Performance Summary ⬇\n";
    cout << "+------------+-----------+-------------+----------1----+\n";
    cout << "|   🟰Total   | ✅Correct | ❌Incorrect | 🎯Percentage |\n";
    cout << "+------------+-----------+-------------+--------------+\n";
    cout << "| " << setw(10) << stats.totalAnswers << " | "
         << setw(9) << stats.correctAnswers << " | "
         << setw(11) << stats.incorrectAnswers << " | "
         << setw(11) << fixed << setprecision(1) << percentage << "% |\n";
    cout << "+------------+-----------+-------------+--------------+\n";
}

// Displays the number of total and correctly solved questions for each difficulty level.
void printDifficultyStats(stDifficultyStats difficultyStats)
{
    cout << "\n📊 Questions by Difficulty ⬇\n";
    cout << "+------------+--------+--------+\n";
    cout << "| Difficulty | Total  | Solved |\n";
    cout << "+------------+--------+--------+\n";
    cout << "| Easy       | " << setw(6) << difficultyStats.totalEasy << " | " << setw(6) << difficultyStats.solvedEasy << " |\n";
    cout << "| Medium     | " << setw(6) << difficultyStats.totalMed << " | " << setw(6) << difficultyStats.solvedMed << " |\n";
    cout << "| Hard       | " << setw(6) << difficultyStats.totalHard << " | " << setw(6) << difficultyStats.solvedHard << " |\n";
    cout << "+------------+--------+--------+\n";
}

// Displays the user's final score as a percentage and ranks their performance.
void printAccuracyStats(double percentage)
{
    cout << "🎯 Final Score: " << fixed << setprecision(1) << percentage << "%\n";

    // Provide feedback based on accuracy percentage
    if (percentage == 0)
        cout << "😢 Don't give up! Try again.\n";
    else if (percentage < 40)
        cout << "👍 Good effort! Keep practicing.\n";
    else if (percentage < 70)
        cout << "🌟 Well done! You're improving!\n";
    else if (percentage < 90)
        cout << "🔥 Excellent! You're a math champion!\n";
    else
        cout << "🏅 Perfect score! You're unstoppable!\n";

    cout << "--------------------------------------------\n";
}

// Displays the final game statistics, including performance summary, difficulty stats, and accuracy.
void printGameStats(stGameStats &stats, int numOfQuestions, stDifficultyStats difficultyStats)
{
    double percentage = (static_cast<double>(stats.correctAnswers) / numOfQuestions) * 100;
    // Print overall game stats
    printPerformanceSummary(stats, percentage);
    // Print breakdown by difficulty
    printDifficultyStats(difficultyStats);
    cout << "\n";
    // Print accuracy percentage and ranking
    printAccuracyStats(percentage);
}

// Updates difficulty statistics by tracking total and correctly solved questions per difficulty level.
void updateDifficultyStats(enLevelOfDifficulty questionDifficulty, stDifficultyStats &difficultyStats, bool isSolved)
{
    switch (questionDifficulty)
    {
    case enLevelOfDifficulty::hard:
        difficultyStats.totalHard++; // Track total hard questions attempted.
        if (isSolved)
        {
            difficultyStats.solvedHard++; // Track correctly answered hard questions.
        }
        break;

    case enLevelOfDifficulty::med:
        difficultyStats.totalMed++; // Track total medium questions attempted.
        if (isSolved)
        {
            difficultyStats.solvedMed++; // Track correctly answered medium questions.
        }
        break;

    case enLevelOfDifficulty::easy:
        difficultyStats.totalEasy++; // Track total easy questions attempted.
        if (isSolved)
        {
            difficultyStats.solvedEasy++; // Track correctly answered easy questions.
        }
        break;

    case enLevelOfDifficulty::mixDifficulty:
        // Handle mixDifficulty by choosing a random difficulty and updating the corresponding stats
        updateDifficultyStats(mixLevelOfDifficulty(), difficultyStats, isSolved);
        break;
    }
}

//  * * * * * * * * * * * * * * * * * * * * * * * *
// Handles the main game loop, generating questions, handling user input, and tracking game statistics.
void mainGame()
{
    // Step 1: Read user preferences (number of questions, difficulty, and question type)
    int numOfQuestions = readNumOfQuestions();
    enLevelOfDifficulty userDifficulty = chooseDifficulty();
    enQuestionsTypes userQType = chooseQType();

    // Step 2: Initialize game statistics
    stGameStats stats;                 // Tracks total, correct, and incorrect answers
    stDifficultyStats difficultyStats; // Tracks total and solved questions by difficulty

    // Step 3: Loop through each question
    for (int q = 1; q <= numOfQuestions; q++)
    {
        // Determine difficulty and type for this question (handles Mix Mode)
        enLevelOfDifficulty questionDifficulty = (userDifficulty == mixDifficulty) ? mixLevelOfDifficulty() : userDifficulty;
        enQuestionsTypes questionType = (userQType == mixQuestions) ? mixQTypes() : userQType;

        // Generate a random question based on the selected difficulty and type
        stQuestion question = generateQuestion(questionDifficulty, questionType);

        // Get the time limit for answering (based on difficulty level)
        int timeLimit = getTimeLimit(questionDifficulty);

        // Clear the screen to provide a clean interface before displaying the question
        clearScreen();

        // Display question information (difficulty, type, question text, and time limit)
        cout << "\n----------------------\n";
        cout << "📌 Question " << q << " of " << numOfQuestions << endl;
        printQuestionInfo(questionType, questionDifficulty, question);
        cout << "----------------------\n";

        // Read the user's answer with a time limit
        cout << "➤➤ Your Answer: ";
        int userAnswer;
        cin >> userAnswer;

        // Check if the user's answer
        checkUserAnswer(question, userAnswer, stats);

        // Track total questions answered
        stats.totalAnswers++;

        // Update difficulty-based statistics (tracking both total and solved questions)
        updateDifficultyStats(questionDifficulty, difficultyStats, userAnswer == question.correctAnswer);

        // Wait before moving to the next question to improve user experience
        waitBeforeNextQuestion(numOfQuestions, q, 2);
    }

    // Display the game over message and final statistics
    printGameOver();
    printGameStats(stats, numOfQuestions, difficultyStats);
}

// Asks the user if they want to play again and ensures valid input.
bool wantToPlayAgain()
{
    while (true)
    {
        char choice;
        cout << "\nWould you like to play again? (y/n): ";
        cin >> choice;

        // Convert to lowercase to handle both 'Y', 'y', 'N', and 'n'
        choice = tolower(choice);

        if (choice == 'y')
        {
            return true; // Restart the game
        }
        else if (choice == 'n')
        {
            cout << "Goodbye! Thanks for playing. 👋\n";
            return false; // Exit the game
        }
        else
        {
            cout << "❌ Invalid input! Please enter 'y' or 'n'.\n";

            // Clear the error flag to prevent input issues
            cin.clear();

            // Ignore any leftover invalid input to prevent an infinite loop
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main()
{
    // Seed the random number generator for generating random questions
    srand((unsigned)time(NULL));

    while (true)
    {
        mainGame(); // Start a new game session

        // Ask the user if they want to play again
        if (wantToPlayAgain())
            continue; // Restart the loop to play again
        else
            break; // Exit the loop and end the program
    }

    return 0;
}
