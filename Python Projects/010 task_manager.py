"""
Project: Task Manager (Console App)

Description:
A simple terminal-based task manager that allows users to:
1. Add tasks
2. Mark tasks as complete
3. View current tasks
4. View completed tasks

Features:
- Uses lists to store active and completed tasks
- Handles user input with validation
- Clean and simple user interface in the terminal

"""


# _____________________________________________________________________________
# Creating an empty list to hold the tasks
tasks = []
completed = []


# the actual code
def main():
    # Message displaying the main menu options
    message = """
    1. Add a task
    2. Mark as complete
    3. View tasks
    4. Show completed tasks
    5. Quit"""

    while True:
        # Print the options for the user
        print(message)
        # Get the user's choice of action
        choice = input("Choose an option: ")
        # Check which option the user selected and call the corresponding function
        if choice == "1":
            add_task()
        elif choice == "2":
            mark_complete()
        elif choice == "3":
            view_tasks()
        elif choice == "4":
            show_completed()
        elif choice == "5":
            # If the user chooses to quit, print a farewell message and exit the loop
            print("Have a productive day..")
            break
        else:
            # Inform the user if their input is invalid
            print("Invalid Choice.. please choose a number between 1 and 5")


#   ============================ VIEW_TASKS FUNCTION ============================
def view_tasks():
    # Check if there are no tasks in the list
    if not tasks:
        print("No tasks in your list")
        return  # Exit the function if the task list is empty
    # Viewing the task list
    print("YOUR TASKS:")
    # Enumerate through the tasks list and display each task with its number
    for i, task in enumerate(tasks):
        print(f"{i + 1}.{task}")


# ============================ ADD_A_TASK FUNCTION ============================
def add_task():
    while True:
        # Prompt the user to enter a new task
        task_name = input("New Task (press 0 for menu): ").strip()
        # If the user presses '0', break the loop and return to the main menu
        if task_name == "0":
            break
        # Add the new task to the tasks list
        tasks.append(task_name)
        print("Added Successfully")


# ============================ MARK_COMPLETE FUNCTION ============================
def mark_complete():
    # Check if there are no tasks to complete
    if not tasks:
        print("No tasks to complete!")
        return  # Exit the function if the task list is empty
    # Display the current tasks to the user
    view_tasks()
    while True:
        try:
            # Prompt the user to enter the number of the task they want to complete
            complete_choice = int(input("Enter the number of the task: "))
            # Check if the entered number is within the valid range
            if 1 <= complete_choice <= len(tasks):
                # Remove the selected task from tasks and add it to completed
                item_completed = tasks.pop(
                    complete_choice - 1
                )  # stored the removed task in a varible
                completed.append(item_completed)
                print(f"'{item_completed}' Marked as completed")
                break  # Exit the loop after completing a task
            else:
                # Inform the user if their input is out of range
                print(f"Please enter a number between 1 and {len(tasks)}")
        except ValueError:
            # Handle invalid input (non-integer)
            print("Please enter a valid choice!")

    # if the user finished all of his tasks --> engourage him with a proper message
    # if there are remaining tasks --> display it to him
    if tasks:
        view_tasks()
    else:
        print("All tasks completed!")


# ============================ SHOW_COMPLETED FUNCTION ============================
def show_completed():
    # Check if there are no completed tasks to show
    if not completed:
        print("There are no completed tasks!")
        return  # Exit the function if the completed list is empty
    # Display the list of completed tasks
    print("COMPLETED TASKS:")
    for i, task in enumerate(completed):
        print(f"{i + 1}.{task} ✔")

if __name__ == "__main__":
    main() # executing the actual code 
