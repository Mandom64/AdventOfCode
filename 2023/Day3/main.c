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
    memset(buffer, sizeof(buffer[0]), N * sizeof(char));
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
    int cols = (input_size / rows)-1;
    rows++;

    SchematicTable *new_table = malloc(sizeof(SchematicTable));
    new_table->table = malloc(rows * sizeof(char*));
    for(int row = 0; row < rows; row++) {
        new_table->table[row] = malloc(cols * sizeof(char));
    }
    new_table->rows = rows;
    new_table->cols = cols;  

    int j = 0;
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            if(input[cols*row + col] != '\n' ) {
                new_table->table[row][col] = input[cols*row + col];
            }
        }
    }
    return new_table;
}

void destroy_table(SchematicTable *st) {
    if(st == NULL) {
        printf("destroy_table couldn't find any table\n");
        return;
    }
    for(int row = 0; row < st->rows; row++) {
        if(st->table[row] != NULL) {
            free(st->table[row]);
        }
    }
    if(st->table != NULL){
        free(st->table);
    }
    free(st);
}

typedef enum Border{
    LEFT_UP,
    LEFT_DOWN,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_SIDE,
    RIGHT_SIDE,
    UP_SIDE,
    DOWN_SIDE,
    INSIDE,
}Border;

Border check_borders(SchematicTable *st, int row, int col) {
    if(row == 0 && col == 0) { 
        return LEFT_UP;
    }
    if(row == 0 && col == st->cols-1) {
        return RIGHT_UP;
    }
    if(col == 0 && (row > 0 && row < st->rows)) {
        return LEFT_SIDE;
    }
    if(col == st->cols-1 && (row > 0 && row < st->rows)) {
        return RIGHT_SIDE;
    }
    if(row == st->rows-1 && col == 0) {
        return LEFT_DOWN;
    }
    if(row == st->rows-1 && col == st->cols-1) {
        return RIGHT_DOWN;
    }
    if(row == 0 && (col > 0 && col < st->cols)){
        return UP_SIDE;
    }
    if(row == st->rows-1 && (col > 0 && col < st->cols)) {
        return DOWN_SIDE;
    }
    return INSIDE;
}

bool is_symbol(char c) {
    const char *symbols = "-+=/!@#$%^&*";
    for(int i = 0; i < strlen(symbols); i++) {
        if(c == symbols[i]) {
            //printf("c:%c symbols[%d]=%c\n", c, i, symbols[i]);
            return true;
        }
    }
    return false;
}

bool symbol_adjacent(SchematicTable *st, int row, int col) {
    Border pos = check_borders(st, row, col);
    bool adj = false;
    switch(pos) {
        case LEFT_UP:
            if(is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row+1][col]) || 
               is_symbol(st->table[row+1][col+1])) {
                adj = true;
            }
            break;
        case RIGHT_UP:
            if(is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row+1][col]) || 
               is_symbol(st->table[row+1][col-1])) {
                adj = true;
            }
            break;
        case LEFT_SIDE:
            if(is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row+1][col]) || 
               is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row-1][col+1]) || 
               is_symbol(st->table[row+1][col+1])) {
                adj = true;
            }
            break;
        case RIGHT_SIDE:
            if(is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row+1][col])  ||
               is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row-1][col-1]) || 
               is_symbol(st->table[row+1][col-1])) {
                adj = true;
            }
            break;
        case LEFT_DOWN:
            if(is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row-1][col+1])) {
                adj = true;
            }
            break;
        case RIGHT_DOWN:
            if(is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row-1][col-1])) {
                adj = true;
            }
            break;
        case UP_SIDE:
            if(is_symbol(st->table[row+1][col]) || 
               is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row+1][col-1]) || 
               is_symbol(st->table[row+1][col+1])) {
                adj = true;
            }
            break;
        case DOWN_SIDE:
            if(is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row-1][col-1]) || 
               is_symbol(st->table[row-1][col+1])) {
                adj = true;
            }
            break;
        case INSIDE:
            if(is_symbol(st->table[row][col-1]) || 
               is_symbol(st->table[row][col+1]) || 
               is_symbol(st->table[row-1][col]) || 
               is_symbol(st->table[row+1][col])  || 
               is_symbol(st->table[row-1][col-1]) || 
               is_symbol(st->table[row-1][col+1]) || 
               is_symbol(st->table[row+1][col-1]) || 
               is_symbol(st->table[row+1][col+1])) {
                adj = true;   
            }
            break;
    }

    return adj;
}

int main(int argc, char **argv) {
    char *input = read_data(argv[1]);
    SchematicTable *st = create_table(input);
    int sum_parts = 0;

    for(int row = 0; row < st->rows; row++) {
        for(int col = 0; col < st->cols; col++) {
            char curr_number[128] = {0};
            int k = 0;
            bool sym_adj = false;
            bool number_adj = false;

            // Find each number in a row
            while(isdigit(st->table[row][col])) {
                if(symbol_adjacent(st, row, col)) {
                    sym_adj = true;
                }

                curr_number[k] = st->table[row][col];
                col++;
                k++;
            }
            if(is_symbol(st->table[row][col])) {
                
            }
            if(sym_adj) {
                int part_number = strtol(curr_number, &curr_number, 10);
                sum_parts += part_number;
                //printf("part number %d is adj\n", part_number);
            }
        }
    }
    printf("sum of parts adjacent to symbols is %d\n", sum_parts);
    free(input);
    destroy_table(st);
    return EXIT_SUCCESS;
}

/*
    Print current pos
    // printf("   %c   \n", (st->table[row-1][col]));
    // printf("%c   ", (st->table[row-1][col-1]));
    // printf("  %c\n", (st->table[row-1][col+1]));
    // printf("%c  %c  %c\n", st->table[row][col-1], st->table[row][col], st->table[row][col+1]);
    // printf("%c  ", (st->table[row+1][col-1]));
    // printf("   %c\n", (st->table[row+1][col+1]));
    // printf("   %c   \n", (st->table[row+1][col]));
    // printf("\n-------------\n");

    What is what
    // bool left = is_symbol(st->table[row][col-1]); 
    // bool right = is_symbol(st->table[row][col+1]);
    // bool up = is_symbol(st->table[row-1][col]);
    // bool down = is_symbol(st->table[row+1][col]); 
    // bool up_left = is_symbol(st->table[row-1][col-1]);
    // bool up_right = is_symbol(st->table[row-1][col+1]);
    // bool down_left = is_symbol(st->table[row+1][col-1]);
    // bool down_right = is_symbol(st->table[row+1][col+1]);
*/