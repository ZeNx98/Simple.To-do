/*
ToDoList.c - Simple Command-Line To-Do List Manager in C

This program allows users to manage a to-do list from the terminal.
Features include adding, viewing, editing, deleting, and marking tasks as done.
Tasks are stored in a singly linked list and persisted to 'tasks.txt'.

Features:
- Add, view, edit, and delete tasks
- Mark tasks as done
- Delete all completed tasks
- Save/load tasks to/from a file
- (Search and sort stubs included)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Task
{
    int id;
    bool done;
    char name[50];
    char description[200];
    struct Task *next;
};
typedef struct Task task;

void NewTask(task **head);
void ViewTask(task *head);
void DeleteTask(task **head);
void EditTask(task *head);
void SaveTask(task *head);
void LoadTask(task **head);
void deleteDoneTasks(task **head);
void freeAllTasks(task *head);
void SearchTask(task *head); // stub
void SortTask(task **head);  // stub

int main()
{
    task *head = NULL;
    int choice;
    LoadTask(&head); // Load tasks from file at the start

    do
    {
        printf("\nTo-Do List Menu:\n");
        printf("1. New Task\n");
        printf("2. View Tasks\n");
        printf("3. Edit Task\n");
        printf("4. Delete Task\n");
        printf("5. Search Task\n");
        printf("6. Sort Tasks\n");
        printf("7. Save Tasks\n");
        printf("8. Load Tasks\n");
        printf("9. Delete Done Tasks\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice)
        {
            case 1:
                NewTask(&head);
                break;
            case 2:
                ViewTask(head);
                break;
            case 3:
                EditTask(head);
                break;
            case 4:
                DeleteTask(&head);
                break;
            case 5:
                SearchTask(head);
                break;
            case 6:
                SortTask(&head);
                break;
            case 7:
                SaveTask(head);
                break;
            case 8:
                LoadTask(&head);
                break;
            case 9:
                deleteDoneTasks(&head);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);

    freeAllTasks(head);
    return 0;
}

void NewTask(task **head)
{
    task *newTask = (task *)malloc(sizeof(task));
    if (newTask == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Find max id
    int max_id = 0;
    task *curr = *head;
    while (curr) {
        if (curr->id > max_id) max_id = curr->id;
        curr = curr->next;
    }
    newTask->id = max_id + 1;
    newTask->done = false;

    printf("Enter task name: ");
    fgets(newTask->name, sizeof(newTask->name), stdin);
    newTask->name[strcspn(newTask->name, "\n")] = 0;

    printf("Enter task description: ");
    fgets(newTask->description, sizeof(newTask->description), stdin);
    newTask->description[strcspn(newTask->description, "\n")] = 0;

    newTask->next = NULL;

    // Append to end
    if (*head == NULL) {
        *head = newTask;
    } else {
        curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = newTask;
    }

    printf("Task added successfully!\n");
    SaveTask(*head);
}

void ViewTask(task *head)
{
    if (head == NULL)
    {
        printf("No tasks available.\n");
        return;
    }

    task *current = head;
    while (current != NULL)
    {
        printf("ID: %d\n Name: %s\n Description: %s\n Done: %s\n", current->id, current->name, current->description, current->done ? "Yes" : "No");
        current = current->next;
    }
}

void DeleteTask(task **head)
{
    if (*head == NULL)
    {
        printf("No tasks available.\n");
        return;
    }

    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);
    getchar();

    task *current = *head;
    task *prev = NULL;

    while (current != NULL && current->id != id)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Task with ID %d not found.\n", id);
        return;
    }

    if (prev == NULL)
    {
        // Deleting the head node
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    free(current);
    printf("Task deleted successfully!\n");
    SaveTask(*head);
}

void EditTask(task *head)
{
    if (head == NULL)
    {
        printf("No tasks available.\n");
        return;
    }

    int id;
    printf("Enter task ID to edit: ");
    scanf("%d", &id);
    getchar();

    task *current = head;

    while (current != NULL && current->id != id)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Task with ID %d not found.\n", id);
        return;
    }

    printf("Enter new task name: ");
    fgets(current->name, sizeof(current->name), stdin);
    current->name[strcspn(current->name, "\n")] = 0;

    printf("Enter new task description: ");
    fgets(current->description, sizeof(current->description), stdin);
    current->description[strcspn(current->description, "\n")] = 0;

    printf("Is the task done? (1 for Yes, 0 for No): ");
    scanf("%d", (int*)&current->done);
    getchar();

    printf("Task updated successfully!\n");
    SaveTask(head);
}

void SaveTask(task *head)
{
    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    task *current = head;
    while (current != NULL)
    {
        fprintf(file, "%d|%d|%s|%s\n", current->id, current->done, current->name, current->description);
        current = current->next;
    }

    fclose(file);
    printf("Tasks saved successfully!\n");
}

void LoadTask(task **head)
{
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL)
    {
        // No file to load, not an error
        return;
    }

    freeAllTasks(*head);
    *head = NULL;

    char line[300];
    while (fgets(line, sizeof(line), file))
    {
        task *newTask = (task *)malloc(sizeof(task));
        if (newTask == NULL)
        {
            printf("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        newTask->next = NULL;

        // Parse line
        char *token = strtok(line, "|");
        if (!token) continue;
        newTask->id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        newTask->done = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(newTask->name, token, sizeof(newTask->name));
        newTask->name[sizeof(newTask->name)-1] = 0;

        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(newTask->description, token, sizeof(newTask->description));
        newTask->description[sizeof(newTask->description)-1] = 0;

        // Append to end
        if (*head == NULL) {
            *head = newTask;
        } else {
            task *curr = *head;
            while (curr->next) curr = curr->next;
            curr->next = newTask;
        }
    }

    fclose(file);
    printf("Tasks loaded successfully!\n");
}

void freeAllTasks(task *head)
{
    task *current = head;
    while (current != NULL)
    {
        task *temp = current;
        current = current->next;
        free(temp);
    }
}

void deleteDoneTasks(task **head)
{
    task *current = *head;
    task *prev = NULL;

    while (current != NULL) {
        if (current->done) {
            task *toDelete = current;
            if (prev == NULL) {
                *head = current->next;
                current = *head;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(toDelete);
        } else {
            prev = current;
            current = current->next;
        }
    }
    SaveTask(*head);
    printf("All done tasks deleted.\n");
}

// Stub for SearchTask
void SearchTask(task *head) {
    printf("SearchTask not implemented yet.\n");
}

// Stub for SortTask
void SortTask(task **head) {
    printf("SortTask not implemented yet.\n");
}
