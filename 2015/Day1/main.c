#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 16384
#define DEBUG 0

int firstPart(char *input) {
    int floor = 0;
    for(int i = 0; i < BUFFER_SIZE; i++) {
        if(input[i] == '\0'){
            break;
        }
        if(input[i] == '(') {
            floor += 1;
        }
        else if(input[i] ==')') {
            floor -= 1;
        }
    }
    return floor;
}

int secondPart(char *input) {
    int floor = 0;
    for(int i = 0; i < BUFFER_SIZE; i++) {
        if(input[i] == '\0'){
            break;
        }
        if(input[i] == '(') {
            floor += 1;
        }
        else if(input[i] ==')') {
            floor -= 1;
        }
        if(DEBUG) {
            printf("%4d: %2d %c\n", i, floor, input[i]);
        }
        if(floor == -1) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    FILE *input;
    if((input = fopen("input.txt", "r")) == NULL) {
        printf("Input file could not be opened./n");
        return EXIT_SUCCESS;
    }

    char buffer[BUFFER_SIZE] = {0};
    fgets(buffer, sizeof(buffer), input);
    printf("Final floor that Santa sire will find himself is ");
    printf("= %d\n", firstPart(buffer));
    printf("Santa will enter the basement at ");
    printf("= %d", secondPart(buffer) + 1);
    fclose(input);
    return EXIT_SUCCESS;
}

