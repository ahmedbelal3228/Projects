"""
Project: Advanced Console-Based Online Store

Description:
This program simulates a shopping experience in the terminal. Users can:
1. Browse a variety of electronics (phones, laptops, accessories, etc.)
2. Add available items to a virtual cart
3. View the cart with item-wise and total cost
4. Clear the cart
5. Exit the program with a final summary

Features:
- Structured inventory using nested dictionaries
- Input validation and quantity checking
- Cart system that tracks quantity, price, and cost
- Clean tabular display with running totals

Goal:
Practice building a real-world system using Python fundamentals like dictionaries, functions, loops, and conditionals.
"""


# Dictionary of available items with their prices and quantity
items = {
    # Apple products
    "iPhone 13": {"price": 1000, "quantity": 10},
    "iPhone 14 Pro": {"price": 1100, "quantity": 8},
    "MacBook Pro": {"price": 2000, "quantity": 5},
    "MacBook Air M2": {"price": 1400, "quantity": 7},
    "AirPods Pro": {"price": 250, "quantity": 1},
    "AirPods Pro (2nd Gen)": {"price": 300, "quantity": 10},
    "iPad Pro": {"price": 800, "quantity": 15},
    "iPad Air (5th Gen)": {"price": 700, "quantity": 12},
    "Apple Watch Series 7": {"price": 600, "quantity": 8},
    "Apple Watch Ultra": {"price": 800, "quantity": 6},
    # Samsung products
    "Samsung Galaxy Z Fold 4": {"price": 1800, "quantity": 5},
    "Samsung Galaxy S23 Ultra": {"price": 1200, "quantity": 10},
    "Samsung Galaxy Note 20 Ultra": {"price": 950, "quantity": 6},
    # Google products
    "Google Pixel 7 Pro": {"price": 900, "quantity": 8},
    # OnePlus products
    "OnePlus 11": {"price": 700, "quantity": 12},
    # Xiaomi products
    "Xiaomi Mi 13 Pro": {"price": 750, "quantity": 9},
    # Sony products
    "Sony Xperia 1 IV": {"price": 1200, "quantity": 4},
    # Dell products
    "Dell XPS 15": {"price": 1500, "quantity": 4},
    # Microsoft products
    "Microsoft Surface Laptop 5": {"price": 1300, "quantity": 8},
    # GoPro products
    "GoPro HERO11 Black": {"price": 450, "quantity": 7},
    # Oppo products
    "Oppo Find X6 Pro": {"price": 1000, "quantity": 7},
    "Oppo Reno 8 Pro": {"price": 750, "quantity": 10},
    "Oppo A77 5G": {"price": 300, "quantity": 15},
    "Oppo F21 Pro 5G": {"price": 450, "quantity": 8},
    # Kindle products
    "Kindle Oasis": {"price": 250, "quantity": 20},
}


# Welcome message for the user
print("Welcome to our shop 😇")


def main():
    # Menu options message
    message = """
Available Options:
1. View Available Items
2. View Cart
3. Total Cart Price
4. Clear Cart
5. Quit
    """
    # Infinite loop to keep the program running until the user decides to quit
    while True:
        print(message)  # Display the menu options to the user
        user_option = input("What option do you want?: ")  # Ask user for their choice

        # Check user's option and call the appropriate function
        if user_option == "1":
            view_items()  # Display available items
        elif user_option == "2":
            view_cart()  # Display items in the cart
        elif user_option == "3":
            total_price()  # Calculate and display the total price of the cart
        elif user_option == "4":
            clear_cart()  # Clear the cart
        elif user_option == "5":
            Quit_program()  # Display the cart and quit the program
            break  # Exit the loop, ending the program
        else:
            print("Please enter a number between 1 and 5")  # Handle invalid input


# Initialize an empty dictionary to hold the user's cart items
cart = {}

# ================================ VIEW_ITEMS FUNCTION ================================


def view_items():
    # List to store item names for user selection
    items_lst = []

    # Function to display available items
    def display_items(dict):
        # Print the header of the table
        print(f"{'No.':<5} {'Item':<30} {'Price':<15} {'Quantity':<15}")
        # Loop through items and their details
        for i, (name, info) in enumerate(dict.items(), 1):
            price = info["price"]
            quantity = info["quantity"]
            items_lst.append(name)  # Add item name to the list
            if quantity > 0:
                print(
                    f"{i:<5} {name:<30} {price:<15} {quantity:<15}"
                )  # Print available items
            else:
                print(
                    f"{i:<5} {name:<30} {price:<15} {'out of stock':<15}"
                )  # Indicate if an item is out of stock

    display_items(items)  # Call the function to display available items

    # Loop to allow user to select items to add to the cart
    while True:
        try:
            user_choice = int(
                input(
                    "Enter the number of the item you want.\n(0 to the previous menu)\n--> "
                )
            )
            # Check if the choice is within the valid range
            if 1 <= user_choice <= len(items_lst):
                # Get the selected item based on user choice
                user_item = items_lst[user_choice - 1]
                try:
                    # Ask for the quantity
                    user_quantity = int(input("What is the quantity?: "))
                    # Check if the requested quantity is available
                    if user_quantity <= items[user_item]["quantity"]:
                        if user_item in cart:
                            # Add to existing item quantity in the cart
                            cart[user_item]["quantity"] += user_quantity
                        else:
                            # Add a new item to the cart
                            cart[user_item] = {
                                "quantity": user_quantity,
                                "price": items[user_item]["price"],
                            }

                        # Update the remaining quantity
                        remaining_quantity = (
                            items[user_item]["quantity"] - user_quantity
                        )
                        items[user_item].update(
                            {"quantity": remaining_quantity}
                        )  # Update the item's quantity in the inventory
                        print(
                            f"\n{user_quantity} {user_item} added to your cart ✔"
                        )  # Confirm the addition to the cart
                    else:
                        # Handle cases where the requested quantity is unavailable
                        if items[user_item]["quantity"] > 1:
                            print(
                                f"Sorry.. We only have {items[user_item]['quantity']} items of {user_item}"
                            )
                        elif items[user_item]["quantity"] == 1:
                            print(
                                f"Sorry.. We only have {items[user_item]['quantity']} item of {user_item}"
                            )
                        else:
                            print(f"Sorry.. {user_item} not available ")
                except ValueError:
                    print(
                        "Invalid input, please enter a number for the quantity"
                    )  # Handle invalid quantity input
            elif user_choice == 0:
                break  # Exit the loop, returning to the main menu
            else:
                print(
                    f"Please enter a number between 1 and {len(items_lst)}"
                )  # Handle invalid item selection
        except ValueError:
            print(
                f"Invalid option!.. please enter a number between 1 and {len(items_lst)}"
            )  # Handle non-integer input


# ================================ VIEW_CART FUNCTION ================================


def view_cart():
    # Inform the user if the cart is empty
    if not cart:
        print("Your cart is empty")
    else:
        # List to store the costs of items in the cart
        costs_lst = []
        print("Your Cart:")  # Header for the cart display
        print(f"{'No.':<5} {'Item':<30} {'Price':<10} {'Quantity':<10} {'Cost':<15}")
        # Loop through cart items
        for i, (name, info) in enumerate(cart.items(), 1):
            price = info["price"]
            quantity = info["quantity"]
            total_item_cost = quantity * price  # Calculate the cost of each item
            costs_lst.append(total_item_cost)  # Add the cost to the list
            print(
                f"{i:<5} {name:<30} {price:<10} {quantity:<10} {total_item_cost:<15}"
            )  # Display the cart items
        total_cost = sum(costs_lst)  # Calculate the total cost of the cart
        print(f"Total: ${total_cost}")  # Display the total cost


# ================================ SUM_TOTAL_PRICE FUNCTION ================================


def total_price():
    # Inform the user if the cart is empty
    if not cart:
        print("Your cart is empty")
    else:
        # List to store the costs of items in the cart
        costs_lst = []
        # Loop through cart items to calculate the total price
        for info in cart.values():
            price = info["price"]
            quantity = info["quantity"]
            total_item_cost = quantity * price  # Calculate the cost of each item
            costs_lst.append(total_item_cost)  # Add the cost to the list
        total_cost = sum(costs_lst)  # Calculate the total cost of the cart
        print(f"Total: ${total_cost}")  # Display the total cost


# ================================ CLEAR_THE_CART FUNCTION ================================


def clear_cart():
    # Inform the user if the cart is already empty
    if not cart:
        print("Your cart is already empty")
    else:
        cart.clear()  # Clear the cart
        print(
            "Your cart has been cleaned successfully"
        )  # Confirm the cart has been cleared


# ================================ QUIT_PROGRAM FUNCTION ================================


def Quit_program():
    view_cart()  # Display the cart before quitting
    print("Have a great day 😊")  # Farewell message


# Start the program by calling the main function
if __name__ == "__main__":
    main()
