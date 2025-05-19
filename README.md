# To-Do List Manager

## Overview
This project is a simple command-line to-do list manager implemented in C. It allows users to manage their tasks efficiently through a terminal interface.

## Features
- **Add Tasks**: Create new tasks with a name and description.
- **View Tasks**: Display all tasks with their details, including whether they are completed.
- **Edit Tasks**: Modify existing tasks, including changing their name, description, and completion status.
- **Delete Tasks**: Remove specific tasks from the list.
- **Mark Tasks as Done**: Update the status of tasks to indicate completion.
- **Delete All Completed Tasks**: Remove all tasks that have been marked as done.
- **Save and Load Tasks**: Persist tasks to a file (`tasks.txt`) for future sessions.

## Usage
1. Compile the program using a C compiler.
2. Run the executable from the terminal.
3. Follow the on-screen menu to manage your tasks.

## Compilation
To compile the program, use the following command:
```
gcc ToDoList.c -o ToDoList
```

## Running the Program
After compilation, run the program with:
```
./ToDoList
```

## File Structure
- `ToDoList.c`: Contains the implementation of the to-do list manager.
- `tasks.txt`: (Automatically created) Stores the tasks persistently.

## License
This project is open-source and available for modification and distribution.
