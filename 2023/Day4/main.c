#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char *read_data(char *file_path) {
    FILE *fp = NULL;
    char *buffer = NULL;
    int N = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL) {
        printf("Failed to open file.\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    N = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(N * sizeof(char));   
    memset(buffer, sizeof(buffer[0]), N * sizeof(char));
    fread(buffer, sizeof(buffer[0]), N, fp);

    return buffer;
}

int number(char *str, int size, int *pos) {
    int i = *pos;
    int num = 0;
    while(isdigit(str[i]) && i < size) {
        num = num*10 + (str[i] - '0');
        i++;
    }
    *pos = i;
    return num;
}

int part_one(char *input) {
    int cards = 0;
    for (int i = 0; i < strlen(input); i++) {
        if(input[i] == '\n') cards++; 
    }
    cards++;

    // Numbers in each card
    int N = (int)(strlen(input) / cards);
    int winning[N], mine[N];
    int w = 0;
    int m = 0;
    int total_points = 0;
    int points = 0;
    bool sep_found = false;
    bool first_match = true;
    for (int i = 9; i < strlen(input); i++) {


        if(input[i] == '\n') {
            for(int i = 0; i < w; i++) {
                for(int j = 0; j < m; j++) {
                    if(winning[i] == mine[j]) {
                        if(first_match) {
                            points = 1;
                            first_match = false;
                        } else {
                            points *= 2;
                        }
                    }
                }
            }
            total_points += points;
            points = 0;
            sep_found = false;
            first_match = true;
            w = 0;
            m = 0;
            i += 10;  
        } else if(input[i] == '|') {
            sep_found = true;
        }

        if(sep_found) {
            int num = number(input, strlen(input), &i);
            if(num != 0) {
                mine[m++] = num;
            }
        } else {
            int num = number(input, strlen(input), &i);
            if(num != 0) {
                winning[w++] = num;
            }
        }
    }

    return total_points;
}

int part_two(char *input) {
    char **cards = NULL;
    int rows = 0;
    int cols = 0;

    // Create the table
    for (int i = 0; i < strlen(input); i++) {
        if(input[i] == '\n') rows++; 
    }
    cols = (strlen(input) / (rows+1)) + 1;
    cards = malloc(rows * sizeof(char*));
    for(int row = 0; row < rows; row++) {
        cards[row] = malloc(cols * sizeof(char));
    }
    // Each row is one card
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            cards[row][col] = input[cols*row + col];
        }
    }

    // Instances hold how many times a card has won, including copies
    int instances[rows];
    for(int i = 0; i < rows; i++) {
        instances[i] = 1;
    }
    for(int i = 0; i < rows; i++) {
        int points = part_one(cards[i]);
        int matches = 0;
        if(points == 1) {
            matches = 1;
        } else if(points > 1){
            while(points > 1) {
                points /= 2;
                matches++;
            }
            matches++;
        } else {
            matches = 0;
        }

        if(matches > 0 ) {
            for(int z = 0; z < instances[i]; z++) {
                int next_card = i + 1;
                for(int row = next_card; row < matches + next_card; row++) {
                    instances[row]++;
                }
            }
        }
    }
    
    int total_cards = 0;
    for(int row = 0; row < rows; row++) {
        total_cards += instances[row];
        free(cards[row]);
    }
    free(cards);
    return total_cards;
}


int main(int argc, char **argv) {
    char *input = NULL;
    if((input = read_data(argv[1])) == NULL) {
        return EXIT_FAILURE;
    } 
    printf("The total part_one are %d\n", part_one(input));
    printf("Total instances are %d\n", part_two(input));
    free(input);

    return EXIT_SUCCESS;
}