#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Caller must free buffer
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
	free(input);
	return EXIT_SUCCESS;
}