#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct Student
{
    int regno;
    char name[50];
    char dept[30];
    int marks;
} Student;

struct node
{
    Student data;
    struct node *left;
    struct node *right;
};

typedef struct node *Tree;

int queue[MAX];
int front = 0;
int rear = -1;
int count = 0;



Tree Search(Tree root, int regno)
{
    if (root == NULL || root->data.regno == regno)
        return root;

    if (regno < root->data.regno)
        return Search(root->left, regno);

    return Search(root->right, regno);
}



Tree Insert(Tree root, Student s)
{
    if (root == NULL)
    {
        Tree temp = (Tree)malloc(sizeof(struct node));

        if (temp == NULL)
        {
            printf("Memory allocation failed\n");
            return NULL;
        }

        temp->data = s;
        temp->left = NULL;
        temp->right = NULL;

        return temp;
    }

    if (s.regno < root->data.regno)
        root->left = Insert(root->left, s);

    else if (s.regno > root->data.regno)
        root->right = Insert(root->right, s);

    else
        printf("Duplicate register number\n");

    return root;
}



void Display(Tree root)
{
    if (root != NULL)
    {
        Display(root->left);

        printf("\nRegister Number : %d", root->data.regno);
        printf("\nName            : %s", root->data.name);
        printf("\nDepartment      : %s", root->data.dept);
        printf("\nMarks           : %d\n", root->data.marks);

        Display(root->right);
    }
}



Tree FindMin(Tree root)
{
    while (root != NULL && root->left != NULL)
        root = root->left;

    return root;
}



Tree Delete(Tree root, int regno)
{
    Tree temp;

    if (root == NULL)
    {
        printf("Student not found\n");
        return NULL;
    }

    if (regno < root->data.regno)
        root->left = Delete(root->left, regno);

    else if (regno > root->data.regno)
        root->right = Delete(root->right, regno);

    else
    {
        if (root->left == NULL)
        {
            temp = root->right;
            free(root);
            return temp;
        }

        else if (root->right == NULL)
        {
            temp = root->left;
            free(root);
            return temp;
        }

        temp = FindMin(root->right);
        root->data = temp->data;
        root->right = Delete(root->right, temp->data.regno);
    }

    return root;
}



void Enqueue(int regno)
{
    if (count == MAX)
    {
        printf("Queue is full\n");
        return;
    }

    rear = (rear + 1) % MAX;
    queue[rear] = regno;
    count++;

    printf("Student added to processing queue\n");
}



void DeQueue(Tree root)
{
    if (count == 0)
    {
        printf("Queue is empty\n");
        return;
    }

    int regno = queue[front];
    front = (front + 1) % MAX;
    count--;

    Tree temp = Search(root, regno);

    if (temp != NULL)
    {
        printf("Processing student: %s\n", temp->data.name);
        printf("Register Number: %d\n", temp->data.regno);
    }
    else
    {
        printf("Student record not found\n");
    }
}



void DisplayQueue(void)
{
    int i, pos;

    if (count == 0)
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Students waiting for processing:\n");

    for (i = 0; i < count; i++)
    {
        pos = (front + i) % MAX;
        printf("%d\n", queue[pos]);
    }
}



int CountStudents(Tree root)
{
    if (root == NULL)
        return 0;

    return 1 + CountStudents(root->left)
             + CountStudents(root->right);
}



void CopyRecords(Tree root, Student a[], int *n)
{
    if (root != NULL)
    {
        CopyRecords(root->left, a, n);
        a[*n] = root->data;
        (*n)++;
        CopyRecords(root->right, a, n);
    }
}



void Sort(Tree root)
{
    Student a[MAX], temp;
    int n, i, j;

    n = 0;
    CopyRecords(root, a, &n);

    if (n == 0)
    {
        printf("No student records available\n");
        return;
    }

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (a[j].marks > a[j + 1].marks)
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    printf("\nStudents sorted by marks:\n");

    for (i = 0; i < n; i++)
    {
        printf("\nRegister Number: %d", a[i].regno);
        printf("\nName: %s", a[i].name);
        printf("\nMarks: %d\n", a[i].marks);
    }
}


int main()
{
    Tree root = NULL;
    Tree temp;

    Student s;
    int choice, regno;

    do
    {
        printf("\n===== SMART STUDENT RECORD SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Sort Students by Marks\n");
        printf("6. Add Student to Queue\n");
        printf("7. Process Next Student\n");
        printf("8. Display Waiting Queue\n");
        printf("9. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("Enter Register Number: ");
                scanf("%d", &s.regno);

                if (s.regno <= 0)
                {
                    printf("Invalid register number\n");
                    break;
                }

                if (Search(root, s.regno) != NULL)
                {
                    printf("Duplicate register number\n");
                    break;
                }

                printf("Enter Student Name: ");
                scanf(" %49[^\n]", s.name);

                printf("Enter Department: ");
                scanf(" %29[^\n]", s.dept);

                printf("Enter Marks (0-100): ");
                scanf("%d", &s.marks);

                if (s.marks < 0 || s.marks > 100)
                {
                    printf("Invalid marks\n");
                    break;
                }

                root = Insert(root, s);
                printf("Student added successfully\n");
                break;

            case 2:
                if (root == NULL)
                    printf("No student records available\n");
                else
                    Display(root);
                break;

            case 3:
                printf("Enter Register Number to search: ");
                scanf("%d", &regno);

                temp = Search(root, regno);

                if (temp == NULL)
                {
                    printf("Student not found\n");
                }
                else
                {
                    printf("\nStudent Found\n");
                    printf("Register Number: %d\n", temp->data.regno);
                    printf("Name: %s\n", temp->data.name);
                    printf("Department: %s\n", temp->data.dept);
                    printf("Marks: %d\n", temp->data.marks);
                }
                break;

            case 4:
                printf("Enter Register Number to delete: ");
                scanf("%d", &regno);

                if (Search(root, regno) == NULL)
                {
                    printf("Student not found\n");
                }
                else
                {
                    root = Delete(root, regno);
                    printf("Student deleted successfully\n");
                }
                break;

            case 5:
                Sort(root);
                break;

            case 6:
                printf("Enter Register Number: ");
                scanf("%d", &regno);

                if (Search(root, regno) == NULL)
                    printf("Student not found\n");
                else
                    Enqueue(regno);
                break;

            case 7:
                DeQueue(root);
                break;

            case 8:
                DisplayQueue();
                break;

            case 9:
                printf("Thank you!\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 9);

    return 0;
}