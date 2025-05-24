"""
Project: Console-Based Online Store

Description:
A simple terminal app where users can:
1. View available items (price + stock)
2. Add items to their cart
3. See cart contents and total cost
4. Exit with a summary

Features:
- Uses dictionaries for inventory and cart
- Handles user input and validation
- Displays data in a clean table format

Goal:
Practice basic Python skills in a real-world mini project.
"""

items = {
    "Iphone 12": {"price": 800, "quantity": 6},
    "Iphone 12 Pro": {"price": 950, "quantity": 4},
    "Iphone 12 Pro Max": {"price": 110, "quantity": 3},
    "Samsung Galaxy S21": {"price": 750, "quantity": 8},
    "Samsung Galaxy S21 Ultra": {"price": 1200, "quantity": 5},
    "Google Pixel 5": {"price": 700, "quantity": 7},
    "OnePlus 9": {"price": 650, "quantity": 10},
    "Sony Xperia 5": {"price": 900, "quantity": 2},
    "Huawei P40 Pro": {"price": 1000, "quantity": 6},
    "Xiaomi Mi 11": {"price": 600, "quantity": 12},
    "Oppo Find X3": {"price": 950, "quantity": 4},
    "Asus Rog Phone 5": {"price": 1300, "quantity": 1},
}

# the message that will be shown to the user (conatain the option)
first_message = """\nWelcome To Our Store...
Please choose an option:
1. View available items
2. View the content of you cart
3. View the total price of your cart
4. Quit
"""

repeated_message = """
OPTIONS:
1. View available items
2. View the content of you cart
3. View the total price of your cart
4. Quit
"""
# printing the options message
print(first_message)

# creating the cart
cart = {}

while True:
    # getting the user choise
    inp = input("Enter the number of your choice: ")

    # ==============================================================================================
    # 1. if the user choose to view available items
    if inp == "1":
        # printing the head of the table
        print(f"\n{'No.':<5} {'Item':<25} {'Price':<15} {'Quantity':<10}")
        # create a list to take the item index from it
        items_list = list(items.keys())
        # loop through the mian dict to get the item and the info(price & quantity)
        for i, item in enumerate(items, 1):
            # check first if the item is in the stock (quantity > 0)
            if items[item]["quantity"] > 0:
                print(
                    f"{i:<5} {item:<25} {items[item]['price']:<15} {items[item]['quantity']:<10}"
                )
            # if the user took all the quantity (if the item not available anymore)
            else:
                print(
                    f"{i:<5} {item:<25} {items[item]['price']:<15} {'out of stock':<10}"
                )
        # ask the user to choose item
        user_choice = int(input("\nchoose an item, (0 to step back): "))
        # if the user choosed to return to the main windows (pressed 0)
        if user_choice == 0:
            print(repeated_message)
            continue  # go to the start of the while loop
        if 1 <= user_choice <= len(items_list):
            user_item = items_list[
                user_choice - 1
            ]  # the 1 that we added to display the table

            # check if the user choise is in the store
            if user_item in items:
                # check if this item is available before asking for the quantity
                if items[user_item]["quantity"] == 0:
                    print(f"Sorry!.. {user_item.upper()} is out of stock")
                    continue
                # asking the user for the quantity
                user_quantity = int(input("Enter the quantity: "))
                # check if required quantity is exists
                if user_quantity <= items[user_item]["quantity"]:
                    # check if the item is in the cart (the user wants to buy for the second time)
                    if user_item in cart:
                        # add the user_quantity to the quantity in the cart
                        cart[user_item]["quantity"] += user_quantity
                        print(f"{user_quantity} {user_item} added to you cart")
                    else:
                        # updating the item info in the cart (he is buying this item for the first time)
                        cart[user_item] = {
                            "price": items[user_item]["price"],
                            "quantity": user_quantity,
                        }
                        print(f"{user_quantity} {user_item} added to you cart")

                    # subtract what user took from the original quantity in the store
                    items[user_item]["quantity"] -= user_quantity

                else:
                    print(f"\nSorry!.. Not enough quantity in the store")

            # if the user choise was out of the store --> print not found
            else:
                print("\nnot found !")
                continue
        else:
            print("\nInvalid selection")
    # ==============================================================================================
    # 2. if the user choose to view his cart content
    elif inp == "2":
        # check if the cart is not empty
        if cart:  # if the cart is True (conatain items)
            print("Your Cart...")
            # the head of the table
            print(f"\n{'Item':<25} {'Price':<15} {'Quantity':<10}")
            # loop through the cart
            for item in cart:
                # defining the price & quantity
                price = cart[item]["price"]
                quantity = cart[item]["quantity"]
                # printing the result
                print(f"{item:<25} {price:<15} {quantity:<10}")
        # if the cart is not empty
        else:
            print("No items found in the cart !")

    # ==============================================================================================

    # 3. if the user choose the see the total price
    elif inp == "3":
        # check if the cart is not empty
        if cart:  # if the cart is True (conatain items)
            # make a variable to hold the total
            total = 0
            # loop through the cart to get the prices
            for item in cart:
                # sum the prices
                total += cart[item]["price"] * cart[item]["quantity"]

            # print the total message
            print(f"\nThe Total Cost Is ${total}")

        else:
            print("No items found in the cart")
            print("The Total Cost Is $0")
    # ==============================================================================================

    elif inp == "4":
        # another way to check if the cart is empty or not
        # convert the items of the cart the list to check the legths of it
        cart_list = list(cart.keys())
        # check if the cart is empty
        if len(cart_list) == 0:
            print("You cart is empty...")
            print("Pleased to meet you.")
            break
        else:
            print("Quitting The Program....")
            # ===================================================
            # print the Cart to the user before quitting
            print("\nYour Cart...")
            # the head of the tabl
            print(f"{'Item':<25} {'Price':<15} {'Quantity':<10}")
            # loop through the cart
            for item in cart:
                # defining the price & quantity
                price = cart[item]["price"]
                quantity = cart[item]["quantity"]
                # printing the result
                print(f"{item:<25} {price:<15} {quantity:<10}")
            # ===================================================
            # printing the Totol price before quitting
            # variable to store the total price in it
            total = 0
            # loop through the cart to get the prices (price * quantity)
            for item in cart:
                total += cart[item]["price"] * cart[item]["quantity"]
            print(f"Total Price ${total}")
            # exit the program
            break
    # ==============================================================================================

    # (big else) if the user choosed an option out of (1, 2, 3, 4)
    else:
        print("Ivalid choise..!")
        print("Pleas enter a number between 1 and 4.")

    print(repeated_message)
