#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

char *read_data(char *file_path) 
{
    FILE *fp = NULL;
    char *buffer = NULL;
    int N = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL) {
        printf("[read_data()] fopen() failed to open \"%s\".\n", file_path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    N = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(N * sizeof(char));                  
    fread(buffer, sizeof(buffer[0]), N, fp);

    return buffer;
}

int isnumber(char *str, int pos) 
{
    int i = pos;
    int num = 0;
    while(isdigit(str[i])) {
        num = 10*num + (int)str[i];
    }
    return num;
}

int part_one(char *input) 
{
    int rows = 2;
    int cols = (int)(strlen(input) / 2);
    uint64_t times[cols];
    int t = 0;
    uint64_t distances[cols];
    int d = 0;

    // Parse the table
    for(int row = 0; row < rows; row++) {
        for(int col = 10; col < cols; col++) {
            char *p = &input[cols*row + col];
            if(isdigit(*p)) {
                uint64_t num = 0;
                while(isdigit(*p)) {
                    num = 10*num + (*p - '0');
                    p++;
                    col++;
                }
                if(row == 0) times[t++] = num;
                if(row == 1) distances[d++] = num;
            }
        }
    }
    if(t != d) {
        return -1;
    }

    uint64_t margin_of_error = 1;
    for(int race = 0; race < t; race++) {
        uint64_t speed = 0;
        uint64_t distance = 0;
        uint64_t num_of_ways = 0;
        //printf("Race %d:\n", race);
        for(uint64_t button_time = 0; button_time < times[race]+1; button_time++) {
            speed = button_time;
            uint64_t time = times[race] - button_time;
            distance = speed * time;
            if(distance > distances[race]) {
                num_of_ways++;
            }
            //printf("        speed:%ld time:%ld\n", speed, time);
            //printf("    Button pressed for %dms, distance is %ldms\n", button_time, distance); 
        }
        //printf("    #number of ways to win is %ld\n", num_of_ways);
        margin_of_error *= num_of_ways;
    }
    printf("\nMargin of error is %ld\n", margin_of_error);
    
	return margin_of_error;
}

int main(int argc, char **argv) 
{
    char *input = NULL;
    if(argc > 1 && ((input = read_data(argv[1])) != NULL)) {
        printf("%s\n", input);       
    } else {
		printf("[Main]: Couldn't open file argv[1].\n");
		return EXIT_FAILURE;
	}
    printf("_____________________________________\n\n");
    part_one(input);
	free(input);
	return EXIT_SUCCESS;
}