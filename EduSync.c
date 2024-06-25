#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <unistd.h>
#include "election.h"
#define CS 0
#define EC 1
#define EE 2
#define ME 3
#define CE 4
#define MM 5
#define MAX_PASSWORD_LENGTH 21

// global declaration.
FILE *file, *inFile;
int count, number_of_students_in_each_branch[] = {0, 0, 0, 0, 0, 0}, no_subjects;
bool if_data_present = false;

/* 
--> Value of -2 indicates that student has not registered for that course.
--> Value of -1 indicates that student has registered and there is no update from the repective faculty.
*/

struct subject
{
    char subjectname[100];
    int credits;
} *sub;

struct student
{
    char name[40];
    int marks[10];
    char roll_no[10];
    int sum;
    float cgpa;
    struct branch
    {
        char branch_code[5];
    } student_branch;
} *students;

struct attendance
{
    int attend[10];
    char roll_no[10];
    // int sum;
    // float cgpa;
} *students_attendance;
   
// animation for exiting the programme.
void exitproject()
{
    system("cls");

    // clears the memory assossiated with the pointers.
    free(sub);
    free(students);
    free(students_attendance);

    PlaySound(TEXT("ending.wav"), NULL, SND_FILENAME | SND_ASYNC);
    Sleep(2000);

    char ThankYou[] = " ========= Thank You =========\n";
    char SeeYouSoon[] = " ======== See You Soon =======\n";
    // for creating the animation.
    for (int i = 0; i < strlen(ThankYou); i++)
    {
        printf("%c", ThankYou[i]);
        Sleep(40);
    }
    for (int i = 0; i < strlen(SeeYouSoon); i++)
    {
        printf("%c", SeeYouSoon[i]);
        Sleep(40);
    }
    Sleep(2000);
    system("cls");
    exit(0);
}

// function to check if its a valid integer.
int is_digit()
{
    int a;
    scanf("%d",&a);
    d:
    if(a>0 && a<11){
        return a;
    }
    else
    {
        printf("Invalid Input.\n");
        printf("Enter again: ");
        fflush(stdin);
        scanf("%d",&a);
        goto d;
    }
}

// function for navigating inside the programme.
void choiceFunc()
{
    int temp;
f:
    printf("\n==============================\n");
    printf("||          1: Back         ||\n");
    printf("||          2: Exit         ||\n");
    printf("==============================\n");
    printf("Enter the choice: ");
    temp = is_digit();
    switch (temp)
    {
    case 1:
        system("cls");
        break;
    case 2:
        exitproject();
    default:
        printf("Wrong Choice.\n");
        goto f;
    }
}

// function to view and store results.
void view(int i, bool b)
{
    // the boollean value determines whether the function should output the result, else it just retrives the info and store it.
    char ch;
    int l = 0;
    if (b)
        printf("\n\n");
    file = fopen("credits.txt", "r");
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch != ' ')
            sub[l++].credits = ch - '0';
    }
    fclose(file);

    int totalmarks = 0, temp = 0;
    float cg = 0, totalcg = 0;
    bool a = 1;
    for (int j = 0; j < no_subjects; j++)
    {
        if (students[i].marks[j] != -2)
        {
            if (students[i].marks[j] == -1)
            {
                temp++;
                a = 0;
                if (b)
                    printf("%s : NA\n", sub[j].subjectname);
            }
            else
            {
                temp++;
                {
                    switch (students[i].marks[j] / 10) // assigns respective grates according to marks.
                    {
                    case 10:
                    case 9:
                        if (b)
                            printf("%s : EX\n", sub[j].subjectname);
                        cg = cg + (10 * sub[j].credits);
                        break;
                    case 8:
                        if (b)
                            printf("%s : A\n", sub[j].subjectname);
                        cg = cg + (9 * sub[j].credits);
                        break;
                    case 7:
                        if (b)
                            printf("%s : B\n", sub[j].subjectname);
                        cg = cg + (8 * sub[j].credits);
                        break;
                    case 6:
                        if (b)
                            printf("%s : C\n", sub[j].subjectname);
                        cg = cg + (7 * sub[j].credits);
                        break;
                    case 5:
                        if (b)
                            printf("%s : D\n", sub[j].subjectname);
                        cg = cg + (6 * sub[j].credits);
                        break;
                    case 4:
                        if (b)
                            printf("%s : P\n", sub[j].subjectname);
                        cg = cg + (5 * sub[j].credits);
                        break;
                    default:
                        if (b)
                            printf("%s : F\n", sub[j].subjectname);
                        cg = cg + (0 * sub[j].credits);
                        break;
                    }
                }
                totalmarks += students[i].marks[j]; // calculates the total marks.
                totalcg += (10.0 * sub[j].credits);
            }
        }
    }
    students[i].sum = totalmarks;
    if (a && temp>=4)
    {
        students[i].cgpa = cg * 10.0 / totalcg; // calculates the cgpa of student.
        if (b)
            printf("\nYour final CGPA is: %.02f\n", students[i].cgpa);
    }
    else
    {
        if (b)
        {
            printf("\nYour results have not been updated yet. Please wait...(Press enter to continue)\n");
            getch();
            system("cls");
        }
        students[i].cgpa = -1; // this will be default value of cgpa if required conditions are not satisfied.
    }
}

// function to retrive details about each students from student.csv. 
void retrieve_data_of_each_student(int i, char *buf)
{

    bool a = 0;
    char *score;
    score = (char *)malloc(5);
    int temp = 0, k = 0, l = 0;
    int totalmarks = 0;
    for (int j = 0;; j++)
    {
        if (buf[j] == ',')
        {
            temp++; // helps in determining and segregating required informations.
            k = 0;
            continue;
        }
        switch (temp)
        {
        case 0:
            students[i].name[k++] = buf[j];
            break;
        case 1:
            students[i].student_branch.branch_code[k++] = buf[j];
            break;
        case 2:
            students[i].roll_no[k++] = buf[j];
            break;
        default:
            while (true) // goes till the end and stores the student's marks.
            {
                if (buf[j] != '=')
                {
                    if (buf[j] != ',')
                        score[k++] = buf[j++];
                    else
                    {
                        students[i].marks[l] = atoi(score); //converts the character array to a number.
                        if (students[i].marks[l] > -1)
                            totalmarks += students[i].marks[l];
                        l++;
                        free(score);
                        k = 0;
                        j++;
                        score = (char *)malloc(5);
                    }
                }
                else
                {
                    a = 1;
                    break;
                }
            }
        }
        if (a)
            break;
    }
    students[i].sum = totalmarks;
    free(score);
    if (strcmp(students[i].student_branch.branch_code, "CS") == 0)
        number_of_students_in_each_branch[CS]++;
    else if (strcmp(students[i].student_branch.branch_code, "EC") == 0)
        number_of_students_in_each_branch[EC]++;
    else if (strcmp(students[i].student_branch.branch_code, "EE") == 0)
        number_of_students_in_each_branch[EE]++;
    else if (strcmp(students[i].student_branch.branch_code, "ME") == 0)
        number_of_students_in_each_branch[ME]++;
    else if (strcmp(students[i].student_branch.branch_code, "CE") == 0)
        number_of_students_in_each_branch[CE]++;
    else if (strcmp(students[i].student_branch.branch_code, "MM") == 0)
        number_of_students_in_each_branch[MM]++;
}

// function to fetch number of subjects and students.
void retrieve_info()
{
    char ch, *buf;
    buf = (char *)malloc(1000);
    int temp = 0, i = 0, j = 0, temp2 = 3, temp3;
    count = 1;
    no_subjects = -3;
    file = fopen("student.csv", "r");
    inFile = fopen("attendance.csv", "r");

    if (file == NULL)
    {
        perror("Error opening file.\n");
        exitproject();
    }

    // counts the number of subjects and students.
    while ((ch = fgetc(file)) != EOF) 
    {
        if (ch == '\n')
            count++;
        if (count == 1)
            if (ch == ',')
                no_subjects++;
    }

    // allocate memory for subject names.
    sub = calloc(no_subjects, sizeof(struct student));
    // allocate memory for the required students.
    students = calloc(count - 1, sizeof(struct student));
    // allocate memory for the attendance of students.
    students_attendance = calloc(count - 1, sizeof(struct attendance));

    temp3 = no_subjects + 1;
    fclose(inFile);
    fclose(file);

    file = fopen("student.csv", "r");
    inFile = fopen("attendance.csv", "r");
    // passes each line to other function to get student details.
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            temp++;
            i = 0;
        }
        if (temp > 1 && ch == '\n')
        {
            retrieve_data_of_each_student(temp - 1, buf);
            free(buf);
            buf = (char *)malloc(1000);
        }
        if (temp)
        {
            if (ch != '\n')
                buf[i++] = ch;
        }
        else
        {
            if (ch == ',' && temp2)
                temp2--;
            if (!temp2)
            {
                if (ch == ',' && temp3--)
                {
                    i++;
                    j = 0;
                    continue;
                }
                else
                {
                    if (i > no_subjects)
                        continue;
                    sub[i - 1].subjectname[j++] = ch;
                }
            }
        }
    }

    fclose(file);
    for (int i = 1; i < count - 1; i++)
        view(i, false); // stores the student's current result.

    printf("\n");
}

// function to retrive details about each students from attendance.csv. 
void retrieve_attendance_of_each_student(int i, char *buf)
{

    bool a = 0;
    char *score;
    score = (char *)malloc(5);
    int temp = 0, k = 0, l = 0;
    strcpy(students_attendance[i].roll_no, students[i].roll_no);

    for (int j = 0;; j++)
    {
        if (buf[j] == ',')
        {
            temp++; // helps in determining and segregating required informations.
            k = 0;
            continue;
        }
        if (temp)
        {
            while (true) // goes till the end and stores the student's attendance.
            {
                if (buf[j] != '=')
                {
                    if (buf[j] != ',')
                        score[k++] = buf[j++];
                    else
                    {
                        students_attendance[i].attend[l] = atoi(score); //converts the character array to a number.
                        l++;
                        free(score);
                        k = 0;
                        j++;
                        score = (char *)malloc(5);
                    }
                }
                else
                {
                    a = 1;
                    break;
                }
            }
        }
        if (a)
            break;
    }
    free(score);
}

// function to fetch the details of attendance.
void retrieve_attendance()
{
    char ch, *buf;
    buf = (char *)malloc(1000);
    int temp = 0, i = 0, j = 0, temp2 = 1, temp3;
    file = fopen("attendance.csv", "r");
    if (file == NULL)
    {
        perror("Error opening file.\n");
        exitproject();
    }

    // allocate memory for the required students.
    students_attendance = calloc(count - 1, sizeof(struct attendance));

    temp3 = no_subjects + 1;
    // passes each line to other function to get student's attendance details.
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            temp++;
            i = 0;
        }
        if (temp > 1 && ch == '\n')
        {
            retrieve_attendance_of_each_student(temp - 1, buf);
            free(buf);
            buf = (char *)malloc(1000);
        }
        if (temp)
        {
            if (ch != '\n')
                buf[i++] = ch;
        }
    }
    fclose(file);
}

// function to create headerfile.
void createheader()
{
    file = fopen("student.csv", "w");
    FILE *temp;
    temp = fopen("credits.txt", "w");
    inFile = fopen("attendance.csv", "w");
    FILE *f1, *f2;
    f1 = fopen("Faculty.txt", "a");
    f2 = fopen("FacultyAdvisor.txt", "a");

    if_data_present = false;
    system("cls");
    printf("\nThe Database has not yet been created. PLease enter the following details to create the database.(Press enter to continue)\n");
    getch();
    // taking number of subjects
    printf("\n\nNumber of subjects: ");
    scanf("%d", &no_subjects);

    sub = calloc(no_subjects, sizeof(struct student));

    // making headers
    fprintf(file, "Name,Branch,Roll Number,");
    fprintf(inFile, "Roll Number,");

    for (int i = 0; i < no_subjects; i++)
    {
        system("cls");
        char faculty_id[50];
        // gets all the details regarding subject.
        printf("Enter the subject no. %d: ", i + 1);
        scanf("%s", sub[i].subjectname);
        fprintf(file, "%s,", sub[i].subjectname);
        fprintf(inFile, "%s,", sub[i].subjectname);
        printf("Enter the number of credits of %s: ", sub[i].subjectname);
        scanf("%d", &sub[i].credits);
        fprintf(temp, "%d ", sub[i].credits);
        printf("Enter the faculty of %s: ", sub[i].subjectname);
        // inputs the faculty for the respective subject.
        scanf("%s", faculty_id);
        fprintf(f1, "%s %s@iitbbs %s \n", faculty_id, faculty_id, sub[i].subjectname);
    t:
        printf("Do you wish to add %s as a faculty advisor for any branch?\nEnter 1:Yes 2:No\n", faculty_id);
        int a;
        a = is_digit();
        switch (a)
        {
        case 1:
        v:
            printf("Enter the branch code: \n");
            char branch[5];
            scanf("%s", branch);
            // checks if it's a valid branch.
            if (strcmp(branch, "CS") == 0 || strcmp(branch, "EC") == 0 || strcmp(branch, "EE") == 0 || strcmp(branch, "ME") == 0 || strcmp(branch, "CE") == 0 || strcmp(branch, "MM") == 0)
            {
                fprintf(f2, "%s %s \n", faculty_id, branch);
            }
            else
            {
                printf("No such branch exist.\n");
                choiceFunc();
                goto v;
            }
            break;
        case 2:
            break;
        default:
            printf("Wrong Choice.\n");
            choiceFunc();
            goto t;
        }
    }
    fprintf(file, "Total Marks\n");
    fprintf(inFile, "Attendance\n");
    count = 2;

    system("cls");
    printf("\nNow the database is ready.Thank you for the information.(Press enter to continue)\n");
    getch();
    fclose(file);
    fclose(temp);
    fclose(inFile);
    fclose(f1);
    fclose(f2);
}

// function to check whether the file has an appropriate heading.
void headercheck()
{

    file = fopen("student.csv", "r");
    char *check;
    check = (char *)calloc(10, sizeof(char));
    // checks if file is empty.
    if (file == NULL)
    {
        createheader();
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            check[i] = fgetc(file);
            if (check[i] == EOF)
            {
                createheader();
                return;
                break;
            }
        }
        fclose(file);
        // compares if the first 4 char is Name or not.
        if (strcmp(check, "Name"))
        {
            printf("%s\n", check);
            createheader();
        }
        else
        {
            if_data_present = true;
            // retrieves all the present information.
            retrieve_info();
            retrieve_attendance();
        }
    }
    free(check);
}

// generate roll number.
void generate_roll_no(int i)
{
    // generates roll number for CS branch
    if (strcmp(students[i].student_branch.branch_code, "CS") == 0)
    {
        number_of_students_in_each_branch[CS]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[CS];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'C' && students[j].roll_no[3] == 'S'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23CS0");
            else
                strcpy(students[i].roll_no, "23CS");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[CS] <= 9)
                strcpy(students[i].roll_no, "23CS0");
            else
                strcpy(students[i].roll_no, "23CS");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[CS]);
        }
    }

    // generates roll number for EC branch
    else if (strcmp(students[i].student_branch.branch_code, "EC") == 0)
    {
        number_of_students_in_each_branch[EC]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[EC];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'E' && students[j].roll_no[3] == 'C'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23EC0");
            else
                strcpy(students[i].roll_no, "23EC");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[EC] <= 9)
                strcpy(students[i].roll_no, "23EC0");
            else
                strcpy(students[i].roll_no, "23EC");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[EC]);
        }
    }

    // generates roll number for EE branch
    else if (strcmp(students[i].student_branch.branch_code, "EE") == 0)
    {
        number_of_students_in_each_branch[EE]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[EE];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'E' && students[j].roll_no[3] == 'E'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23EE0");
            else
                strcpy(students[i].roll_no, "23EE");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[EE] <= 9)
                strcpy(students[i].roll_no, "23EE0");
            else
                strcpy(students[i].roll_no, "23EE");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[EE]);
        }
    }

    // generates roll number for ME branch
    else if (strcmp(students[i].student_branch.branch_code, "ME") == 0)
    {
        number_of_students_in_each_branch[ME]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[ME];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'M' && students[j].roll_no[3] == 'E'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23ME0");
            else
                strcpy(students[i].roll_no, "23ME");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[ME] <= 9)
                strcpy(students[i].roll_no, "23ME0");
            else
                strcpy(students[i].roll_no, "23ME");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[ME]);
        }
    }

    // generates roll number for CE branch
    else if (strcmp(students[i].student_branch.branch_code, "CE") == 0)
    {
        number_of_students_in_each_branch[CE]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[CE];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'C' && students[j].roll_no[3] == 'E'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23CE0");
            else
                strcpy(students[i].roll_no, "23CE");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[CE] <= 9)
                strcpy(students[i].roll_no, "23CE0");
            else
                strcpy(students[i].roll_no, "23CE");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[CE]);
        }
    }

    // generates roll number for MM branch
    else if (strcmp(students[i].student_branch.branch_code, "MM") == 0)
    {
        number_of_students_in_each_branch[MM]++;
        if (if_data_present)
        {
            int cnt = number_of_students_in_each_branch[MM];
            for (int j = count - 2; j > 0; j--)
            {
                if ((students[j].roll_no[2] == 'M' && students[j].roll_no[3] == 'M'))
                {
                    cnt = (students[j].roll_no[4] - '0') * 10 + students[j].roll_no[5] - '0' + 1; //gets the info of last roll number.
                    break;
                }
            }
            if (cnt <= 9)
                strcpy(students[i].roll_no, "23MM0");
            else
                strcpy(students[i].roll_no, "23MM");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", cnt);
        }
        else
        {
            if (number_of_students_in_each_branch[MM] <= 9)
                strcpy(students[i].roll_no, "23MM0");
            else
                strcpy(students[i].roll_no, "23MM");
            snprintf(students[i].roll_no + strlen(students[i].roll_no), sizeof(students[i].roll_no) - strlen(students[i].roll_no), "%d", number_of_students_in_each_branch[MM]);
        }
    }
}

// reprints the content of students.csv.
void edit()
{
    file = fopen("student.csv", "w");
    fprintf(file, "Name,Branch,Roll Number,");
    for (int k = 0; k < no_subjects; k++)
        fprintf(file, "%s,", sub[k].subjectname);
    fprintf(file, "Total Marks\n");
    for (int j = 1; j < count - 1; j++)
    {
        fprintf(file, "%s,", students[j].name);
        fprintf(file, "%s,", students[j].student_branch.branch_code);
        fprintf(file, "%s,", students[j].roll_no);
        for (int k = 0; k < no_subjects; k++)
            fprintf(file, "%d,", students[j].marks[k]);
        fprintf(file, "=%d", students[j].sum);
        fprintf(file, "\n");
    }
    fclose(file);
}

// reprints the content of attendance.csv.
void edit_attendance()
{
    inFile = fopen("attendance.csv", "w");
    fprintf(inFile, "Roll Number,");
    for (int k = 0; k < no_subjects; k++)
        fprintf(inFile, "%s,", sub[k].subjectname);
    fprintf(inFile, "Attendance\n");
    for (int j = 1; j < count - 1; j++)
    {
        fprintf(inFile, "%s,", students[j].roll_no);
        for (int k = 0; k < no_subjects; k++)
            fprintf(inFile, "%d,", students_attendance[j].attend[k]);
        fprintf(inFile, "=0\n");
    }
    fclose(inFile);
}

// adding new students into the database.
void add_students()
{
    file = fopen("student.csv", "a");
    inFile = fopen("attendance.csv", "a");
    if (inFile == NULL || file == NULL)
    {
        perror("Error opening file.\n");
        return;
    }

    int n, totalmarks = 0, choice2, choice3, newmark;
    char roll[10], new[20];

    if (if_data_present)
    {
        printf("\nEnter the details of student: ");
        n = 1;
        students = realloc(students, (n + count - 2) * sizeof(struct student)); // increase the required memory.
        students_attendance = realloc(students_attendance, (n + count - 2) * sizeof(struct attendance));
    }
    else
    {
        printf("\nNumber of students to be added: ");
        scanf("%d", &n);
        students = calloc(n, sizeof(struct student)); // initialise the required memory.
        students_attendance = calloc(n, sizeof(struct attendance));
    }

    // takes students details of new student.
    for (int i = count - 2; i < count + n - 2; i++)
    {
        printf("\n\nEnter the name of student number %d: ", i + 1);
        scanf("%s", students[i].name);
        fprintf(file, "%s,", students[i].name);
    // getting the branch of the student
    s:
        printf("============================================================================\n");
        printf("           The available branches in the college are as follows:            \n");
        printf("============================================================================\n");
        printf("||          Computer science and engineering  branch code: CS             ||\n");
        printf("||          Electronics and communication engineering  branch code: EC    ||\n");
        printf("||          Electrical engineering  branch code: EE                       ||\n");
        printf("||          Mechanical engineering  branch code: ME                       ||\n");
        printf("||          Civil engineering  branch code: CE                            ||\n");
        printf("||          Metallurgical and materials engineering  branch code: MM      ||\n");
        printf("============================================================================\n\n");
        printf("\nEnter the branch of the student number %d: ", i + 1);
        scanf("%s", students[i].student_branch.branch_code);
        if (strcmp(students[i].student_branch.branch_code, "CS") && strcmp(students[i].student_branch.branch_code, "EC") && strcmp(students[i].student_branch.branch_code, "EE") && strcmp(students[i].student_branch.branch_code, "ME") && strcmp(students[i].student_branch.branch_code, "CE") && strcmp(students[i].student_branch.branch_code, "MM"))
        {
            system("cls");
            printf("\nInvalid Branch Code\n");
            goto s;
        }
        fprintf(file, "%s,", students[i].student_branch.branch_code);
        //  generating the roll number
        generate_roll_no(i);
        fprintf(file, "%s,", students[i].roll_no);
        fprintf(inFile, "%s,", students[i].roll_no);
        // -2 to the marks of each subjects as default value.
        for (int j = 0; j < no_subjects; j++)
        {
            students[i].marks[j] = -2;
            students_attendance[i].attend[j] = -2;
            fprintf(file, "%d,", students[i].marks[j]);
            fprintf(inFile, "%d,", students_attendance[i].attend[j]);
            if (students[i].marks[j] > -1)
                totalmarks += students[i].marks[j];
        }
        fprintf(file, "=%d\n", totalmarks);
        fprintf(inFile, "=0\n");
        fclose(file);
        fclose(inFile);
        file = fopen("Subject.txt", "a");
        // generating password.
        fprintf(file, "%s %s@iitbbs \n", students[i].roll_no, students[i].roll_no);
        fclose(file);
        file = fopen("student.csv", "a");
        inFile = fopen("attendance.csv", "a");

        system("cls");
        printf("\n%dth student done\n\n", i + 1);
    }
    count += n;
    fclose(file);
    fclose(inFile);
    free(students_attendance);
    free(students);
    free(sub);
    memset(number_of_students_in_each_branch, 0, sizeof(number_of_students_in_each_branch)); //sets all the value in array as 0.
    retrieve_info();
    retrieve_attendance();
}

// adding attendance info of the students.
void add_attendance(char Sub[])
{

    char Roll[20];
    file = fopen("attendance.csv", "a");
    printf("Enter the roll no of the student whose attendance you wish to add.\n");
    scanf("%s", &Roll);
    for (int i = 1; i < (count - 1); i++)
    {
        for (int j = 0; j < no_subjects; j++)
        {
            if (strcmp(Sub, sub[j].subjectname) == 0 && strcmp(students[i].roll_no, Roll) == 0) //gets the appropriate student and subject.
            {
                if (students_attendance[i].attend[j] != -2)
                {
                    if (students_attendance[i].attend[j] == -1)
                    {
                        int attendances;
                        printf("Enter the Attendance: ");
                        scanf("%d", &attendances);
                        students_attendance[i].attend[j] = attendances;
                    }
                    else
                    {
                    t:
                        printf("Attendance of the student is %d.\nDo you wish to change?\n 1: Yes\t2: No\n", students_attendance[i].attend[j]);
                        int choice;
                        choice = is_digit();
                        switch (choice)
                        {
                        case 1:
                            printf("Enter the attendance: ");
                            int mark;
                            scanf("%d", &mark);
                            students_attendance[i].attend[j] = mark;
                            system("cls");
                            printf("Attendance updated successfully.\n");
                            break;
                        case 2:
                            system("cls");
                            return;
                        default:
                            printf("Wrong Choice.\n");
                            choiceFunc();
                            goto t;
                        }
                    }
                }
                else
                {
                    system("cls");
                    printf("The above student doesn't come under you or is not present.(Press enter to conitnue)\n");
                    getch();
                }
            }
        }
    }
    remove("attendance.csv");
    edit_attendance();
}

// adding new faculty into the database.
void add_faculty()
{
u:
    if (file == NULL)
    {
        printf("Error!");
        exitproject();
    }
    int a;
    char *username, *subj;
    username = (char *)malloc(100);
    subj = (char *)malloc(50);
    printf("Enter Faculty ID: ");
    scanf("%s", username);
    inFile = fopen("Faculty.txt", "r");
    char *cp, buf[1000];
    int count = 0;
    // checks if the ID is already present.
    while (1)
    {
        cp = fgets(buf, sizeof(buf), inFile);
        if (cp == NULL)
            break;
        cp = strtok(buf, " ");
        if (strcmp(buf, username) == 0)
        {
            count++;
            break;
        }
    }
    fclose(inFile);
    file = fopen("Faculty.txt", "a");
    // gets the faculty details.
    if (count == 0)
    {
        printf("Enter Subject: ");
        scanf("%s", subj);
        fprintf(file, "%s %s@iitbbs %s \n", username, username, subj);
    t:
        printf("Do you wish to add %s as faculty advisor for any branch?\nEnter 1:Yes 2:No\n", username);
        a = is_digit();
        switch (a)
        {
        case 1:
            inFile = fopen("FacultyAdvisor.txt", "a");
        v:
            printf("Enter the branch code: \n");
            char branch[5];
            scanf("%s", branch);
            // checks if its a valid branch.
            if (strcmp(branch, "CS") == 0 || strcmp(branch, "EC") == 0 || strcmp(branch, "EE") == 0 || strcmp(branch, "ME") == 0 || strcmp(branch, "CE") == 0 || strcmp(branch, "MM") == 0)
            {
                fprintf(inFile, "%s %s \n", username, branch);
                fclose(inFile);
            }
            else
            {
                printf("No such branch exists.\n");
                goto v;
            }
            break;
        case 2:
            break;
        default:
            goto t;
        }
        free(username);
        free(subj);
        fclose(file);
    }
    else
    {
        printf("ID already exists.\n");
        choiceFunc();
        goto u;
    }
}

// delete the student's password.
void delete_in_txt_file(int i)
{
    file = fopen("Subject.txt", "r");
    FILE *temp;
    temp = fopen("Temp.txt", "w");
    const char *delim = " \t";
    char *cp;
    char buf[1000];
    // prints all the info to one more temporary file except the line which needs to be deleted.
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        if (cp == NULL)
            break;

        // gets the student's roll number
        cp = strtok(buf, delim);
        if (strcmp(cp, students[i].roll_no) == 0)
            continue;
        else
        {
            fprintf(temp, "%s ", cp);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s \n", cp);
        }
    }
    fclose(file);
    fclose(temp);
    char ch;

    remove("Subject.txt");
    // clears the content of Subject.txt.
    file = fopen("Subject.txt", "w");
    temp = fopen("Temp.txt", "r");

    // reprints everything back to Subject.txt.
    while ((ch = fgetc(temp)) != EOF)
    {
        fprintf(file, "%c", ch);
    }

    fclose(file);
    fclose(temp);
    // removes the temporary file.
    remove("Temp.txt");
}

// delete student's info from student.csv and attendance.csv.
void delete_students_info(int i)
{
    // clears the content of the respective files.
    remove("student.csv");
    remove("attendance.csv");

    file = fopen("student.csv", "w");
    inFile = fopen("attendance.csv", "w");

    // creates the header in the new empty file.
    fprintf(file, "Name,Branch,Roll Number,");
    fprintf(inFile, "Roll Number,");
    for (int k = 0; k < no_subjects; k++)
    {
        fprintf(file, "%s,", sub[k].subjectname);
        fprintf(inFile, "%s,", sub[k].subjectname);
    }
    fprintf(file, "Total Marks\n");
    fprintf(inFile, "Attendance\n");

    // reprints everything into the file except for that student.
    for (int j = 1; j < count - 1; j++)
    {
        if (j == i)
            continue;
        else
        {
            fprintf(file, "%s,", students[j].name);
            fprintf(file, "%s,", students[j].student_branch.branch_code);
            fprintf(file, "%s,", students[j].roll_no);
            fprintf(inFile, "%s,", students_attendance[j].roll_no);
            for (int k = 0; k < no_subjects; k++)
            {
                fprintf(file, "%d,", students[j].marks[k]);
                fprintf(inFile, "%d,", students_attendance[j].attend[k]);
            }
            fprintf(file, "=%d\n", students[j].sum);
            fprintf(inFile, "=0\n");
        }
    }

    fclose(file);
    fclose(inFile);
    delete_in_txt_file(i);  // clears the password of the student.

    free(students);
    free(sub);
    free(students_attendance);
    memset(number_of_students_in_each_branch, 0, sizeof(number_of_students_in_each_branch));

    // stores the data in the required form again by retrive functions.
    retrieve_info();
    retrieve_attendance();

    system("cls");
    printf("Deleted successfully.\n");
}

// editing the student's information.
void edit_students_info(int i)
{
    int choice;
    printf("\n===========================\n");
    printf("||    1.Change Name      ||\n");
    printf("||    2.Change Branch    ||\n");
    printf("||    3.Back             ||\n");
    printf("||    4.Exit             ||\n");
    printf("===========================\n");
    printf("Enter the choice: ");
    choice = is_digit();
    system("cls");

    switch (choice)
    {
    case 1:
        printf("Enter new name: ");
        scanf("%s", students[i].name);
        break;
    case 2:
    s:
        printf("\n============================================================================\n");
        printf("           The available branches in the college are as follows:            \n");
        printf("============================================================================\n");
        printf("||          Computer science and engineering  branch code: CS             ||\n");
        printf("||          Electronics and communication engineering  branch code: EC    ||\n");
        printf("||          Electrical engineering  branch code: EE                       ||\n");
        printf("||          Mechanical engineering  branch code: ME                       ||\n");
        printf("||          Civil engineering  branch code: CE                            ||\n");
        printf("||          Metallurgical and materials engineering  branch code: MM      ||\n");
        printf("============================================================================\n\n");
        printf("\nEnter the branch: ");
        scanf("%s", students[i].student_branch.branch_code);
        system("cls");
        // checks if it's a valid branch.
        if (strcmp(students[i].student_branch.branch_code, "CS") && strcmp(students[i].student_branch.branch_code, "EC") && strcmp(students[i].student_branch.branch_code, "EE") && strcmp(students[i].student_branch.branch_code, "ME") && strcmp(students[i].student_branch.branch_code, "CE") && strcmp(students[i].student_branch.branch_code, "MM"))
        {
            printf("\nInvalid Branch Code");
            goto s;
        }
        generate_roll_no(i);
        break;
    case 3:
        break;
    case 4:
        exitproject();
    default:
        printf("Wrong choice\n");
    }

    // storing the student data and attendance in temporary structure.
    struct student tempedit = students[i];
    struct attendance temp_attend = students_attendance[i];
    delete_students_info(i);

    file = fopen("student.csv", "a");
    inFile = fopen("attendance.csv", "a");

    // printing the edited data.
    fprintf(file, "%s,", tempedit.name);
    fprintf(file, "%s,", tempedit.student_branch.branch_code);
    fprintf(file, "%s,", tempedit.roll_no);
    fprintf(inFile, "%s,", tempedit.roll_no);
    for (int k = 0; k < no_subjects; k++)
    {
        fprintf(file, "%d,", tempedit.marks[k]);
        fprintf(inFile, "%d,", temp_attend.attend[k]);
    }
    fprintf(file, "=%d\n", tempedit.sum);
    fprintf(inFile, "=0\n");

    fclose(file);
    fclose(inFile);

    file = fopen("Subject.txt", "a");
    // generating password.
    fprintf(file, "%s %s@iitbbs \n", tempedit.roll_no, tempedit.roll_no);
    fclose(file);

    free(students);
    free(sub);
    free(students_attendance);
    memset(number_of_students_in_each_branch, 0, sizeof(number_of_students_in_each_branch));

    retrieve_info();
    retrieve_attendance();

    system("cls");
    printf("Successfully edited.\n");
}

// deletes all the faculty related details.
void delete_faculty_info()
{
    file = fopen("Faculty.txt", "r");
    FILE *temp;
    temp = fopen("Temp.txt", "w");
    const char *delim = " \t";
    char *cp;
    char buf[1000];
    char id[20];
    printf("Enter the id of the faculty you wish to delete: ");
    scanf("%s", &id);
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        if (cp == NULL)
            break;

        // get the faculty id
        cp = strtok(buf, delim);
        if (strcmp(cp, id) == 0)
            continue;
        else
        {
            fprintf(temp, "%s ", cp);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s ", cp);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s \n", cp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("Faculty.txt");
    rename("Temp.txt", "Faculty.txt");

    system("cls");
    printf("Faculty removed successfully.\n");
}

// function which allows students to register for the available courses.
void Register_Courses(int i)
{
t:
    printf("\n\n");
    int a = 0;
    for (int j = 0; j < no_subjects; j++)
    {
        if (students[i].marks[j] != -2)
        {
            printf("Registered for %s\n", sub[j].subjectname);
            a++;
        }
    }
    printf("Maximum 6 courses can be registered and register atleast for 4 to get results and CGPA.\n");
    if (a < 6)
    {
        printf("Select the courses you wish to register.\nEnter the respective choices: \n");
        int j;
        for (j = 0; j < no_subjects; j++)
        {
            printf("      %d: %s\n", j + 1, sub[j].subjectname);
        }
        int choice;
        scanf("%d", &choice);
        system("cls");
        for (int j = 1; j < no_subjects + 1; j++)
        {
            // checks and edits in the respective place.
            if (choice == j)
            {
                if (students[i].marks[choice - 1] == -2)
                {
                    students[i].marks[choice - 1] = -1;
                    students_attendance[i].attend[choice - 1] = -1;
                    printf("Registered Succesfully(press enter to continue.)\n");
                    getch();
                    edit();
                    edit_attendance();
                    break;
                }
                else
                {
                    printf("Already registered(press enter to continue.)\n");
                    getch();
                }
                break;
            }
        }
    }
    // when the maximum number of courses are registered.
    else
    {
        printf("Cannot register for any more courses(press enter to continue.)\n");
        getch();
    }
    int choice = 0;

    system("cls");
    printf("Do you wish to continue?\n    1.Yes\n    2.No\n");
    scanf("%d", &choice);
    system("cls");
    if (choice == 2)
    {
        return;
    }
    else
        goto t;
}

// function to update student's password.
void stu_update_password(int i)
{
    // deletes the existing password.
    delete_in_txt_file(i);
    char password[30];
    file = fopen("Subject.txt", "a");
    printf("\nIt is advised not to have a password more than 20 characters. It might give u problems while logging in.(Press Enter to continue)\n");
    getch();
    printf("Enter the new password: ");
    scanf("%s", password);
    // inputs the new password into the file.
    fprintf(file, "%s %s \n", students[i].roll_no, password);
    fclose(file);

    system("cls");
    printf("Password updated successfully.\n");
}

// function to update faculty's password.
void fac_update_password(char ID[])
{
    file = fopen("Faculty.txt", "r");
    FILE *temp;
    temp = fopen("Temp.txt", "w");
    const char *delim = " \t";
    char *cp;
    char buf[1000];
    char password[20];
    printf("Enter the updated password: ");
    scanf("%s", &password);

    // prints all other contents of file into a new file.
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        if (cp == NULL)
            break;

        // get the ID
        cp = strtok(buf, delim);
        if (strcmp(cp, ID) == 0)
        {
            fprintf(temp, "%s ", cp);
            fprintf(temp, "%s ", password);
            cp = strtok(NULL, delim);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s \n", cp);
        }
        else
        {
            fprintf(temp, "%s ", cp);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s ", cp);
            cp = strtok(NULL, delim);
            fprintf(temp, "%s \n", cp);
        }
    }
    fclose(file);
    fclose(temp);

    // clears the content of the function.
    file = fopen("Faculty.txt", "w");
    temp = fopen("Temp.txt", "r");
    char ch;
    // reprints everything back into the main file with the updated password.
    while ((ch = fgetc(temp)) != EOF)
    {
        fprintf(file, "%c", ch);
    }
    fclose(file);
    fclose(temp);
    remove("Temp.txt");

    system("cls");
    printf("Password updated successfully.\n");
}

// makes the result of previous election available to students.
void view_result_of_election()
{
    system("cls");
    file = fopen("result.txt", "r");
    if (file == NULL)
        printf("No elections are held until now.\n");
    else
    {
        char ch;
        while ((ch = fgetc(file)) != EOF)
            printf("%c", ch);
    }
}

// for students to apply for leave.
void apply_for_leave(int i)
{
    printf("Please enter the date in the format DD/MM/YYYY\nFor example 1/1/2024 as 01/01/2024\n(Press enter to continue)\n\n");
    getch();
    inFile = fopen("leave.txt", "a");
    char from_date[20], to_date[20], Reason[1000];
    printf("Leave from (DD/MM/YYYY): ");
    scanf("%s", from_date);
    printf("Leave until (DD/MM/YYYY): ");
    scanf("%s", to_date);
    printf("Give the reason in a compact form (not more than 1000 characters): ");
    fflush(stdin);
    gets(Reason);
    // stores the information in the required manner to show the Faculty advisor.
    fprintf(inFile, "%s %s has applied for leave from %s to %s because of \"%s\". \n", students[i].roll_no, students[i].roll_no, from_date, to_date, Reason);
    fclose(inFile);

    system("cls");
    printf("Successfully applied for leave.\n");
}

// helps the student view his previous leave records.
void view_leave_record(int i)
{
    file = fopen("leave_record.txt", "r");
    char *cp;
    char buf[1000];
    char buf2[1000];
    int count1 = 0;
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        strcpy(buf2, buf);
        if (cp == NULL)
            break;
        if (strcmp(strtok(buf, " "), students[i].roll_no) == 0)
        {
            printf("%s\n", buf + 7);
            count1++;
        }
    }
    if (count1 == 0)
        printf("No leave has been applied or processed till now.\n\n");
}

// function that provides students all his functions.
void information_stu(char RollNo[])
{
    for (int i = 1; i < count; i++)
    {
        // finds the respective student.
        if (strcmp(students[i].roll_no, RollNo) == 0)
        {
        t:
            printf("Roll No: %s\n", RollNo);
            printf("====================================================\n");
            printf("||      1: View results                           ||\n");
            printf("||      2: Register for courses                   ||\n");
            printf("||      3: Cast vote                              ||\n");
            printf("||      4: Change password                        ||\n");
            printf("||      5: View results of previous elections     ||\n");
            printf("||      6: Apply for leave                        ||\n");
            printf("||      7: View previous leave records            ||\n");
            printf("||      8: Log-Out                                ||\n");
            printf("||      9: Exit                                   ||\n");
            printf("====================================================\n\n");
            printf("Enter the choice: ");
            int choice;
            choice = is_digit();
            system("cls");
            switch (choice)
            {
            case 1:
                printf("Hello %s, Welcome to Portal\n", students[i].name);
                bool a = 1;
                view(i, true);
                choiceFunc();
                goto t;
            case 2:
                retrieve_attendance();
                Register_Courses(i);
                choiceFunc();
                goto t;
            case 3:
                studentPanel(students[i].roll_no);
                choiceFunc();
                goto t;
            case 4:
                stu_update_password(i);
                choiceFunc();
                goto t;
            case 5:
                view_result_of_election();
                choiceFunc();
                goto t;
            case 6:
                apply_for_leave(i);
                choiceFunc();
                goto t;
            case 7:
                view_leave_record(i);
                choiceFunc();
                goto t;
            case 8:
                return;
            case 9:
                exitproject();
            default:
                system("cls");
                printf("Wrong Choice");
                choiceFunc();
                goto t;
            }
        }
    }
}

// function to view all students in decreasing order of their cgpa (By admin)
void viewSortedList()
{
    int num_students = 0;
    // Here, num_students refers to the number of students whose cgpa has been updated
    for (int i = 1; i < count - 1; i++)
    {
        if (students[i].cgpa >= 0.0)
        {
            num_students++;
        }
    }
    int arr_pos[num_students];
    int count1 = 0;
    float sorted_list[num_students];
    for (int i = 1; i < count - 1; i++)
    {
        if (students[i].cgpa >= 0)
        {
            sorted_list[count1] = students[i].cgpa;
            arr_pos[count1] = i;
            count1++;
        }
    }

    float temp;
    int temp1;

    // using bubble sort to get a sorted list
    for (int i = 0; i < num_students; i++)
    {
        for (int j = 0; j < num_students - i - 1; j++)
        {
            if (sorted_list[j] < sorted_list[j + 1])
            {
                temp = sorted_list[j];
                sorted_list[j] = sorted_list[j + 1];
                sorted_list[j + 1] = temp;
                temp1 = arr_pos[j];
                arr_pos[j] = arr_pos[j + 1];
                arr_pos[j + 1] = temp1;
            }
        }
    }

    if (num_students)
    {
        printf("Students ranked in decreasing order of cgpa:\n");
        for (int i = 0; i < num_students; i++)
        {
            printf("%.2f     %s     %s\n", sorted_list[i], students[arr_pos[i]].roll_no, students[arr_pos[i]].name);
        }
    }
    else printf("cgpa has not been updated for any student\n");
}

// function to view students of a particular branch in decreasing order of their cgpa (By admin)
void viewSortedList_branch(int input)
{
    char branch[3];
    switch (input)
    {
    case 1:
        branch[0] = 'C';
        branch[1] = 'S';
        branch[2] = '\0';
        break;
    case 2:
        branch[0] = 'E';
        branch[1] = 'C';
        branch[2] = '\0';
        break;
    case 3:
        branch[0] = 'E';
        branch[1] = 'E';
        branch[2] = '\0';
        break;
    case 4:
        branch[0] = 'M';
        branch[1] = 'E';
        branch[2] = '\0';
        break;
    case 5:
        branch[0] = 'C';
        branch[1] = 'E';
        branch[2] = '\0';
        break;
    case 6:
        branch[0] = 'M';
        branch[1] = 'M';
        branch[2] = '\0';
        break;
    }
    int num_students_branch = 0;
    // Here num_students_branch refers to number of students in that particular branch whose cgpa has been updated
    for (int i = 1; i < count - 1; i++)
    {
        if (!(strcmp(students[i].student_branch.branch_code, branch)) && students[i].cgpa >= 0.0)
        {
            num_students_branch++;
        }
    }

    int arr_pos[num_students_branch];
    int count1 = 0;
    float sorted_list_branch[num_students_branch];
    for (int i = 1; i < count - 1; i++)
    {
        if (!(strcmp(students[i].student_branch.branch_code, branch)) && students[i].cgpa >= 0.0)
        {
            sorted_list_branch[count1] = students[i].cgpa;
            arr_pos[count1] = i;
            count1++;
        }
    }

    float temp;
    int temp1;

    // using bubble sort to get a sorted list
    for (int i = 0; i < num_students_branch; i++)
    {
        for (int j = 0; j < num_students_branch - i - 1; j++)
        {
            if (sorted_list_branch[j] < sorted_list_branch[j + 1])
            {
                temp = sorted_list_branch[j];
                sorted_list_branch[j] = sorted_list_branch[j + 1];
                sorted_list_branch[j + 1] = temp;
                temp1 = arr_pos[j];
                arr_pos[j] = arr_pos[j + 1];
                arr_pos[j + 1] = temp1;
            }
        }
    }

    if (num_students_branch)
    {
        printf("\nStudents ranked in decreasing order of cgpa:\n");
        for (int i = 0; i < num_students_branch; i++)
        {
            printf("%.2f     %s     %s\n", sorted_list_branch[i], students[arr_pos[i]].roll_no, students[arr_pos[i]].name);
        }
    }

    else printf("cgpa has not been updated for any student of %s branch\n", branch);
}

// function to view students of a particular subject in decreasing order of their marks.
void viewSortedList_sub(char Sub[])
{
    for (int j = 0; j < no_subjects; j++)
    {
        // To get the name of the subject the particular faculty teaches
        if (strcmp(Sub, sub[j].subjectname) == 0)
        {
            int num_students_sub = 0, total_students_sub = 0;
            // Here total_students_sub refers to the number of students who are registered for that particular subject while num_students_sub refers to the number of students whose marks have been updated
            for (int k = 1; k < count - 1; k++)
            {
                if (students[k].marks[j] >= 0)
                {
                    num_students_sub++;
                }

                if (students[k].marks[j] >= -1)
                {
                    total_students_sub++;
                }
            }

            int arr_pos[num_students_sub];
            int count1 = 0;
            int sorted_list_sub[num_students_sub];

            for (int k = 1; k < count - 1; k++)
            {
                if (students[k].marks[j] >= 0)
                {
                    sorted_list_sub[count1] = students[k].marks[j];
                    arr_pos[count1] = k;
                    count1++;
                }
            }

            int temp;
            int temp1;

            // using bubble sort for sorting in decreasing order
            for (int k = 0; k < num_students_sub; k++)
            {
                for (int l = 0; l < num_students_sub - k - 1; l++)
                {
                    if (sorted_list_sub[l] < sorted_list_sub[l + 1])
                    {
                        temp = sorted_list_sub[l];
                        sorted_list_sub[l] = sorted_list_sub[l + 1];
                        sorted_list_sub[l + 1] = temp;
                        temp1 = arr_pos[l];
                        arr_pos[l] = arr_pos[l + 1];
                        arr_pos[l + 1] = temp1;
                    }
                }
            }

            if (num_students_sub)
            {
                printf("\nStudents ranked in decreasing order of marks:\n");
                for (int k = 0; k < num_students_sub; k++)
                {
                    printf("%d     %s     %s\n", sorted_list_sub[k], students[arr_pos[k]].roll_no, students[arr_pos[k]].name);
                }
            }

            if (total_students_sub - num_students_sub)
            {
                printf("\nMarks have not been entered for the following students:\n");
                for (int k = 1; k < count - 1; k++)
                {
                    if (students[k].marks[j] == -1)
                    {
                        printf("%s    %s\n", students[k].roll_no, students[k].name);
                    }
                }
            }
        }
    }
}

// helps faculty view the students who are registered for his subject.
void facview(char Sub[])
{
    printf("Students Registered : Marks\n");
    for (int i = 1; i < (count - 1); i++)
    {
        for (int j = 0; j < no_subjects; j++)
        {
            if (strcmp(Sub, sub[j].subjectname) == 0)
            {
                if (students[i].marks[j] != -2)
                {
                    if (students[i].marks[j] == -1)
                        printf("%s : NA\n", students[i].roll_no);
                    else
                        printf("%s : %d\n", students[i].roll_no, students[i].marks[j]);
                }
            }
        }
    }
}

// helps faculty update the marks of students who are registered for his subject.
void facmark(char Sub[])
{
    char Roll[10];
    printf("Enter the roll no of the student: \n");
    scanf("%s", &Roll);
    for (int i = 1; i < (count - 1); i++)
    {
        for (int j = 0; j < no_subjects; j++)
        {
            // finds the student who has registered in that particular course.
            if (strcmp(Sub, sub[j].subjectname) == 0 && strcmp(students[i].roll_no, Roll) == 0)
            {
                if (students[i].marks[j] != -2)
                {
                    if (students[i].marks[j] == -1)
                    {
                        int marks;
                        printf("Enter the marks: ");
                        scanf("%d", &marks);
                        students[i].marks[j] = marks;
                    }
                    else
                    {
                    t:
                        printf("Marks %d has already been given.\nDo you wish to change?\n 1: Yes\t2: No\n", students[i].marks[j]);
                        int choice;
                        choice = is_digit();
                        switch (choice)
                        {
                        case 1:
                            printf("Enter the marks: ");
                            int mark;
                            scanf("%d", &mark);
                            students[i].marks[j] = mark;
                            system("cls");
                            printf("Marks updated successfully.\n");
                            break;
                        case 2:
                            return;
                            break;
                        default:
                            printf("Wrong Choice.\n");
                            goto t;
                        }
                    }
                }
            }
        }
    }
    remove("student.csv");
    edit();
}

// allows faculty advisor to accept the leave application.
void accept_leave(char buf1[])
{
    inFile = fopen("leave_record.txt", "a");
    fprintf(inFile, "%s", strtok(buf1, " "));
    // prints the necessary content into other file such that the student can get to know about the update.
    fprintf(inFile, " Your leave got accepted from %.10s to %.10s \n", buf1 + 41, buf1 + 55);
    fclose(inFile);
    inFile = fopen("leave.txt", "r");
    FILE *temp;
    temp = fopen("temp.txt", "w");
    char *cp;
    char buf[1000];
    char buf2[1000];
    // deletes the accepted leave application from the file and restores the rest.
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        strcpy(buf2, buf);
        if (cp == NULL)
            break;
        if (strcmp(strtok(buf1, " "), strtok(buf, " ")) == 0)
            continue;
        else
            fprintf(temp, "%s\n", buf2);
    }
    fclose(inFile);
    fclose(temp);
    char ch;
    inFile = fopen("leave.txt", "w");
    temp = fopen("temp.txt", "r");
    while ((ch = fgetc(temp)) != EOF)
    {
        fprintf(inFile, "%c", ch);
    }
    fclose(inFile);
    fclose(temp);
    remove("temp.txt");

    system("cls");
    printf("Leave granted.\n");
}

// allows faculty advisor to reject the leave application.
void reject_leave(char buf1[])
{
    inFile = fopen("leave_record.txt", "a");
    printf("Enter the reason: ");
    char reason[1000];
    fflush(stdin);
    gets(reason);
    fprintf(inFile, "%s", strtok(buf1, " "));
    // prints the necessary content into other file such that the student can get to know about the update.
    fprintf(inFile, " Your leave got rejected from %.10s to %.10s due to %s\n", buf1 + 41, buf1 + 55, reason);
    fclose(inFile);
    inFile = fopen("leave.txt", "r");
    FILE *temp;
    temp = fopen("temp.txt", "w");
    char *cp;
    char buf[1000];
    char buf2[1000];
    // deletes the rejected leave application from the file and restores the rest.
    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        strcpy(buf2, buf);
        if (cp == NULL)
            break;
        if (strcmp(strtok(buf1, " "), strtok(buf, " ")) == 0)
            continue;
        else
            fprintf(temp, "%s\n", buf2);
    }
    fclose(inFile);
    fclose(temp);
    char ch;
    inFile = fopen("leave.txt", "w");
    temp = fopen("temp.txt", "r");
    while ((ch = fgetc(temp)) != EOF)
    {
        fprintf(inFile, "%c", ch);
    }
    fclose(inFile);
    fclose(temp);
    remove("temp.txt");

    system("cls");
    printf("Leave application has been rejected.\n");
}

// shows the leave application to the faculty advisor.
void view_leave(char branch[])
{
    file = fopen("leave.txt", "r");
    const char *delim1 = " \t";
    char *cp2;
    char buf1[1000];
    if (file == NULL)
    {
        printf("No applications received\n");
        fclose(file);
        choiceFunc();
        return;
    }
    int choice;
    int count1 = 0;
    while (1)
    {
        cp2 = fgets(buf1, sizeof(buf1), file);
        if (cp2 == NULL)
            break;
        // checks for the faculty advisor assigned to the student's branch.
        if ((branch[0] == buf1[2] && branch[1] == buf1[3]))
        {
            count1++;
            printf("%s", buf1 + 7);
        t:
            printf("==============================================\n");
            printf("||         1: Accept Leave Application      ||\n");
            printf("||         2: Reject Leave Application      ||\n");
            printf("||         3: Decide Later                  ||\n");
            printf("==============================================\n\n");
            printf("Enter the choice: ");
            choice = is_digit();
            switch (choice)
            {
            case 1:
                accept_leave(buf1);
                break;
            case 2:
                reject_leave(buf1);
                break;
            case 3:
                system("cls");
                return;
            default:
                system("cls");
                printf("Wrong Choice\n");
                choiceFunc();
                goto t;
            }
        }
        if (count1 == 0)
            printf("There are no leave applications pending currently\n");
    }
}

// function that provides faculty advisor all his functions.
void information_fac_FA(char Faculty_Name[], char branch[], char Sub[])
{
    // shows the pop up to the faculty advisor if any leave application is pending for him to update.
    file = fopen("leave.txt", "r");
    const char *delim1 = " \t";
    char *cp2;
    char buf1[1000];
    int count2 = 0;
    if (file == NULL)
    {
        printf("No leave applications pending(press enter to continue)\n");
        getch();
        system("cls");
        fclose(file);
        count2++;
    }
    int choice1;
    int count1 = 0;
    while (count2==0)
    {
        cp2 = fgets(buf1, sizeof(buf1), file);
        if (cp2 == NULL)
            break;
        if ((branch[0] == buf1[2] && branch[1] == buf1[3]))
        {
            count1 = 1;
        }
    }
    if (count1 == 1)
    {
    u:
        printf("===================================================\n");
        printf("||           1: View leave applications          ||\n");
        printf("||           2: See later                        ||\n");
        printf("===================================================\n");
        printf("Enter the choice: ");
        choice1 = is_digit();
        system("cls");
        switch (choice1)
        {
        case 1:
            view_leave(branch);
            choiceFunc();
            break;
        case 2:
            break;
        default:
            system("cls");
            printf("Wrong Choice\n");
            choiceFunc();
            goto u;
        }
    }


t:
    printf("ID: %s\n", Faculty_Name);
    printf("=======================================================================\n");
    printf("||      1: View students registered                                  ||\n");
    printf("||      2: Enter marks of students                                   ||\n");
    printf("||      3: View sorted list of students based on their marks         ||\n");
    printf("||      4: Change Password                                           ||\n");
    printf("||      5: Add attendance                                            ||\n");
    printf("||      6. View pending leave applications                           ||\n");
    printf("||      7: Log-Out                                                   ||\n");
    printf("||      8: Exit                                                      ||\n");
    printf("=======================================================================\n");
    printf("Enter the choice: ");
    int choice;
    choice = is_digit();
    system("cls");
    switch (choice)
    {
    case 1:
        // printf("%s", Sub);
        facview(Sub);
        choiceFunc();
        goto t;
    case 2:
        facmark(Sub);
        choiceFunc();
        goto t;
    case 3:
        viewSortedList_sub(Sub);
        choiceFunc();
        goto t;
    case 4:
        fac_update_password(Faculty_Name);
        choiceFunc();
        goto t;
    case 5:
        add_attendance(Sub);
        choiceFunc();
        goto t;
    case 6:
        view_leave(branch);
        choiceFunc();
        goto t;
    case 7:
        return;
    case 8:
        exitproject();
    default:
        system("cls");
        printf("Wrong Choice");
        choiceFunc();
        goto t;
    }
}

// function that provides faculty all his functions.
void information_fac(char Sub[], char ID[])
{
t:
    printf("ID: %s\n", ID);
    printf("=======================================================================\n");
    printf("||      1: View students Registered                                  ||\n");
    printf("||      2: Enter marks of students                                   ||\n");
    printf("||      3: View sorted list of students based on their marks         ||\n");
    printf("||      4: Change Password                                           ||\n");
    printf("||      5: Add attendance                                            ||\n");
    printf("||      6: Log-Out                                                   ||\n");
    printf("||      7: Exit                                                      ||\n");
    printf("=======================================================================\n");
    printf("Enter the choice: ");
    int choice;
    choice = is_digit();
    system("cls");
    switch (choice)
    {
    case 1:
        facview(Sub);
        choiceFunc();
        goto t;
    case 2:
        facmark(Sub);
        choiceFunc();
        goto t;
    case 3:
        viewSortedList_sub(Sub);
        choiceFunc();
        goto t;
    case 4:
        fac_update_password(ID);
        choiceFunc();
        goto t;
    case 5:
        retrieve_attendance();
        add_attendance(Sub);
        choiceFunc();
        goto t;
    case 6:
        return;
    case 7:
        exitproject();
    default:
        system("cls");
        printf("Wrong Choice\n");
        choiceFunc();
        goto t;
    }
}

// function to hide the password.
void maskInput(char password[])
{
    char ch;
    int i = 0;

    while (1)
    {
        ch = getch(); // Read a character without echoing it
        if (ch == '\r' || ch == '\n')
        {
            password[i] = '\0'; // Terminate the password string
            break;
        }
        else if (ch == '\b' && i > 0)
        {
            // Handle backspace: remove the last character
            printf("\b \b");
            i--;
        }
        else if (i < MAX_PASSWORD_LENGTH - 1)
        {
            // Display an asterisk and store the character
            printf("*");
            password[i++] = ch;
        }
    }
}

// function to authenticate admin.
void admin_login()
{
    printf("=====================================================\n");
    printf("          Welcome to the Admin Login portal          \n");
    printf("=====================================================\n\n");

    int choice1, input;
    char Username1[10], PassWord1[20];

    printf("Enter Username: ");
    scanf("%s", Username1);
    printf("Enter Password: ");
    maskInput(PassWord1);

    if (strcmp("Admin", Username1) == 0 && strcmp("Admin@iitbbs", PassWord1) == 0)
    {
        system("cls");
        char Login[] = "LOGGED IN SUCCESSFULLY (Press Enter)";
        for (int i = 0; i < strlen(Login); i++)
        {
            printf("%c", Login[i]);
            Sleep(20);
        }
        getch();
        // function to check headerfile
        headercheck();
    t:
        system("cls");
        printf("\n");
        printf("===========================================================\n");
        printf("||      1: Add students                                  ||\n");
        printf("||      2: Edit student details                          ||\n");
        printf("||      3: Delete students                               ||\n");
        printf("||      4: View the results of a particular student      ||\n");
        printf("||      5: View students in terms of their cgpa          ||\n");
        printf("||      6: Add faculty                                   ||\n");
        printf("||      7: Delete faculty                                ||\n");
        printf("||      8: Controls regarding elections                  ||\n");
        printf("||      9: Log-Out                                       ||\n");
        printf("||      10: Exit                                         ||\n");
        printf("===========================================================\n\n");
        printf("Enter the choice: ");
        choice1 = is_digit();
        system("cls");
        switch (choice1)
        {
        case 1:
            add_students();
            choiceFunc();
            goto t;
        case 2:
            printf("Enter the roll number of the student:\n");
            char rollno1[] = "000000";
            bool if_student_present1 = 0;
            scanf("%s", rollno1);
            for (int i = 1; i < count - 1; i++)
                if (strcmp(students[i].roll_no, rollno1) == 0)
                {
                    edit_students_info(i);
                    if_student_present1 = 1;
                    break;
                }
            if (if_student_present1 == 0)
            {
                system("cls");
                printf("The above roll number is not present.\n");
            }
            choiceFunc();
            goto t;

        case 3:
            printf("Enter the roll number of the student:\n");
            char rollno[] = "000000";
            bool if_student_present = 0;
            scanf("%s", rollno);
            for (int i = 1; i < count - 1; i++)
                if (strcmp(students[i].roll_no, rollno) == 0)
                {
                    delete_students_info(i);
                    if_student_present = 1;
                    break;
                }
            if (if_student_present == 0)
            {
                system("cls");
                printf("The above roll number is not present.\n");
            }
            choiceFunc();
            goto t;
        case 4:
            printf("Enter the student's roll number: ");
            bool if_student_present2 = 0;
            char Roll_No[20];
            scanf("%s", &Roll_No);

            for (int i = 1; i < (count - 1); i++)
            {
                if (strcmp(students[i].roll_no, Roll_No) == 0)
                {
                    system("cls");
                    printf("Name: %s\tRoll_No: %s\n", students[i].name, students[i].roll_no);
                    view(i, true);
                    if_student_present2 = 1;
                    break;
                }
            }
            if (if_student_present2 == 0)
            {
                system("cls");
                printf("The above roll number is not present.\n");
            }
            choiceFunc();
            goto t;
        case 5:
        label:
            printf("===================================================================================================\n");
            printf("For students of branch: 1. CS    2. EC    3. EE    4. ME    5. CE    6. MM    7. Overall    8. Exit\n");
            printf("===================================================================================================\n");
            scanf("%d", &input);
            if (input > 0 && input < 7)
            {
                viewSortedList_branch(input);
            }
            else if (input == 7)
            {
                viewSortedList();
            }
            else if (input == 8)
            {
                exitproject();
                break;
            }
            else
            {
                system("cls");
                printf("Invalid input\n");
                choiceFunc();
                goto label;
            }
            choiceFunc();
            goto t;
        case 6:
            add_faculty();
            choiceFunc();
            goto t;
        case 7:
            delete_faculty_info();
            choiceFunc();
            goto t;
        case 8:
            adminPanel();
            choiceFunc();
            goto t;
        case 9:
            return;
        case 10:
            exitproject();
        default:
            system("cls");
            printf("Wrong Choice\n");
            choiceFunc();
            goto t;
        }
    }
    else
    {
        system("cls");
        printf("Invalid login credentials.\n");
        choiceFunc();
        return;
    }
}

// function to authenticate students.
void student_login()
{
    printf("=======================================================\n");
    printf("          Welcome to the Student Login portal          \n");
    printf("=======================================================\n");
u:
    file = fopen("Subject.txt", "r");
    if (file == NULL)
    {
        printf("\n\nThe database is still under development...\nPlease wait");
        fclose(file);
        choiceFunc();
        return;
    }

    const char *delim = " \t";
    char *cp, buf[1000];
    bool if_roll_number_present = 0;
    char *RollNo, *PassWord;
    RollNo = (char *)malloc(20 * sizeof(char));
    PassWord = (char *)malloc(20 * sizeof(char));

    printf("\n\nEnter the Roll Number: ");
    scanf(" %s", RollNo);
    printf("Enter the Password: ");
    maskInput(PassWord);
    system("cls");

    while (1)
    {
        // get a line
        cp = fgets(buf, sizeof(buf), file);
        if (cp == NULL)
            break;

        // get the student name
        cp = strtok(buf, delim);
        if (strcmp(cp, RollNo) != 0)
            continue;
        else
            cp = strtok(NULL, delim);
        if (cp == NULL)
            break;
        int count1 = 0;
        if (strcmp(cp, PassWord) == 0)
        {
            system("cls");
            char Login[] = "LOGGED IN SUCCESSFULLY (Press Enter)";

            for (int i = 0; i < strlen(Login); i++)
            {
                printf("%c", Login[i]);
                Sleep(20);
            }
            getch();
            system("cls");
            retrieve_info();
            information_stu(RollNo);
            if_roll_number_present = 1;
            count1++;
        }
        fclose(file);
        break;
    }
    free(RollNo);
    free(PassWord);
    if (if_roll_number_present == 0)
    {
        printf("The above roll number does not exist or the password is wrong.\n");
        choiceFunc();
        goto u;
    }
}

// function to authenticate faculty.
void faculty_login()
{
    printf("=======================================================\n");
    printf("          Welcome to the Faculty Login portal          \n");
    printf("=======================================================\n\n\n");
b:
    file = fopen("Faculty.txt", "r");
    if (file == NULL)
    {
        printf("The database is still under development...\nPlease wait");
        fclose(file);
        choiceFunc();
        return;
    }

    int if_faculty_present = 0;
    const char *delim1 = " \t";
    char *cp2;
    char buf1[1000];
    char ID[20];
    char PassWord2[20];

    printf("Enter ID : ");
    scanf("%s", &ID);
    printf("Enter the password : ");
    maskInput(PassWord2);
    system("cls");

    while (1)
    {
        //  get a line
        cp2 = fgets(buf1, sizeof(buf1), file);
        if (cp2 == NULL)
            break;
        // get the faculty name
        cp2 = strtok(buf1, delim1);
        if (strcmp(cp2, ID) != 0)
        {
            continue;
        }
        else
            cp2 = strtok(NULL, delim1);
        if (cp2 == NULL)
            break;
        int count1 = 0;
        if (strcmp(cp2, PassWord2) == 0)
        {
            if_faculty_present = 1;
            cp2 = strtok(NULL, delim1);
            system("cls");
            char Login[] = "LOGGED IN SUCCESSFULLY (Press Enter)\n";
            for (int i = 0; i < strlen(Login); i++)
            {
                printf("%c", Login[i]);
                Sleep(20);
            }
            getch();
            system("cls");
            inFile = fopen("FacultyAdvisor.txt", "r");
            char *cp3;
            while (1)
            {
                //  get a line
                cp3 = fgets(buf1, sizeof(buf1), inFile);
                if (cp3 == NULL)
                    break;
                // get the faculty name
                cp3 = strtok(buf1, delim1);
                if (strcmp(cp3, ID) != 0)
                {
                    continue;
                }
                else
                    cp3 = strtok(NULL, delim1);
                if (cp3 == NULL)
                    break;
                count1 = 1;
                information_fac_FA(ID, cp3, cp2);
                break;
            }
            fclose(inFile);
            if (count1 != 1)
                information_fac(cp2, ID);
        }
        fclose(file);
        break;
    }

    if (if_faculty_present == 0)
    {
        printf("Invalid Login Credentials.\n");
        fclose(file);
        choiceFunc();
        goto b;
    }
}

// display the tiers of login options.
void login()
{
    int choice, choice1, input;
v:
    system("cls");
    printf("================================\n");
    printf("             LOGIN              \n");
    printf("================================\n");
    printf("||      1: Admin Login        ||\n");
    printf("||      2: Student Login      ||\n");
    printf("||      3: Faculty Login      ||\n");
    printf("||      4: Back               ||\n");
    printf("||      5: Exit               ||\n");
    printf("================================\n");
    printf("Enter the choice: ");
    PlaySound(NULL, NULL, SND_PURGE);
    choice = is_digit();
    system("cls");
    switch (choice)
    {
    case 1:
        admin_login();
        goto v;
    case 2:
        student_login();
        goto v;
    case 3:
        faculty_login();
        goto v;
    case 4:
        system("cls");
        return;
    case 5:
        exitproject();
    default:
        system("cls");
        printf("Please enter a valid number. (Press enter to continue)");
        getch();
        choiceFunc();
        goto v;
    }
}

// description about project.
void aboutus()
{
    if (!PlaySound(TEXT("begin.wav"), NULL, SND_FILENAME | SND_ASYNC)) {
        printf("Error playing music.\n");
        exit(0);
    }

    Sleep(1000);

    char Intro[] = "                               *************************************                               \n--> This project is a collaborative effort by the authors to create a student database management portal,\n    incorporating various possible features typically present in such a system.\n--> The code, in its entirety, is based solely on C language, as per the project requirements.\n                               *************************************                               \n\n";
    for (int i = 0; i < strlen(Intro); i++)
    {
        printf("%c", Intro[i]);
        Sleep(20);
    }

    char Authors[] = "============================================\n|| Sl.no. |     Authors     | Roll number ||\n============================================\n||   1.   |  Adarsh Chandra |  23CS01001  ||\n||   2.   |  Shashank M N   |  23CS02010  ||\n||   3.   |  Arit Gandhi    |  23CS01006  ||\n||   4.   |  Deep Jindal    |  23CS02002  ||\n||   5.   |  Vineet Kumar   |  23CS01068  ||\n============================================\n\n";
    for (int i = 0; i < strlen(Authors); i++)
    {
        printf("%c", Authors[i]);
        Sleep(30);
    }

    printf("(Press Enter to Continue)");
    getch();

    system("cls");
    int choice;
    printf("\nEnter  1: Go to Main Menu\n       2: Exit\n");
    printf("Enter the choice: ");
    PlaySound(NULL, NULL, SND_PURGE);
    scanf("%d", &choice);
    if (choice == 1)
    {
        system("cls");
        return;
    }
    else
        exitproject();
}

int main()
{
    system("cls");
    file = fopen("student.csv", "r");
    if (file != NULL)
    {
        retrieve_info();
        retrieve_attendance();
    }
    fclose(file);

    if (!PlaySound(TEXT("begin.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP )) {
        printf("Error playing music.\n");
        exit(1);
    }

    Sleep(2000);

    char Welcome[] = "        ****************************        \n  Welcome to EduSync : Academic Profile Portal  \n        ****************************        \n";
    for (int i = 0; i < strlen(Welcome); i++)
    {
        printf("%c", Welcome[i]);
        Sleep(40);
    }

    int choice;
t:
    printf("\n\n========================\n");
    printf("       MAIN MENU        \n");
    printf("========================\n");
    printf("||      1: Login      ||\n");
    printf("||      2: About us   ||\n");
    printf("||      3: Exit       ||\n");
    printf("==========================\n");
    printf("Enter the choice: ");
    choice = is_digit();
    system("cls");
    switch (choice)
    {
    case 1:
        login();
        goto t;
    case 2:
        aboutus();
        goto t;
    case 3:
        exitproject();
        break;
    default:
        system("cls");
        printf("Wrong Choice\n");
        choiceFunc();
        goto t;
    }
    return 0;
}
