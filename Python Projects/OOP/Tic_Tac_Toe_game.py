"""
Project: Tic Tac Toe Game (Console Version)

Description:
A two-player console-based Tic Tac Toe game with a user-friendly interface.
Players take turns placing X or O on a 3x3 board until one wins or the game ends in a draw.

Features:
- Start menu with options to play, view rules, or quit
- Turn-based gameplay with input validation
- Clear display of both the playable board and reference numbers
- Automatic win/draw detection
- End-of-game menu to restart, view score (future), or quit

Technologies & Concepts:
- Object-Oriented Programming (OOP)
- Class design for Board, Player, Menu, and Game
- Clean user interaction in the terminal
- Screen clearing for better game flow

Planned Future Enhancements:
- Add a Score class to track wins
- Detect and handle draw explicitly
- Improve restart behavior to maintain scores

Goal:
Practice OOP in Python while building an interactive, real-world game using structured code and user-driven logic.
"""

# ===========================================================================
# ____________________________________________
# FUTRUE UPDATED ......
# 1. Restart menu option when player win
# 2. adding the check draw menu
# 3. Score class
# ____________________________________________
import os


def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")


class Board:

    def __init__(self):
        self.cells = [" " for _ in range(9)]

    def display_board(self):
        """Displaying the board with and empty cells
        Args:
            None
        Returns:
            a board of 3*3 (9 empty cells)
        """
        print("-------------")
        print(f"  {self.cells[0]} | {self.cells[1]} | {self.cells[2]}")
        print("-------------")
        print(f"  {self.cells[3]} | {self.cells[4]} |  {self.cells[5]} ")
        print("-------------")
        print(f"  {self.cells[6]} | {self.cells[7]} | {self.cells[8]} ")
        print("-------------")

    def display_board_nums(self):
        """displaying the board as a nums in the cells"""
        print("-------------")
        print("  1 | 2 | 3")
        print("-------------")
        print("  4 | 5 | 6 ")
        print("-------------")
        print("  7 | 8 | 9 ")
        print("-------------")

    def clear_board(self):
        """Returns to board to 9 cells of nums and remove the symbols"""
        return self.display_board()

    # _________________________________________________________________

    def update(self, position: int, symbol: str) -> tuple[bool, str]:
        """
        Update the board with the player's move.

        Args:
            position (int): The position on the board (1-9)
            symbol (str): The player's symbol ('X' or 'O')

        Returns:
           tuple[bool, str]: (Success status, Error message if any)
        """
        if not self.is_valid_move(position):
            return False, f"Invalid move. Postition: {position} is not available"
        if not self.is_valid_symbol(symbol):
            return (
                False,
                f"Invalid symbol: {symbol}. You only can use either 'X' or 'O'",
            )
        # if they return True
        self.cells[position - 1] = symbol.upper()
        return True, ""

    # _________________________________________________________________
    def is_valid_move(self, position: int) -> bool:
        try:
            return 1 <= position <= 9 and self.cells[position - 1] == " "

        except (ValueError, IndexError):
            return False

    def is_valid_symbol(self, symbol: str) -> bool:
        # the symbol should be either X or O
        # if the player entered X he can't chooese O any more
        return symbol.upper().strip() in ("X", "O")


# ========================================================================
class Player:

    def __init__(self):
        self.name = ""
        self.symbol = ""

    def set_name(self):
        while True:

            name = input("What is your name?: ").strip()
            if name.isalpha():  # if the name is not empty
                self.name = name
                break
            else:
                print("Invalid name. Use letters only.")

    def set_symbol(self, the_other_symbol=None):
        while True:
            symbol = input("choose a symbol-> (X or O): ").upper().strip()
            if the_other_symbol and symbol == the_other_symbol:
                print(f"{symbol} is takes , choose the other symbol")
                continue

            if symbol.upper() not in ("X", "O"):
                print(f"Invalid symbol: {symbol}. You only can use either 'X' or 'O'")
                continue

            self.symbol = symbol
            return symbol


# ==============================================================
class Menu:

    def __init__(self):
        self.start_message = """
Welcome to our game: Tic Tac Toe
Please choose one of the following options:

1. Start New Game
2. View Rules
3. Quit The Game

Enter your choice: """

        self.end_message = """
______________________________
1. Play Again
2. View Score 
3.Quit The Game
        
Enter your choice: """

    def display_start_menu(self):
        print(self.start_message)

    def display_end_menu(self):
        print(self.end_message)

    def get_user_choice(self, message):
        while True:
            try:
                user_choice = int(input(message))
                if user_choice in (1, 2, 3):
                    return user_choice
                else:
                    print("Please enter a number between 1 and 3")
            except ValueError:
                print("Invalid input. Please enter a number")

    def run(self):
        while True:
            self.display_start_menu()
            choice = self.get_user_choice(self.start_message)
            if choice == 1:
                print("Starting new game.....")

            elif choice == 2:
                self.display_rules()

            elif choice == 3:
                print("Quitting the game...")
                break

    def display_rules(self):
        rules = """  
    Tic Tac Toe Rules:  
    1. The game is played on a 3x3 grid.  
    2. Players take turns placing their symbol (X or O) in empty cells.  
    3. The first player to get 3 of their symbols in a row (horizontally, vertically, or diagonally) wins.  
    4. If all cells are filled and no player has won, the game is a draw.  
    """
        print(rules)

    def end(self):
        while True:
            self.display_end_menu()
            choice = self.get_user_choice(self.end_message)
            if choice == 1:
                print("Starting new game....")
            elif choice == 2:
                print("Viewing score....")  # view score handling needed
            elif choice == 3:
                print("Quitting the game....")
                break


# ==============================================================
# THE MOST IMPORTANT CLASS IN THE GAME
# ==============================================================
class Game:

    def __init__(self):
        self.menu = Menu()
        self.board = Board()
        self.players = self.create_players()
        self.current_player_index = 0

    def create_players(self):
        """
        Create a list of two Player objects.
        """
        return [Player(), Player()]

    def setup_game(self):
        ## DISPLAYING THE BOARDS
        # Create board
        self.board = Board()  # Creating a board object
        # displaying the board with numbers
        print("\nHere's the board with numbered cells:")
        self.board.display_board_nums()
        print("\nRemember these numbers when making your move!")

        # displaying
        ## HERE WE WILL SETUP EACH PLAYER SEPRATELY
        # Setting up the first player
        print("\nPlayer 1: ...")
        self.players[0].set_name()
        first_symbol = self.players[0].set_symbol()
        print(f"{self.players[0].name}, Your symbol is {first_symbol}")

        # setting up the second player
        print("\nPlayer 2: ...")
        self.players[1].set_name()
        second_symbol = self.players[1].set_symbol(first_symbol)
        print(f"{self.players[1].name}, Your symbol is {second_symbol}")

        print("\nGame is ready to start ! Let's begin")

    def switch_turn(self):
        "Switch to the next player's turn."
        self.current_player_index = (self.current_player_index + 1) % 2

    def get_currnet_player(self):
        """Getting the currnet player object"""
        return self.players[self.current_player_index]

    def play_turn(self):
        """
        Play a turn for the current player.
        """
        # Clear the screen at the start of each player's turn
        clear_screen()

        # displating the two board (one for playing and the NumBoard for reference)
        print("\nCurrent board state:")
        self.board.display_board()
        print("\nNumbered board for reference:")
        self.board.display_board_nums()

        # get the current player object
        currnet_player = self.get_currnet_player()
        while True:
            try:
                move = int(input(f"{currnet_player.name}: Enter your move: "))
                if self.board.is_valid_move(move):
                    boolian, message = self.board.update(move, currnet_player.symbol)
                    if boolian:
                        break
                    else:
                        print(message)
                else:
                    print("Ivalid move, the cell is already occupied or out of range")
            except ValueError:
                print("Invalid input. Please enter a number between 1 and 9")

    def check_winner(self):
        """
        Check if the current player has won the game.
        Returns True if there's a winner, False otherwise.
        """
        winning_conditions = [
            [0, 1, 2],
            [3, 4, 5],
            [6, 7, 8],  # Rows
            [0, 3, 6],
            [1, 4, 7],
            [2, 5, 8],  # Columns
            [0, 4, 8],
            [2, 4, 6],  # Diagonals
        ]
        current_player = self.get_currnet_player()
        symbol = current_player.symbol
        # Check each winning condition
        for condition in winning_conditions:
            if all(self.board.cells[num] == symbol for num in condition):
                return True
        return False

    def game_play(self):
        """
        Main method to play the game.
        Displays the start menu once and then manages game sessions based on user input.
        """
        # Show the start menu once when the game begins
        start_choice = self.menu.get_user_choice(self.menu.start_message)

        # Handle start menu choice
        if start_choice == 1:
            print("Starting a new game...")
        elif start_choice == 2:
            # Display rules and return to the start menu
            self.menu.display_rules()
            return self.game_play()  # Re-display start menu after viewing rules
        elif start_choice == 3:
            # Exit the game
            print("Exiting the game. Thank you for playing!")
            return

        # Keep playing new games until the user chooses to quit
        while True:
            # 1. Set up the game
            self.setup_game()
            # 2. main loop of the game
            while True:
                # 2a. play the game
                self.play_turn()

                # 2b. impelement the turn on the board
                self.board.display_board()

                # 2c. check if there is a winner
                if self.check_winner():
                    current_player = self.get_currnet_player()
                    print(f"Congratulations, {current_player.name}! You won!")
                    break
                # 2d. Check for a tie
                if self.is_board_full():
                    print(f"it's a tie, the board is full")
                    break

                # 2e. switch to the next player's turn
                self.switch_turn()
            # 3. Game over !!
            print("Game over.")

            # 4. Display the end menu and handle the user's choice
            if not self.restart_game():  # Exit if the user chooses to quit
                break

    def restart_game(self):
        """
        Display the end game menu and handle the user's choice to restart the game,
        view the score, or quit.

        This function shows the end game menu to the user and captures their choice.
        Based on the input, it will either restart the game, display the score (if implemented),
        or quit the game with a farewell message.

        Returns:
            bool: True if the game should be restarted, False if the user chooses to quit.
        """

        # Display the end game menu and get the user's choice
        # self.menu.display_end_menu()
        # choice = self.menu.get_user_choice(self.menu.end_message)

        while True:
            # Display the end game menu and get the user's choice
            choice = self.menu.get_user_choice(self.menu.end_message)

            # Handle the user's choice
            if choice == 1:  # Restart the game
                return True
            elif choice == 2:  # View the score (placeholder for future implementation)
                print("Score viewing is not implemented yet.")
                # Continue the loop to re-display the end menu after viewing the score
            elif choice == 3:  # Quit the game
                print("Thank you for playing! Goodbye.")
                return False

    def is_board_full(self):
        """
        Check if the board is full (i.e., no more moves can be made).
        Returns True if the board is full, False otherwise.
        """
        return " " not in self.board.cells


# ====================================== TESTING ======================================

game = Game()
game.game_play()
