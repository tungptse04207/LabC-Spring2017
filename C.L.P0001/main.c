/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: crazyboy
 *
 * Created on January 10, 2017, 2:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
 * 
 */
const int MAX_LENGTH = 30;

char** loadFile(char* fileName, char** arrWord, int *count); //read line and save to arrWord
int exitProgram(char* mess); //yes/no to exit program
char* lowerString(char* str); //to lower all character
int cmpfunc(const void * a, const void * b); // compare function use in qsort()
int binarySearch(char* word, char** arrWord, int count); // binary search algorthrim
void isExist(char* word, char** arrWord, int count);
int findWord(char* word, char** arrWord, int totalWord); //find possible word

int main(int argc, char** argv) {
    char** arrWord = NULL;
    char* word;
    int i, totalWord = 0;
    printf("Welcome to the Spell Checker\n");
    arrWord = loadFile("dictionary.txt", arrWord, &totalWord);
    qsort(arrWord, totalWord, sizeof (char*), cmpfunc);
    printf("The dictionary has been loaded\n");
    do {
        word = (char*) calloc(255, sizeof (char));
        printf("Please enter the word you would like to checked: ");
        gets(word);
        word = lowerString(word);
        isExist(word, arrWord, totalWord);
        free(word); //clear memory for *word
    } while (exitProgram("Would you like to enter another word? (yes/no) "));
    //clear memory for **arrWord
    for (i = 0; i < totalWord; i++) {
        free(arrWord[i]);
    }
    free(arrWord);
    return (EXIT_SUCCESS);
}

char** loadFile(char* fileName, char** arrWord, int *count) {
    FILE* f;
    char* line = (char*) calloc(1, sizeof (char));
    f = fopen(fileName, "r");
    while (fgets(line, MAX_LENGTH, f) != NULL) {
        line[strlen(line) - 1] = '\0'; //clear ENTER at the end of line
        if (arrWord == NULL) {
            arrWord = (char**) calloc(1, sizeof (char*));
        } else {
            int newSize = (*count) + 1;
            arrWord = (char**) realloc(arrWord, newSize * sizeof (char*));
        }
        arrWord[(*count)] = (char*) calloc(30, sizeof (char));
        strcpy(arrWord[(*count)], line);
        (*count)++;
    }
    fclose(f);
    free(line);
    return arrWord;
}

int exitProgram(char* mess) {
    int i;
    char* ch = (char*) calloc(255, sizeof (char));
    while (1) {
        printf("%s ", mess);
        gets(ch);
        ch = lowerString(ch);
        if (strcmp(ch, "yes") == 0) {
            return 1;
        } else if (strcmp(ch, "no") == 0) {
            return 0;
        }
    }
}

char* lowerString(char* str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

int cmpfunc(const void * str1, const void * str2) {
    const char *a = *(const char **) str1;
    const char *b = *(const char **) str2;
    return strcmp(a, b);
}

/*
 * if found return 1
 * else return 0
 */
int binarySearch(char* word, char** arrWord, int count) {
    int left = 0, right = count - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (strcmp(word, arrWord[mid]) == 0) {
            return 1;
        } else if (strcmp(word, arrWord[mid]) < 0) {
            right = mid - 1;
        } else
            left = mid + 1;
    }
    return 0;
}

void isExist(char* word, char** arrWord, int count) {
    switch (binarySearch(word, arrWord, count)) {
        case 1:
            printf("Great, %s in the dictionary\n", word);
            break;
        case 0:
            printf("Hear is the possible words you could have meant:\n");
            if (findWord(word, arrWord, count) == 0) {
                printf("No possible words\n");
            }
            break;
    }
}

/*
 * return 0 if not found or length of word <3 or >30
 */
int findWord(char* word, char** arrWord, int totalWord) {
    int i, j, k, count, countFoundWord = 0;
    if (strlen(word) < 3 || strlen(word) > 30) {
        return 0;
    }
    for (i = 0; i < totalWord; i++) {
        count = 0;
        if (strlen(arrWord[i]) >= 3) {
            char tmp[30];
            strcpy(tmp, arrWord[i]);
            for (j = 0; j < strlen(word); j++) {
                for (k = 0; k < strlen(tmp); k++) {
                    if ((int) word[j] == (int) tmp[k]) {
                        tmp[k] = 'A'; //mark this character not to recheck
                        count++;
                        if (count == 3) {
                            puts(arrWord[i]);
                            countFoundWord++;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (countFoundWord == 0) {
        return 0;
    }
    return 1;
}
