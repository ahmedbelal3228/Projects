"""
Project: Full ATM Simulation System

Description:
A console-based ATM simulation for managing real-world banking operations.
Users can:
1. Insert a card and authenticate using a PIN
2. Deposit and withdraw money
3. View account balance and transaction history
4. Change their card PIN securely
5. Transfer money between accounts

Core Features:
- Object-Oriented Design using classes and inheritance
- Use of `Enum` for menu, card, and transaction types
- Abstract `Transaction` class with deposit, withdrawal, inquiry, and transfer subclasses
- Multiple banks, customers, accounts, and cards
- Input validation, PIN verification, and transaction logs
- Reusable handlers for actions like deposit, withdrawal, transfer, and PIN change

Technologies & Concepts:
- Python OOP (classes, inheritance, encapsulation)
- Abstraction (`ABC`)
- Enums for clean option handling
- Modular architecture (each responsibility in its class)
- Input handling and validation
- Screen clearing for better user experience

Goal:
Simulate a real-world ATM system to practice advanced OOP, design patterns, and system modeling in Python.
"""

import os
import platform
from abc import ABC, abstractmethod
import datetime
from enum import Enum


# =====================================ENUM==================================================
# =====================================CLASSES===============================================
# ================================TRANSACTION TYPED================================
class TransactionType(Enum):
    DEPOSIT = "Deposit"
    WITHDRAW = "Withdraw"
    BALANCE_INQUIRY = "Balacne Inquiry"
    TRANSFER = "Transfer"


# ================================CARD TYPES================================
class CardType(Enum):
    DEBIT = "Debit"
    CREDIT = "Credit"


# ================================MENU OPTIONS================================
class MenuOption(Enum):
    DEPOSIT = "1"
    WITHDRAW = "2"
    BALANCE_INQUIRY = "3"
    TRANSACTION_HISTORY = "4"
    CHANGE_PIN = "5"
    TRANSFER_MONEY = "6"
    EXIT = "7"


# ================================CARD================================
class Card:
    def __init__(self, number, pin, type: TransactionType):
        self.number = number
        self.__pin = pin
        self.type = type

    def get_pin(self):
        return self.__pin

    def set_pin(self, old_pin, new_pin):
        """changing the old_pin to a pin that the user enter"""
        if self.__pin == old_pin:
            if isinstance(new_pin, str) and new_pin.isdigit():
                self.__pin = new_pin
                return True

        return False


# ================================CLEAR SCREEN CLASS================================
class Screen:
    # _________________________________________________________________
    def clear_screen(self):
        os.system("cls" if platform.system() == "Windows" else "clear")

    # _________________________________________________________________
    def display_message(self, message):
        print(message)

    # _________________________________________________________________
    def display_menu(self, bank_name, atm_location):
        message = f"""
Welcome to {bank_name} ATM ({atm_location})
Please select an option:
1. Deposit Money
2. Withdraw Money
3. Check Balance
4. View Transaction History
5. Change PIN
6. Transfer money to another account
7. Exit
"""
        self.display_message(message)


# ================================KEYPAD================================
class Keypad:
    def get_input(self, message):
        return input(message).strip()


#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# ================================ACCOUNT================================
class Account:
    def __init__(self, account_number, balance=0):
        self.account_number = account_number
        self.linked_cards = []
        self.transaction_history = []
        self.balance = balance
        self.linked_card = None

    # ____________________________________________
    def apply_deposit(self, amount):
        # Create a Transaction object to record the deposit, and apply it
        transaction = DepositTransaction(
            self, TransactionType.DEPOSIT, amount, len(self.transaction_history) + 1
        )

        transaction.apply_transaction()
        # Append the transaction to the transa action history list
        self.transaction_history.append(transaction)

    # ____________________________________________
    def apply_withdraw(self, amount):
        # Create a Transaction object to record the withdrawal , and apply it
        transaction = WithdrawTransaction(
            self, TransactionType.WITHDRAW, amount, len(self.transaction_history) + 1
        )
        transaction.apply_transaction()
        # Append the transaction to the transaction history list
        self.transaction_history.append(transaction)

    # ____________________________________________
    def show_balance(self):
        current_balance = BalanceInquiry(
            self,
            TransactionType.BALANCE_INQUIRY,
            self.balance,
            len(self.transaction_history) + 1,
        )
        current_balance.apply_transaction()
        self.transaction_history.append(current_balance)

    # ____________________________________________
    def show_transaction_history(self):
        if self.transaction_history:
            # Display each transaction's details using __str__ of Tranzsaction
            print(f"Transaction History for account: {self.account_number}:")
            for i, transaction in enumerate(self.transaction_history, 1):
                print(f"{i}. {transaction}")
        else:
            print("No transactions avialable")

    # ____________________________________________
    def __str__(self):
        card_info = ", ".join(
            [f"{card.number}({card.type.value}) " for card in self.linked_cards]
        )
        return f"Account number: {self.account_number} , Balance: ${self.balance} ,linked Cards: {card_info}"

    # ____________________________________________
    def add_card(self, card: Card):
        self.linked_cards.append(card)
        print(f"Card added successufully to your account")

    def apply_transfer(self, amount, recipient_account):
        # Step 1: Create and apply the transaction for the sender
        sender_transaction = TransferMoneyTransaction(
            self,
            TransactionType.TRANSFER,
            -amount,  # note that amount is negative to deduct the balance
            len(self.transaction_history) + 1,
        )

        # Only proceed if the sender's transaction is successful
        if not sender_transaction.apply_transaction():
            return

        self.transaction_history.append(sender_transaction)

        # Step 2: Create and apply the transaction for the recipient
        recipient_transaction = TransferMoneyTransaction(
            recipient_account,
            TransactionType.TRANSFER,
            amount,
            len(recipient_account.transaction_history) + 1,
        )
        recipient_transaction.apply_transaction()
        recipient_account.transaction_history.append(recipient_transaction)


# ================================CUSTOMER================================
class Customer:
    def __init__(self, name: str, phone: str, email: str, address: str):
        self.name = name
        self.phone = phone
        self.email = email
        self.address = address
        self.accounts = {}

    def add_account(self, account: Account):
        """Add an Account object to the customer."""
        self.accounts[account.account_number] = account

    def __str__(self):
        return f"{self.name.title()} ,Phone: {self.phone} ,Email: {self.email}"


# ================================BANK================================
class Bank:
    def __init__(self, name: str, swift: str):
        self.name = name
        self.swift = swift
        self.customers = []
        self.accounts = {}
        self.card = []

    # ____________________________________________
    def add_customer(self, customer: Customer):
        self.customers.append(
            customer
        )  # adding a customer to the list of Bank's customers

        for account in customer.accounts.values():  # adding account to the bank
            self.accounts[account.account_number] = account

    # ____________________________________________
    def show_customers(self):
        print("Customers....")
        for i, customer in enumerate(self.customers, 1):
            print(f"{i}- {customer}")


#####===================================HANDLER CLASSES===================================#####
###################################################################################################
class DepositHandler:
    def __init__(self, keypad, screen):
        self.keypad = keypad
        self.screen = screen

    def handle(self, account):
        while True:
            try:
                # Get and validate the deposit amount
                amount = int(self.keypad.get_input("Enter the amount to Deposit: "))
                if amount <= 0:
                    self.screen.display_message(
                        "Ivalid amount , Please enter a positive integer amount"
                    )
                    continue
                # Delegate deposit logic to the Account class
                account.apply_deposit(amount)
                break
            # If the input was a string or non_integer
            except ValueError:
                self.screen.display_message(
                    "Invalid amount, Please enter a valid number"
                )


class WithdrawHandler:
    def __init__(self, keypad, screen):
        self.keypad = keypad
        self.screen = screen

    def handle(self, account):
        while True:
            try:
                # Get and validate the deposit amount
                amount = int(self.keypad.get_input("Enter the amount to Withdraw: "))
                if amount <= 0:
                    self.screen.display_message(
                        "Ivalid amount , Please enter a positive integer amount"
                    )
                    continue
                # Delegate deposit logic to the Account class
                account.apply_withdraw(amount)
                break
            # If the input was a string or non_integer
            except ValueError:
                self.screen.display_message(
                    "Invalid amount, Please enter a valid number"
                )


class BalanceInquiryHandler:
    def handle(self, account):
        account.show_balance()


class TransactionHistoryHnadler:
    def handle(self, account):
        account.show_transaction_history()


class ChangingPinHnadler:
    def __init__(self, screen, keypad):
        self.screen = screen
        self.keypad = keypad

    def handle(self, card: Card):
        while True:
            # Step 1: Verify old PIN
            old_pin = self.keypad.get_input("Enter your current pin: ")
            if card.get_pin() != old_pin:
                self.screen.display_message(
                    "Wrong PIN , PIN change failed..Please try later"
                )
                continue

            # Step 2: Input and confirm new PIN
            new_pin = self.keypad.get_input("Enter your new PIN: ")
            confirm_pin = self.keypad.get_input("Confirm your new PIN: ")
            if new_pin != confirm_pin:
                self.screen.display_message("New PINs do not match. PIN change failed.")
                continue

            # Step 3: Validate and set the new PIN
            if card.set_pin(old_pin, new_pin):
                self.screen.display_message("PIN changed successfully.")
                break
            else:
                self.screen.display_message("Failed to update PIN. Please try again.")
                continue


class TransferMoneyHandler:
    def __init__(self, keypad, screen):
        self.keypad = keypad
        self.screen = screen

    def handle(self, account, bank):
        while True:
            try:
                # Step 1: Input recipient account number
                recipient_account_number = self.keypad.get_input(
                    "Enter recipient account number: "
                )

                recipient_account = bank.accounts.get(recipient_account_number)
                if not recipient_account:
                    self.screen.display_message(
                        "Recipient account not found. Please try again."
                    )
                    continue

                # Step 3: Input transfer amount
                amount = int(self.keypad.get_input("Enter amount to transfer: "))

                # Step 4: Validate transfer amount
                if amount <= 0:
                    self.screen.display_message(
                        "Amount must be positive. Please try again."
                    )
                    continue

                # Step 5: Perform the transfer
                account.apply_transfer(amount, recipient_account)
                # Display success message
                self.screen.display_message(
                    f"${amount} successfully transferred to account {recipient_account.account_number}."
                )
                self.screen.display_message(f"You balance now : ${account.balance}")

                break
            except ValueError:
                self.screen.display_message(
                    "Invalid input. Please enter a valid number."
                )


#####===================================TRANSACTION CLASSES===================================#####
###################################################################################################


# ================================TRANSACTION================================
class Transaction(ABC):
    def __init__(self, account: Account, type: TransactionType, amount, id):
        self.account = account
        self.type = type
        self.amount = amount
        self.id = id
        self.timestamp = datetime.datetime.now()

    @abstractmethod
    def apply_transaction(self):
        """Applying the transacton , this will be implemented by the sublcasses that will inherit from the Transaction class"""
        pass

    def __str__(self):
        # Provides a formatted string for easy printing of transaction details
        return f"Transaction ID: {self.id}, Type: {self.type.value}, Amount: ${self.amount}, Time: {self.timestamp.strftime('%Y-%m-%d %H:%M:%S')}"


# ================================DEPOSIT-TRANSACTION-(SUBCLASS)================================
class DepositTransaction(Transaction):
    """
    Handles deposit transactions by increasing the account balance.
    """

    def apply_transaction(self):
        self.account.balance += self.amount
        print(
            f"${self.amount} has been added successfully\nNew balance: ${self.account.balance}"
        )


# ================================WITHDRAW-TRANSACTION-(SUBCLASS)================================
class WithdrawTransaction(Transaction):
    """
    Handles withdrawal transactions by decreasing the account balance if sufficient funds are available.
    """

    def apply_transaction(self):
        if self.account.balance - self.amount >= 0:
            self.account.balance -= self.amount
            print(
                f"${self.amount} has been withdrawn successfully\nNew balance: ${self.account.balance}"
            )
        else:
            print("Insufficient fund!")
            print(f"you only have ${self.account.balance}")


# ================================BALACNE-INQUIRY-SUBCLASS================================
class BalanceInquiry(Transaction):
    def apply_transaction(self):
        print(f"Your balance is: ${self.account.balance}")


class TransferMoneyTransaction(Transaction):
    """
    Handles transfer transactions for both sender and recipient accounts.
    """

    def apply_transaction(self):
        # Ensure the transaction amount is valid
        if self.amount < 0 and self.account.balance + self.amount < 0:
            print("Insufficient funds to complete the transfer.")
            return False
        self.account.balance += self.amount
        return True


# ================================VERIFICATION CLASS================================
class Verification:
    def __init__(self, atm):
        # Reference to the ATM object to access its attributes and resources
        self.atm = atm

    # ____________________________________________
    def verify_pin(self):
        """
        Verifies the PIN for the current card in the ATM.
        """
        if not self.atm.current_account.linked_cards:
            self.atm.screen.display_message(
                "No cards linked to this account. Please link a card first."
            )
            return False

        entered_pin = self.atm.keypad.get_input("Enter PIN: ")
        for card in self.atm.current_account.linked_cards:
            if (
                self.atm.current_card.get_pin() == entered_pin
            ):  # Check the entered PIN against the card's PIN
                return True
        return False

    # _________________________________________________________________
    def search_card(self):
        MAX_RETRIES = 5  # Maximum retries for both card and PIN
        retry_counts = 0
        """
        Searches for a card in the bank's customer data and validates it.
        If the card is found and the PIN is correct, sets the ATM's current account and card.
        """
        while retry_counts < MAX_RETRIES:
            card_num = self.atm.keypad.get_input("Enter your card number: ")

            # Search through all customers and their accounts for the card
            for account in self.atm.bank.accounts.values():
                for card in account.linked_cards:
                    if card.number == card_num:  # Card found
                        self.atm.current_card = card
                        self.atm.current_account = account
                        self.atm.screen.display_message(
                            "Valid Card! Please enter your PIN."
                        )
                        attempts = 0
                        while attempts < MAX_RETRIES:
                            if self.verify_pin():  # Call verify_pin to check the PIN
                                self.atm.screen.display_message(
                                    "PIN verified. Welcome!"
                                )
                                return account
                            attempts += 1
                            remaining_attempts = MAX_RETRIES - attempts
                            self.atm.screen.display_message(
                                f"Invalid PIN. {remaining_attempts} attempts remaining."
                            )

                        self.atm.screen.display_message(
                            "Too many invalid attempts. Please try again later."
                        )
                        self.atm.current_card = None
                        self.atm.current_account = None
                        return None

            # # If the card is not found
            self.atm.screen.display_message(
                "Invalid card! There is no account linked to this card."
            )
            continue


##================================ATM INTERFACE================================


#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# ===================================ATM CLASS=========================================
class ATM:
    def __init__(self, bank: Bank, atm_location):
        self.bank = bank
        self.atm_location = atm_location
        self.current_card = None
        self.current_account = None
        self.keypad = Keypad()
        self.screen = Screen()
        self.deposit_handler = DepositHandler(self.keypad, self.screen)
        self.withdraw_handler = WithdrawHandler(self.keypad, self.screen)
        self.transaction_history_handler = TransactionHistoryHnadler()
        self.balance_iquiry_handler = BalanceInquiryHandler()
        self.changing_pin_handler = ChangingPinHnadler(self.screen, self.keypad)
        self.transfer_money_handler = TransferMoneyHandler(self.keypad, self.screen)
        # Initialize Verification with a reference to this ATM instance
        self.verification = Verification(self)

    # _________________________________________________________________
    def insetrt_card(self):
        self.screen.display_message("Please insert your card.")
        account = self.verification.search_card()
        if account:
            self.screen.display_message(f"Welcome to your account: {account}")
        self.handle_user_option()

    # _________________________________________________________________
    def handle_user_option(self):
        while self.current_account:
            self.screen.display_menu(self.bank.name, self.atm_location)
            option = self.keypad.get_input("Enter your option here: ")

            if option == MenuOption.DEPOSIT.value:
                self.screen.clear_screen()
                self.deposit_handler.handle(self.current_account)

            elif option == MenuOption.WITHDRAW.value:
                self.screen.clear_screen()
                self.withdraw_handler.handle(self.current_account)

            elif option == MenuOption.BALANCE_INQUIRY.value:
                self.screen.clear_screen()
                self.balance_iquiry_handler.handle(self.current_account)

            elif option == MenuOption.TRANSACTION_HISTORY.value:
                self.screen.clear_screen()
                self.transaction_history_handler.handle(self.current_account)

            elif option == MenuOption.CHANGE_PIN.value:
                self.screen.clear_screen()
                self.changing_pin_handler.handle(self.current_card)

            elif option == MenuOption.TRANSFER_MONEY.value:
                self.screen.clear_screen()
                self.transfer_money_handler.handle(self.current_account, self.bank)

            elif option == MenuOption.EXIT.value:
                self.screen.display_message(
                    "Thank you for using the ATM. Have a great day!"
                )
                self.current_card = None
                self.current_account = None
                break

            else:
                self.screen.display_message("Please chooce a valid option !")

            # Clearing the screen everytime the user chooces an option
            self.keypad.get_input("\nPress enter to display the menu again: ")
            self.screen.clear_screen()


# =========================================================================================================
# =========================================================================================================
# =========================================================================================================
# =========================================================================================================

# USE CASE
# =============== Initialize Banks ===============
bank1 = Bank("Bank Masr", "EGBM")
bank2 = Bank("National Bank", "NBEG")
bank3 = Bank("CIB", "CIBE")

# =============== Add Customers ===============
# Customers for Bank Masr
cust1 = Customer("Ahmed Belal", "01080260373", "ahmed@gmail.com", "Alexandria/Mandara")
cust2 = Customer("Salma Ibrahim", "01122334455", "salma@example.com", "Cairo/Maadi")

# Customers for National Bank
cust3 = Customer("Omar Adel", "01233445566", "omar@example.com", "Giza/6th October")
cust4 = Customer("Sara Khaled", "01066778899", "sara@example.com", "Alexandria/Stanley")

# Customers for CIB
cust5 = Customer("Kareem Fathy", "01599887766", "kareem@example.com", "Cairo/Zamalek")
cust6 = Customer("Nour Hassan", "01155667788", "nour@example.com", "Cairo/New Cairo")

# =============== Add Accounts ===============
# Accounts for Ahmed Belal (Bank Masr)
acc1 = Account("EGBA-123456", balance=1000)
acc2 = Account("EGBA-789012", balance=500)

# Accounts for Salma Ibrahim (Bank Masr)
acc3 = Account("EGBA-345678", balance=2000)

# Accounts for Omar Adel (National Bank)
acc4 = Account("NBEG-567890", balance=1500)

# Accounts for Sara Khaled (National Bank)
acc5 = Account("NBEG-112233", balance=800)

# Accounts for Kareem Fathy (CIB)
acc6 = Account("CIBE-445566", balance=3000)

# Accounts for Nour Hassan (CIB)
acc7 = Account("CIBE-778899", balance=4000)
acc8 = Account("CIBE-991122", balance=2500)

# =============== Add Cards to Accounts ===============
card1 = Card("1111-9999-2223-4747", "1234", CardType.DEBIT)
card2 = Card("1111-8888-2222-5555", "5678", CardType.CREDIT)
card3 = Card("1111-7777-3333-6666", "9999", CardType.DEBIT)

card4 = Card("2222-9999-1111-4444", "1111", CardType.DEBIT)
card5 = Card("2222-8888-4444-3333", "4321", CardType.CREDIT)
card6 = Card("2222-5555-6666-7777", "6789", CardType.DEBIT)

card7 = Card("3333-4444-5555-8888", "2468", CardType.DEBIT)
card8 = Card("3333-9999-8888-1111", "1357", CardType.CREDIT)
card9 = Card("3333-5555-7777-2222", "9753", CardType.DEBIT)

# Link cards to accounts
acc1.add_card(card1)
acc1.add_card(card2)
acc3.add_card(card3)

acc4.add_card(card4)
acc5.add_card(card5)

acc6.add_card(card6)
acc7.add_card(card7)

# =============== Add Accounts to Customers ===============
cust1.add_account(acc1)
cust1.add_account(acc2)

cust2.add_account(acc3)

cust3.add_account(acc4)
cust4.add_account(acc5)

cust5.add_account(acc6)
cust6.add_account(acc7)
cust6.add_account(acc8)

# =============== Add Customers to Banks ===============
bank1.add_customer(cust1)
bank1.add_customer(cust2)

bank2.add_customer(cust3)
bank2.add_customer(cust4)

bank3.add_customer(cust5)
bank3.add_customer(cust6)

# =============== Start the ATM System ===============
# Example: Start an ATM for Bank Masr
atm1 = ATM(bank1, "Alexandria")
atm1.insetrt_card()

# Example: Start an ATM for National Bank
atm2 = ATM(bank2, "Giza")
atm2.insetrt_card()
