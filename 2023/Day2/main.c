#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int countDigits(int num) {
    int digits = 1;
    while(num /= 10) {
        digits++;
    }

    return digits;
}

int firstNumber(char *str) {
    char tmp[128] = {0};
    int j = 0;
    bool found = false;

    for(int i = 0; str[i]; i++) {
        while(isdigit(str[i])) {
            tmp[j] = str[i];
            i++;
            j++;
            found = true;
        }
        if(found) {
            return strtol(tmp, &tmp, 10);
        }
    }

    return -1;
}

char cube_color(char *str) {
    if(strstr(str, "red") != NULL) {
        return 'r';
    }
    if(strstr(str, "green") != NULL) {
        return 'g';
    }
    if(strstr(str, "blue") != NULL) {
        return 'b';
    }
}



int main(int argc, char **argv) {
    FILE *fp = NULL;
    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("File failed to open.\n");
        return EXIT_FAILURE;
    }

    char game[512] = {0};
    char *sp, *rest;
    bool game_pass;
    int game_id = 1;
    int id_sum = 0;
    int pow_sum = 0;

    while(fgets(game, sizeof(game), fp) != NULL) {
        game_pass = true;
        int max_rcubes = 0;
        int max_gcubes = 0;
        int max_bcubes = 0;
        //printf("%3d %d: %s", game_id, strlen(game), game);

        for(sp = strtok_r(game, ",;", &rest); sp != NULL; sp = strtok_r(NULL, ",;", &rest)) {
            int cubes;
            char color = cube_color(sp);
            if(sp[0] == 'G') {
                cubes = firstNumber(&sp[countDigits(game_id)+6]);
            }
            else {
                cubes = firstNumber(sp);
            }
            switch (color) {
                case 'r':
                    if(cubes > max_rcubes) {
                        max_rcubes = cubes;
                    }
                    if(cubes > 12) {
                        game_pass = false;
                    }
                    break;
                case 'g':
                    if(cubes > max_gcubes) {
                        max_gcubes = cubes;
                    }
                    if(cubes > 13) {
                        game_pass = false;
                    }
                    break;
                case 'b':
                    if(cubes > max_bcubes) {
                        max_bcubes = cubes;
                    }
                    if(cubes > 14) {
                        game_pass = false;
                    }
                    break;
            }
            //printf("\n     /%s", sp);
        }

        //printf("\n  //%s\n", game_pass? "pass" : "fail");
        if(game_pass) {
            id_sum += game_id;
        }
        pow_sum += max_rcubes * max_gcubes * max_bcubes;
        game_id++;
    }
    
    printf("\npart_1 id_sum = %d\n", id_sum);
    printf("part_2 pow_sum = %d\n\n", pow_sum);
    return EXIT_SUCCESS;
}