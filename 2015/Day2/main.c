#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char string[1024][128];
    int count;
} Strings;

typedef struct {
    int length[1024];
    int width[1024];
    int height[1024];
    int count;
} BoxSurfaceArea;

Strings *create_strings() {
    Strings *new_strings = malloc(sizeof(Strings));
    new_strings->count = 0;
    return new_strings;
}

BoxSurfaceArea *create_boxArea() {
    BoxSurfaceArea *new_area = malloc(sizeof(BoxSurfaceArea));
    new_area->count = 0;
    return new_area;
}

BoxSurfaceArea *fill_data(Strings *input) {
    BoxSurfaceArea *wrap_paper = create_boxArea();

    for(int i = 0; i < input->count; i++) {
        char *token = strtok(input->string[i], "x");
        int counter = 0;
        while(token != NULL) {
            int num = strtol(token, NULL, 10);
            if(counter == 0) {
                wrap_paper->length[wrap_paper->count] = num;
            }
            else if(counter == 1) {
                wrap_paper->width[wrap_paper->count] = num;
            }
            else if(counter == 2) {
                wrap_paper->height[wrap_paper->count] = num;
            }
            token = strtok(NULL, "x");
            counter++;
        }
        wrap_paper->count++;
    }

    return wrap_paper;
}

int min3(int a, int b, int c) {
    int min = 0;
    min = (a < b) ? a : b; 
    min = (min < c) ? min : c; 
    return min;
}

int part_one(Strings *input) {
    BoxSurfaceArea *wrap_paper = fill_data(input);
    int sum = 0;

    for(int i = 0; i < wrap_paper->count; i++) {
        int x = wrap_paper->length[i] * wrap_paper->width[i];
        int y = wrap_paper->width[i] * wrap_paper->height[i];
        int z = wrap_paper->height[i] * wrap_paper->length[i];
        int partial_sum = 2*x + 2*y + 2*z + min3(x, y, z);
        sum += partial_sum;
    }

    return sum;
}

int part_two(Strings *input) {
    BoxSurfaceArea *wrap_paper = fill_data(input);
    int sum = 0;

    for(int i = 0; i < wrap_paper->count; i++) {
        int x = wrap_paper->length[i] + wrap_paper->width[i];
        int y = wrap_paper->width[i] + wrap_paper->height[i];
        int z = wrap_paper->height[i] + wrap_paper->length[i];
        int ribbon = 2 * min3(x, y, z);
        int bow = wrap_paper->length[i] * wrap_paper->height[i] * wrap_paper->width[i];
        int partial_sum = ribbon + bow;
        sum += partial_sum;
    }

    return sum;
}

int main(int argc, char **argv) {
    Strings *input = create_strings();
    FILE *fp = fopen("input.txt", "r");
    if((fp = fopen("input.txt", "r")) == NULL) {
        printf("Input file could not be opened./n");
        return EXIT_SUCCESS;
    }

    char buffer[128] = {0};
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(input->string[input->count++], buffer);
    }

    int sum = 0;
    if(strcmp(argv[1], "1") == 0) {
        sum = part_one(input);
        printf("__________________PART 1__________________\n");
        printf("Final wrap paper surface area is = %d sqft\n\n", sum);

    }
    if(strcmp(argv[1], "2") == 0) {
        sum = part_two(input);
        printf("__________________PART 2__________________\n");
        printf("Final ribbon feet is = %d ft\n\n", sum);
    }
    return EXIT_SUCCESS;
}