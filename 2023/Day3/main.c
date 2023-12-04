#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char *read_data(char *file_path) {
    char *buffer = NULL;
    int N = 0;

    FILE *fp = fopen(file_path, "r");
    if(fp == NULL) {
        printf("Failed to open file.\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    N = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(N * sizeof(char));
    fread(buffer, sizeof(buffer[0]), N, fp);
    return buffer;
}

// cols are the same in each row so they don't need to be an array
typedef struct{
    char **table;
    int rows;
    int cols;
} SchematicTable;

SchematicTable *create_table(char *input) {
    int rows = 0;
    int input_size = strlen(input);
    for(int i = 0; i < input_size; i++) {
        if(input[i] == '\n') {
            rows++;
        }
    }
    int cols = (input_size / rows);

    SchematicTable *new_table = malloc(sizeof(SchematicTable));
    new_table->table = malloc(rows * sizeof(char*));
    for(int i = 0; i < rows; i++) {
        new_table->table[i] = malloc(cols * sizeof(char));
    }
    new_table->rows = rows;
    new_table->cols = cols;  

    int j = 0;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(input[j + i*cols] != '\n' ) {
                new_table->table[i][j] = input[j + i*cols];
            }
        }
    }

    return new_table;
}

bool is_symbol(char c) {
    const char *symbols = "-+=/!@#$%^&*";
    for(int i = 0; i < strlen(symbols); i++) {
        //printf("c:%c symbols[%d]=%c\n", c, i, symbols[i]);
        if(c == symbols[i]) {
            return true;
        }
    }
    return false;
}


int main(int argc, char **argv) {
    char *input = read_data(argv[1]);
    SchematicTable *st = create_table(input);
    int sum_parts = 0;

    for(int row = 0; row < st->rows; row++) {
        for(int col = 0; col < st->cols; col++) {
            char curr_number[128] = {0};
            int k = 0;
            bool adj = false;

            // Find each number in a row
            while(isdigit(st->table[row][col])) {
                if(row == 0 && col == 0) {
                    if(is_symbol(st->table[row][col+1]) ||
                       is_symbol(st->table[row+1][col]) ||
                       is_symbol(st->table[row+1][col+1])) {
                            adj = true;
                       }
                }
                if(row > 1 && row < st->rows-1 && col > 1 && col < st->cols-1) {
                    if(is_symbol(st->table[row][col-1]) || 
                    is_symbol(st->table[row][col+1]) ||
                    is_symbol(st->table[row-1][col]) ||
                    is_symbol(st->table[row+1][col]) || 
                    is_symbol(st->table[row-1][col-1]) ||
                    is_symbol(st->table[row-1][col+1]) ||
                    is_symbol(st->table[row+1][col-1]) ||
                    is_symbol(st->table[row+1][col+1])) {
                        adj = true;   

                        // printf("   %c   \n", (st->table[row-1][col]));
                        // printf("%c   ", (st->table[row-1][col-1]));
                        // printf("  %c\n", (st->table[row-1][col+1]));
                        // printf("%c  %c  %c\n", st->table[row][col-1], st->table[row][col], st->table[row][col+1]);
                        // printf("%c  ", (st->table[row+1][col-1]));
                        // printf("   %c\n", (st->table[row+1][col+1]));
                        // printf("   %c   \n", (st->table[row+1][col]));
                        // printf("\n-------------\n");
                    }
                }
                /*
                    Fill out the rest bound checks
                */  
                curr_number[k] = st->table[row][col];
                col++;
                k++;
            }
            if(adj) {
                int part_number = strtol(curr_number, &curr_number, 10);
                sum_parts += part_number;
                //printf("part number %d is adj\n", part_number);
            }
        }
    }
    printf("sum of parts adjacent to symbols is %d\n", sum_parts);
    //free(input);
    return EXIT_SUCCESS;
}