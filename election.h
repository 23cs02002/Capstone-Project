#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#define MAX_CSV_LINE_LENGTH 1024
#define MAX_ROLL_NUMBER_LENGTH 7

// global
int No_of_Candidates; // Total no of candidates for election
int Total_voters;     // total number of students from student.csv

//Counts the number of students in student.csv file
int countStudents()
{ 
    FILE *fp;
    int count = 0;
    char ch;

    fp = fopen("student.csv", "r");
    if (fp == NULL)
    {
        perror("Error opening file.\n");
        exit(1);
    }

    // Count lines (students), excluding the header line
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    fclose(fp);
    return count - 1; // Subtract 1 for the header line
}

//Starts a new election
void initiateNewElection() 
{
    system("cls");
    Total_voters = countStudents();
    printf("\nNew Election Initiation:\n");
    printf("Enter the no. of candidates: ");
    scanf("%d", &No_of_Candidates);

    FILE *fp;
    fp = fopen("candidate.csv", "w");
    if (fp == NULL)
    {
        perror("\nError opening file ...\n");
        exit(1);
    }

    fprintf(fp, "cid,cname,vote\n");

    while (getchar() != '\n') ;//removes all whitespaces utimately clearin buffer to prevent unexpected errors

    for (int i = 0; i < No_of_Candidates; i++)
    {
        char name[100];
        printf("Enter name of candidate %d: ", i + 1);
        fflush(stdin);
        fgets(name, 100, stdin);
        name[strcspn(name, "\n")] = 0; // puts '\0' where the the /n comes for first time
        fprintf(fp, "%d,%s,%d\n", i + 1, name, 0);
    }
    fclose(fp);
    return;
}

//Stores the total no. of students and total candidates
void saveElectionInfoInFile()
{ 
    printf("Saving Election Info in File...\n");
    FILE *fp = fopen("ElectionInfo.csv", "w");
    if (fp == NULL)
    {
        printf("\nError in file creation\n");
        fclose(fp);
        return;
    }
    fprintf(fp, "%d,%d", countStudents(), No_of_Candidates);
    fclose(fp);
    printf("Saved Successfully :)\n");
    sleep(1);
    system("cls");
}

//Declares the result of election winner or draw
void getWinner()
{
    system("cls");
    struct candidate
    {
        int cid;
        char cname[100];
        int votes;
    } CANDIDATE[1000];
    FILE *fp, *file;
    fp = fopen("candidate.csv", "r");
    if (fp == NULL)
    {
        printf("No active elections are going on.\n");
        return;
    }

    char header[MAX_CSV_LINE_LENGTH];
    fgets(header, sizeof(header), fp);// storing cid,cname,votes line

    int num_of_candidates = 0;
    char line[MAX_CSV_LINE_LENGTH];
    for (num_of_candidates = 0; fgets(line, sizeof(line), fp); num_of_candidates++)
    {
        sscanf(line, "%d,%[^,],%d", &CANDIDATE[num_of_candidates].cid, CANDIDATE[num_of_candidates].cname, &CANDIDATE[num_of_candidates].votes);
    }
    fclose(fp);

    int max_votes = -1;
    int winner_index = -1;
    int tie = 0;
    for (int i = 0; i < num_of_candidates; i++)
    {
        if (CANDIDATE[i].votes > max_votes)
        {
            max_votes = CANDIDATE[i].votes;
            winner_index = i;
            tie = 0;
        }
        else if (CANDIDATE[i].votes == max_votes)
        {
            tie = 1;
        }
    }

    file = fopen("result.txt", "w");
    char buffer[100];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 100, "%d-%m-%Y at %H:%M:%S", tm_info);
    fprintf(file, "****The result of the previous election is as follows.****\n\t\t(Note: The result was declared on %s)\n", buffer);

    if (tie)
    {
        printf("\nIt's a tie between the following candidates :\n");
        fprintf(file, "It's a tie between the following candidates :\n");
        for (int i = 0; i < num_of_candidates; i++)
        {
            if (CANDIDATE[i].votes == max_votes)
            {
                printf("%s - Votes: %d\n", CANDIDATE[i].cname, CANDIDATE[i].votes);
                fprintf(file, "%s\n", CANDIDATE[i].cname);
            }
        }
    }
    else
    {
        printf("\n**%s is the Winner with %d votes**\n", CANDIDATE[winner_index].cname, CANDIDATE[winner_index].votes);
        fprintf(file, "%s is the Winner of the election.\n", CANDIDATE[winner_index].cname);
    }
    fclose(file);
    remove("voted_students.txt");
    remove("candidate.csv");
    remove("ElectionInfo.csv");
    printf("(Press enter to return)\n");
    getch();
    system("cls");
}

//Shows result of election to the admin
void view_result_of_election_admin()
{
    system("cls");
    FILE *file;
    file = fopen("result.txt", "r");
    if (file == NULL)
        printf("No elections are held until now.\n");
    else
    {
        char ch;
        while ((ch = fgetc(file)) != EOF)
            printf("%c", ch);//prints the entire content of the result.txt file
    }
}

//Admin election controls
void adminPanel() 
{
    while (1)
    {

        while (1)
        {
            char input;
        t:
            system("cls");
            printf("\n==============================\n");
            printf("      ELECTION  CONTROLS      \n");
            printf("==============================\n");
            printf("||     1: New election      ||\n");
            printf("||     2: Declare result    ||\n");
            printf("||     3: View result       ||\n");
            printf("||     4: Back              ||\n");
            printf("==============================\n\n");
            printf("Option: ");
            scanf(" %c", &input);
            system("cls");

            switch (input)
            {
            case '1':
                initiateNewElection();
                saveElectionInfoInFile();
                break;
            case '2':
                printf("");
                FILE *fp;
                fp = fopen("candidate.csv", "r");
                if (fp == NULL)
                {
                    printf("No active elections are going on.\n");
                    sleep(1);
                    system("cls");
                    break;
                }
                fclose(fp);
                printf("\n--> Note: Once u declare the result, no more voting can be done.\n    If you wish to declare the result, please enter 1 or enter 0(or any other character) to go back.\n    Option: ");
                int choice;
                scanf("%d", &choice);
                switch (choice)
                {
                case 1:
                    getWinner();
                    break;
                default:
                    goto t;
                }
                break;
            case '3':
                view_result_of_election_admin();
                printf("\n(Press enter to return)\n");
                getch();
                system("cls");
                goto t;
            case '4':
                return;
            default:
                printf("Invalid Option(Press Enter)\n");
                getch();
                goto t;
            }
        }
    }
};

//Checks if the student has voted or not
int isVoted(char userID[MAX_ROLL_NUMBER_LENGTH])
{
    FILE *file = fopen("voted_students.txt", "a+");
    if (file == NULL)
    {
        perror("Error opening voted_students.txt\n");
        exit(1);
    }

    // Check if the file pointer is positioned at the beginning of the file
    fseek(file, 0, SEEK_SET);

    char line[MAX_ROLL_NUMBER_LENGTH]; // Including null terminator
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        if (strcmp(line, userID) == 0)
        {
            fclose(file);
            return 1; // Student has voted
        }
    }
    fclose(file);
    return 0;
}

//Save the roll number of students in a file who has voted 
void saveVote(char userID[15], char voteInput)
{
    // Open the file to store the roll numbers of voted students in append mode
    FILE *votedFile = fopen("voted_students.txt", "a+");
    if (votedFile == NULL)
    {
        perror("Error opening voted_students.txt for writing.\n");
        return;
    }
    fprintf(votedFile, "%s\n", userID);
    fclose(votedFile);
}

//Voting for the candidates avaliable 
void castvote()
{
    struct candidate
    {
        int cid;
        char cname[100];
        int votes;
    } CANDIDATE[1000];
    FILE *fp;
    fp = fopen("candidate.csv", "r");
    if (fp == NULL)
    {
        perror("Error opening file ...\n");
        exit(1);
    }

    char header[MAX_CSV_LINE_LENGTH];
    fgets(header, sizeof(header), fp);

    int num_of_candidates = 0;
    char line[MAX_CSV_LINE_LENGTH];
    for (num_of_candidates = 0; fgets(line, sizeof(line), fp); num_of_candidates++)
    {
        sscanf(line, "%d,%[^,],%d", &CANDIDATE[num_of_candidates].cid, CANDIDATE[num_of_candidates].cname, &CANDIDATE[num_of_candidates].votes);
    }
    fclose(fp);

    for (int i = 0; i < num_of_candidates; i++)
    {
        printf("\n%d.%s ", CANDIDATE[i].cid, CANDIDATE[i].cname);
    }

    char voteinput;
t:
    printf("\nYour Vote (enter number)= ");
    voteinput = getch();
    voteinput -= 48;
    printf("*");
    if (voteinput < 1 || voteinput > num_of_candidates)
    {
        printf("\nInvalid choice !! Try again ...\n");
        goto t;
    }

    FILE *fp1;
    fp1 = fopen("candidate.csv", "w");
    fprintf(fp1, "cid,cname,votes\n");
    for (int i = 0; i < num_of_candidates; i++)
    {
        if (voteinput == CANDIDATE[i].cid)
        {
            fprintf(fp1, "%d,%s,%d\n", CANDIDATE[i].cid, CANDIDATE[i].cname, ++CANDIDATE[i].votes);
        }
        else
        {
            fprintf(fp1, "%d,%s,%d\n", CANDIDATE[i].cid, CANDIDATE[i].cname, CANDIDATE[i].votes);
        }
    }
    fclose(fp1);
}

//Student election options
void studentPanel(char userID[])
{
    // char userID[15];
    int voteInput;
    FILE *file;
    file = fopen("candidate.csv", "r");
    if (file == NULL)
    {
        printf("No elections are being conducted.\n");
        return;
    }
    else
    {
        while (1)
        {
            printf("\n\n  To exit press 0");
            if (strcmp(userID, "0") == 0)
                return;

            if (isVoted(userID) != 0)
            {
                printf("\n  Vote already casted !!\n  Contact Admin for furthur query(Press enter to end the programme)\n");
                getch();
                break;
            }
            castvote();

            saveVote(userID, voteInput);
            printf("\n\nThanks for your precious vote(Press Enter)\n");
            getch();
            break;
        }
    }
}
