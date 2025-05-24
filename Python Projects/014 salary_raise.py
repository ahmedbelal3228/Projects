"""
Project: Department Salary Raise Tool

Description:
This console-based tool allows users to give salary raises to all employees
within a specific department. The program checks if the department exists,
then applies the raise and shows the updated salary details.

Features:
- Takes user input for department and raise amount
- Validates department existence and numeric input
- Updates salaries for all employees in the chosen department
- Displays before-and-after salary info per employee

Goal:
Practice working with:
- Lists of dictionaries
- Input validation and user interaction
- Looping, conditional logic, and data manipulation
"""

# Original list of employees
employees = [
    {"name": "Mohamed Ali", "age": 25, "salary": 45000, "department": "Sales"},
    {"name": "Islam Ahmed", "age": 30, "salary": 60000, "department": "Marketing"},
    {"name": "Osama Ashraf", "age": 35, "salary": 38000, "department": "Sales"},
    {"name": "Seif Ali", "age": 40, "salary": 77000, "department": "Engineering"},
    {"name": "Ayman Hamed", "age": 45, "salary": 80000, "department": "Sales"},
    {"name": "Ahmed Alaa", "age": 33, "salary": 76000, "department": "Engineering"},
    {"name": "Sara Nabil", "age": 28, "salary": 50000, "department": "Marketing"},
    {"name": "Hassan Youssef", "age": 50, "salary": 90000, "department": "Management"},
    {"name": "Nourhan Khaled", "age": 29, "salary": 55000, "department": "Sales"},
    {"name": "Khaled Mostafa", "age": 32, "salary": 62000, "department": "Engineering"},
]


# Function to raise salary for employees in a specific department
def raise_salary(employees_lst, criteria, raised_amount):
    new_employees_info = []  # List to store updated employee information
    num_employees_raised = 0  # Counter for number of employees who got a raise
    for employee in employees_lst:
        if employee["department"] == criteria:
            old_salary = employee["salary"]
            new_salary = old_salary + raised_amount
            updated_employee = (
                employee.copy()
            )  # Create a copy of the employee dictionary
            updated_employee["salary"] = new_salary  # Update the salary
            new_employees_info.append(
                updated_employee
            )  # Add updated employee to the new list
            print(
                f"{employee['name']}'s salary was ${old_salary:,.2f}, and now is ${new_salary:,.2f}."
            )
            num_employees_raised += 1  # Increment the counter
        else:
            new_employees_info.append(
                employee.copy()
            )  # Add the employee without changes

    if num_employees_raised == 0:
        print(f"No employees found in the '{criteria}' department !")
    else:
        print(
            f"We have raised {num_employees_raised} employees in the {criteria} department ✔"
        )


# Main loop to interact with the user
while True:
    department = input(
        "\nEnter the department to give raise: "
    ).title()  # Get department name from user
    check_department = False  # Flag to check if the department exists

    # Check if the entered department exists in the employee list
    for employee in employees:
        if employee["department"] == department:
            check_department = True

    if check_department:
        while True:
            try:
                amount = float(
                    input("Enter the amount to raise: ")
                )  # Get raise amount from user
                raise_salary(
                    employees, department, amount
                )  # Call the function to raise salary
                break  # Exit the loop if the amount is valid
            except ValueError:
                print(
                    "Please enter a valid number for the raise amount !"
                )  # Handle invalid input
    else:
        print("Department not found!")  # Inform user if the department does not exist
