#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DEBUG 0

// 2D array of strings that holds each line in input.txt
#define N 16384
#define M 128
char input_list[N][M] = {{0}};
int input_count = 0;

// List of valid input numbers
char numbers[9][6] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};

int charToInt(char c) {
    return (c - '0');
}

char intToChar(int x) {
    return (x + '0');
}

int firstPart(char input[N][M]) {
    int sum = 0;
    int flag = 0;
    int pass_counter = 0;

    for(int i = 0; i < input_count; i++) {
        int first_digit = -1;
        int last_digit = -1;
        for(int j = 0; j < strlen(input[i]); j++) {
            if(isdigit(input[i][j]) && !flag) {
                flag = 1;
                first_digit = charToInt(input[i][j]);
            } else if(isdigit(input[i][j]) && flag){
                last_digit = charToInt(input[i][j]);
            }
        }
        if(first_digit != -1 && last_digit != -1) {
            int partial_sum = first_digit*10 + last_digit;
            sum += partial_sum;
            pass_counter++;
            if(DEBUG) {
                printf("%4d: %2d | %2d = %2d\n", 
                    (i+1), first_digit, last_digit, partial_sum);
            }
        } else if(last_digit == -1) {
            int partial_sum = first_digit*10 + first_digit;
            sum += partial_sum;
            pass_counter++;
            if(DEBUG) {
                printf("%4d: %2d | %2d = %2d #\n", 
                    (i+1), first_digit, first_digit, partial_sum);
            }
        }
        flag = 0;
    }
    printf("pass_counter=%d\n", pass_counter);
    return sum;
}

int secondPart(char input[N][M]) {
    int sum = 0;
    int pass_counter = 0;

    for(int i = 0; i < input_count; i++) {
        char new_input[128] = {0};
        int pos = 0;
        for(int j = 0; j < strlen(input[i]); j++) {
            if(isdigit(input[i][j])) {
                new_input[pos++] = input[i][j];
            }
            else {
                for(int k = 0; k < 9; k++) {
                    // This is so bad lmao
                    if(input[i][j] == numbers[k][0] && 
                       input[i][j+1] == numbers[k][1] && 
                       input[i][j+2] == numbers[k][2]) {
                        new_input[pos++] = intToChar(k+1);
                        break;
                    }
                }
            }
        }
        int num_digits = pos - 1;
        if(num_digits == 0) {
            int first_digit = charToInt(new_input[0]);
            int partial_sum = first_digit*10 + first_digit;
            sum += partial_sum;
            pass_counter++;
            if(DEBUG) {
                printf("%4d: %2d | %2d = %2d #\n", 
                    (i+1), first_digit, first_digit, partial_sum);
            }
        }
        else if(num_digits > 0){
            int first_digit = charToInt(new_input[0]);
            int last_digit = charToInt(new_input[num_digits]);
            int partial_sum = first_digit*10 + last_digit;
            sum += partial_sum;
            pass_counter++;
            if(DEBUG) {
                printf("%4d: %2d | %2d = %2d \n", 
                    (i+1), first_digit, last_digit, partial_sum);
            }
        }
        //printf("%4d: %1d %5s \n", i+1, strlen(new_input), new_input);
    }
    printf("pass_counter=%d\n", pass_counter);
    return sum;
}


int main(int argc, char **argv) {
    FILE *fp_input;
    if((fp_input = fopen("input.txt", "r")) == NULL) {
        printf("Input file could not be opened./n");
        return EXIT_SUCCESS;
    }

    char buffer[128] = {0};
    while(fgets(buffer, sizeof(buffer), fp_input) != NULL) {
        strcat(input_list[input_count++], buffer);
    }
    
    if(strcmp(argv[1], "1") == 0) {
        int res = firstPart(input_list);
        printf("\n_________________PART ONE_________________\n");
        printf("Sum of all calibration values is ");
        printf("= %d\n\n\n", res);
    }
    else if(strcmp(argv[1], "2") == 0) {
        int res = secondPart(input_list);
        printf("\n_________________PART TWO_________________\n");
        printf("Sum of all calibration values is ");
        printf("= %d\n\n\n", res);
    } else {
        printf("Enter arg 1 or 2 for part one or two.\n");
    }

    fclose(fp_input);
    return EXIT_SUCCESS;
}

