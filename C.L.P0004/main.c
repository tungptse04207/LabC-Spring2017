#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* type;
    int point;
    char* question;
    int sizeLstAnswer;
    char** lstAnswer;
    char* trueAnswer;
} QUE;

char** readFile(char*, char**, int*, int*); //open file, read and save to **file
QUE addQuestion(char**, QUE, int*); //split question
void show(QUE); //show question
int inputInt(); //input and valid integer
int* nextQuestion(int*, int*, int); //random no duplicate question
void play(char*, char*, int, QUE*, int); //game play  <3<3<3

int main() {
    QUE* lstQue = NULL;
    char** file = NULL;
    char* firstname = (char*) calloc(256, sizeof (char));
    char* lastname = (char*) calloc(256, sizeof (char));
    char* filename = (char*) calloc(256, sizeof (char));
    int countQuestion = 0;
    int totalLine = 0;
    int i;
    printf("What is your first name? ");
    gets(firstname);
    printf("What is your last name? ");
    gets(lastname);
    printf("What file stored your question? ");
    gets(filename);
    file = readFile(filename, file, &countQuestion, &totalLine);
    if (countQuestion == 0) {
        printf("File not found!");
        exit(0);
    } else {
        lstQue = (QUE*) calloc(countQuestion, sizeof (QUE));
        i = 0;
        int questionNo = 0;
        while (i < totalLine) {
            lstQue[questionNo++] = addQuestion(file, lstQue[questionNo], &i);
        }
        printf("Data load successful!\n");
    }
    int n = inputInt();
    play(firstname, lastname, n, lstQue, countQuestion);
    free(lstQue);
    free(firstname);
    free(lastname);
    free(filename);
    for(i=0; i<totalLine; i++){
        free(file[i]);
    }
    free(file);
    return (EXIT_SUCCESS);
}

char** readFile(char* filename, char** file, int* countQuestion, int* totalLine) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        (*countQuestion) = 0;
    } else {
        char* line = (char*) calloc(255, sizeof (char));
        while (fgets(line, 256, f) != NULL) {
            if (file == NULL) {
                file = (char**) calloc(1, sizeof (char**));
            } else {
                file = (char**) realloc(file, ((*totalLine) + 1) * sizeof (char**));
            }
            file[(*totalLine)] = (char*) calloc(256, sizeof (char));
            line[strlen(line) - 1] = '\0';
            strcpy(file[(*totalLine)], line);
            (*totalLine)++;
        }
        free(line);
        fclose(f);
        (*countQuestion) = atoi(file[0]);
    }
    return file;
}

QUE addQuestion(char** file, QUE q, int* lineNum) {
    if ((*lineNum) == 0) {
        (*lineNum)++;
    }

    //get type (TF, MC, SA)
    q.type = (char*) calloc(2, sizeof (char));
    strcpy(q.type, strtok(file[(*lineNum)], " "));

    //get point
    q.point = atoi(strtok(NULL, " "));

    //get question
    q.question = (char*) calloc(256, sizeof (char));
    strcpy(q.question, file[++(*lineNum)]);

    //type TF, SA
    if (strcmp(q.type, "TF") == 0 || strcmp(q.type, "SA") == 0) {
        q.sizeLstAnswer = 0;
        q.lstAnswer = NULL;
        q.trueAnswer = (char*) calloc(256, sizeof (char));
        strcpy(q.trueAnswer, file[++(*lineNum)]);
    }
        
    //type MC
    else {
        q.sizeLstAnswer = atoi(file[++(*lineNum)]);
        q.lstAnswer = (char**) calloc(q.sizeLstAnswer, sizeof (char*));
        int i = 0;
        while (i < q.sizeLstAnswer) {
            q.lstAnswer[i] = (char*) calloc(256, sizeof (char));
            strcpy(q.lstAnswer[i++], file[++(*lineNum)]);
        }
        q.trueAnswer = (char*) calloc(256, sizeof (char));
        strcpy(q.trueAnswer, file[++(*lineNum)]);
    }
    (*lineNum)++;
    return q;
}

void show(QUE q) {
    int i;
    printf("Point: %d\n", q.point);
    printf("Question: %s\n", q.question);
    if (q.sizeLstAnswer != 0) {
        for (i = 0; i < q.sizeLstAnswer; i++) {
            printf("%c) %s\n", (char) (i + 65), q.lstAnswer[i]);
        }
    }
}

int inputInt() {
    int k = 0, n;
    char c;
    while (1) {
        printf("How many question would you like? (out of 3): ");
        k = scanf("%d%c", &n, &c);
        if (k == 0) {
            printf("Sorry, that is not valid!\n");
            /* 
             * clear buffer like fflush and fpurge
             */
            while (getchar() != '\n');
        } else if (n <= 0) {
            printf("Sorry, that is too small!\n");
        } else if (n > 3) {
            printf("Sorry, that is too many!\n");
        } else {
            return n;
        }
    }
}

int* nextQuestion(int* arr, int* size, int countQuestion) {
    if (arr == NULL) {
        arr = (int*) calloc((*size) + 1, sizeof (int));
        arr[(*size)] = (rand() % countQuestion);
    } else {
        arr = (int*) realloc(arr, ((*size) + 1) * sizeof (int));
        int check, n;
        do {
            check = 1;
            n = rand() % countQuestion;
            int i;

            //check duplicate
            for (i = 0; i < (*size); i++) {
                if (n == arr[i]) {
                    check = 0;
                    break;
                }
            }
        } while (check == 0);
        arr[(*size)] = n;
    }
    (*size)++;
    return arr;
}

void play(char*firstname, char*lastname, int n, QUE* lstQue, int countQuestion) {
    int* arr = NULL;
    int size = 0;
    int i = 0;
    int point = 0;
    do {
        int* answer = (char*) calloc(256, sizeof (char));
        arr = nextQuestion(arr, &size, countQuestion);
        show(lstQue[arr[size - 1]]);
        gets(answer);
        if (strcmp(answer, "SKIP") == 0) {
            printf("You have selected to SKIP that quesion\n");
            break;
        } else if (strcmp(answer, lstQue[arr[size - 1]].trueAnswer) == 0) {
            printf("Correct, You get %d point\n", lstQue[arr[size - 1]].point);
            point += lstQue[arr[size - 1]].point;
        } else {
            printf("Incorrect, the answer was %s. You lose %d point\n", lstQue[arr[size - 1]].trueAnswer, lstQue[arr[size - 1]].point);
            point -= lstQue[arr[size - 1]].point;
        }
        free(answer);
        i++;
    } while (i < n);
    printf("******************************\n");
    printf("%s %s, Your game is over\n", firstname, lastname);
    printf("Your final score is %d point\n", point);
    free(arr);
}
