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

    // Numbers in each card
    int N = (int)(strlen(input) / cards);
    int win_nums[N], my_nums[N];
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
                    if(win_nums[i] == my_nums[j]) {
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
                my_nums[m++] = num;
            }
        } else {
            int num = number(input, strlen(input), &i);
            if(num != 0) {
                win_nums[w++] = num;
            }
        }
    }

    return total_points;
}

void table_insert(char **table, int *size, int pos, char *str) {
    *size++;
    for(int i = *size - 1; i >= pos; i--) {
        table[i] = table[i-1];
    }
    table[pos-1] = str;
}

int part_two(char *input) {
    int input_len = strlen(input);
    char **cards = NULL;
    int rows = 0;
    int cols = 0;
    for (int i = 0; i < strlen(input); i++) {
        if(input[i] == '\n') rows++; 
    }
    cols = (input_len / (rows+1)) + 1;

    cards = malloc(rows * 5 * sizeof(char*));
    for(int row = 0; row < rows*5; row++) {
        cards[row] = malloc(cols * sizeof(char));
    }
    printf("rows = %d cols = %d\n", rows, cols);
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            cards[row][col] = input[cols*row + col];
        }
    }

    int wins = 0;
    for(int i = 0; i < rows; i++) {
        int points = part_one(cards[i]);
        int matching_numbers = 0;
        if(points == 1) {
            matching_numbers = 1;
        } else {
            while(points > 1) {
                points /= 2;
                matching_numbers++;
            }
            matching_numbers++;
        }
        if(matching_numbers > 0) {
            wins++;
            // Duplicate matching numbers of next cards 
            // idea: append to the end and sort after?
            for(int row = i; row < matching_numbers; row++) {
                //table_insert(cards, &rows, row, cards[row]);
                //rows++;
                for(int z = rows - 1; z >= row; z--) {
                    cards[z+1] = cards[z];
                }
                cards[row-1] = cards[row];
                // str_sort(cards[5]);
            }
        }
        printf("matching numbers = %d\n", matching_numbers);
    }
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            printf("%c", cards[row][col]);
        }
        putchar('\n');
    }
    printf("wins=%d\n", wins);
    return wins;
}



int main(int argc, char **argv) {
    char *input = NULL;
    if((input = read_data(argv[1])) == NULL) {
        return EXIT_FAILURE;
    } 
    printf("The total part_one are %d\n", part_one(input));
    part_two(input);
}