#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
const int MAX_LENGTH = 30;

char** loadFile(char* fileName, char** arrWord, int *count); //read line and save to arrWord
int exitProgram(char* mess); //yes/no to exit program
char* lowerString(char* str); //to lower all character
int cmpfunc(const void * a, const void * b); // compare function use in qsort()
int binarySearch(char* word, char** arrWord, int count); // binary search algorthrim
void menu(char* word, char** arrWord, int count);
int subStr(char*, char*);
int subSequence(char*, char*);
int permutaion(char*, char*);
int matchScore(char*, char*);

int main() {
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
        menu(word, arrWord, totalWord);
        free(word); //clear memory for *word
    } while (exitProgram("Would you like to enter another word? (yes/no) "));
    //clear memory for **arrWord
    for (i = 0; i < totalWord; i++) {
        free(*(arrWord + i));
    }
    free(arrWord);
    return (EXIT_SUCCESS);
}

char** loadFile(char* fileName, char** arrWord, int *count) {
    FILE* f;
    char* line = (char*) calloc(MAX_LENGTH, sizeof (char));
    f = fopen(fileName, "r");
    while (fgets(line, MAX_LENGTH, f) != NULL) {
        line[strlen(line) - 1] = '\0'; //clear ENTER at the end of line
        if (arrWord == NULL) {
            arrWord = (char**) calloc(1, sizeof (char*));
        } else {
            int newSize = (*count) + 1;
            arrWord = (char**) realloc(arrWord, newSize * sizeof (char*));
        }
        *(arrWord + (*count)) = (char*) calloc(MAX_LENGTH, sizeof (char));
        strcpy(*(arrWord + (*count)), line);
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
        if (strcmp(word, *(arrWord + mid)) == 0) {
            return 1;
        } else if (strcmp(word, *(arrWord + mid)) < 0) {
            right = mid - 1;
        } else
            left = mid + 1;
    }
    return 0;
}

void menu(char* word, char** arrWord, int count) {
    switch (binarySearch(word, arrWord, count)) {
        case 1:
            printf("Great, %s in the dictionary\n", word);
            break;
        case 0:
            printf("Hear is the possible words you could have meant:\n");
            int i, found = 0;
            for (i = 0; i < count; i++) {
                if (subStr(word, (*(arrWord + i)))
                        || subSequence(word, *(arrWord + i))
                        || permutation(word, *(arrWord + i))
                        || matchScore(word, *(arrWord + i))) {
                    puts(*(arrWord + i));
                    found++;
                }
            }
            if (found == 0) {
                printf("Sorry, no possible for this word!!!");
            }
            break;
    }
}

/*
 * case sub string
 * shortStr is a child string of longStr
 */
int subStr(char* shortStr, char* longStr) {
    if (strstr(longStr, shortStr) != NULL) {

        return 1;
    }
    return 0;
}

/*
 * all character of shortStr exist in longSrt
 * (follow by index)
 * ex: "our" and "computer"
 */
int subSequence(char* shortStr, char* longStr) {
    while (*shortStr != '\0') {
        while (*shortStr != *longStr && *longStr != '\0') {
            *longStr++;
        }
        if (*longStr == '\0') {
            break;
        }
        if (*shortStr == *longStr) {
            *shortStr++;
            *longStr++;
        }
    }
    if (*shortStr == '\0') {

        return 1;
    }
    return 0;
}

/*
 * str1 and str2 must be same length
 * all character of str1 exist in str2 but dont care about index
 * ex: "the" and "eth"
 */
int permutation(char* str1, char* str2) {
    if (strlen(str1) != strlen(str2)) {
        return 0;
    }
    char* tmpStr1 = (char*) calloc(MAX_LENGTH, sizeof (char));
    char* tmpStr2 = (char*) calloc(MAX_LENGTH, sizeof (char));
    strcpy(tmpStr1, str1);
    strcpy(tmpStr2, str2);
    int i, j;
    for (i = 0; i < strlen(tmpStr1); i++) {
        for (j = i; j < strlen(tmpStr2); j++) {
            if (*(tmpStr1 + i) > *(tmpStr1 + j)) {
                char tmp = *(tmpStr1 + i);
                (*(tmpStr1 + i)) = (*(tmpStr1 + j));
                (*(tmpStr1 + j)) = tmp;
            }
            if (*(tmpStr2 + i) > *(tmpStr2 + j)) {
                char tmp = *(tmpStr2 + i);
                (*(tmpStr2 + i)) = (*(tmpStr2 + j));
                (*(tmpStr2 + j)) = tmp;
            }
        }
    }
    if (strcmp(tmpStr1, tmpStr2) == 0) {
        free(tmpStr1);
        free(tmpStr2);
        return 1;
    }
    free(tmpStr1);
    free(tmpStr2);

    return 0;
}

/*
 * str1 and str2 mush same lenght
 * follow by index str1[index] must match str2[index]
 * allow different < 3
 */
int matchScore(char* str1, char* str2) {
    int match = 0;
    if (strlen(str1) != strlen(str2)) {
        return 0;
    }
    while (*str1 != '\0') {
        if (*str1++ != *str2++) {
            match++;
        }
    }
    return match < 3;
}
