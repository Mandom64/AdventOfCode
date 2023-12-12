#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* Caller must free buffer */ 
char *read_data(char *file_path) 
{
    FILE *fp = NULL;
    char *buffer = NULL;
    int N = 0;

    fp = fopen(file_path, "r");
    if(fp == NULL) {
        printf("[ERROR] fopen() failed to open \"%s\".\n", file_path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    N = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc((N+1) * sizeof(char));
    printf("[INFO] Buffer allocated %ld bytes\n", (N+1) * sizeof(char));                
    fread(buffer, sizeof(buffer[0]), N, fp);
    buffer[N] = '\0';
    fclose(fp);

    return buffer;
}

int count_lines(const char *file) 
{
    int lines = 0;
    printf("size of file: %ld\n", strlen(file));
    for(size_t i = 0; i < strlen(file); i++) {
        if(file[i] == '\n') {
            lines++;
        }
    }
    return lines;
}

typedef enum {
    high_card,
    one_pair,
    two_pair,
    three_of,
    four_of,
    full_house,
    five_of,
    invalid,
} CardType;

CardType getType(char *card) 
{
    int unique_chars[128] = {0};
    for(size_t i = 0; i < strlen(card); i++) {
        unique_chars[card[i]-'0']++; 
    }
    int uniques = 0;
    for(int i = 0; i < 128; i++) {
        if(unique_chars[i] > 0) {
            uniques++;
        }
    }

    if(uniques == 5) {
        return high_card;
    } else if(uniques == 4){
        return one_pair;
    } else if(uniques == 3) {
        int two_pair_check = 0;
        for(int i = 0; i < 128; i++) {
            if(unique_chars[i] == 2) {
                two_pair_check++;
            }
        }
        if(two_pair_check == 2) {
            return two_pair;
        } else {
            return three_of;
        }
    } else if(uniques == 2) {
        for(int i = 0; i < 128; i++) {
            if(unique_chars[i] == 4) {
                return four_of;
            }
        }
        return full_house;
    } else if(uniques == 1) {
        return five_of;
    }

    return invalid;
}

typedef struct {
    char power[8];
    int bid;
    CardType type;
    int rank;
} Hand;

typedef struct {
    Hand *hands;
    int size;
} Hands;

/* We evaluate hands in sets of 5 */
#define MAX_SETS 5

void print_hands(const Hands *h) 
{
    for(int set = 0; set < h->size; set += MAX_SETS) {
        for(int hand = 0; hand < MAX_SETS; hand++) {
            printf("Hand:%d\n    Power:%s\n    Bid:%d\n    Type:%d\n    Rank:%d\n", 
                set+hand+1, h->hands[set+hand].power, h->hands[set+hand].bid, (int)h->hands[set+hand].type, h->hands[set+hand].rank);
        }
        printf("____________________________________\n");
    }
}

int winnings(const char *input) 
{
    int input_len = count_lines(input) + 1;
    Hands *h = malloc(sizeof(Hands));
    h->hands = malloc(input_len * sizeof(Hand));
    h->size = 0;

    for(size_t i = 0; i < strlen(input); i++) {
        char power[8] = {0};
        int k = 0;
        while(input[i] !=  ' ') {
            power[k] = input[i];
            i++;
            k++;
        }
        strcpy(h->hands[h->size].power, power);

        int bid = 0;
        /* Move past the spaces to get to the bids */
        while(!isdigit(input[i])) i++;
        while(isdigit(input[i])) {
            bid = bid*10 + (input[i] - '0');
            i++;
        }
        h->hands[h->size].bid = bid;
        h->hands[h->size].type = getType(h->hands[h->size].power);
        h->size++;
    }

    /* Sort based on type */
    for(int set = 0; set < h->size; set += MAX_SETS) {
        for(int i = 0; i < MAX_SETS - 1; i++) {
            for(int j = i+1; j < MAX_SETS; j++) {
                if(h->hands[set+i].type > h->hands[set+j].type) {
                    Hand tmp = h->hands[set+i];
                    h->hands[set+i] = h->hands[set+j];
                    h->hands[set+j] = tmp;
                }
            }
        }
    }

    /* Sort based on power string */
    for(int set = 0; set < h->size; set += MAX_SETS) {
        for(int i = 0; i < MAX_SETS - 1; i++) {
            CardType curr_type = h->hands[set+i].type;
            for(int j = i+1; j < MAX_SETS; j++) {
                if(h->hands[set+j].type > curr_type) {
                    break;
                }
                if(strcmp(h->hands[set+i].power, h->hands[set+j].power) > 0) {
                    Hand tmp = h->hands[set+i];
                    h->hands[set+i] = h->hands[set+j];
                    h->hands[set+j] = tmp;
                }
            }
        }
    }


    /* Finds the ranking of each hand                     *
     * TODO: This must be fixed to get the correct result */
    for(int set = 0; set < h->size; set += MAX_SETS) {
        for(int i = 0; i < MAX_SETS; i++) {
            bool same_power = false;
            for(int j = i+1; j < MAX_SETS; j++) {
                char *power = h->hands[set+i].power;
                char *next_power = h->hands[set+j].power;
                if(h->hands[set+i].type == h->hands[set+j].type) {
                    same_power = true;
                    if(strcmp(power, next_power) > 0) {
                        printf("hand: %d| %s is stronger than %s|i:%d j:%d\n", set, power, next_power, i, j);
                        h->hands[set+i].rank = j+1;
                        h->hands[set+j].rank = i+1;
                        i++;
                        break;
                    }
                    else if(strcmp(power, next_power) < 0) {
                        printf("    hand: %d| %s is stronger than %s|i:%d j:%d\n", set, next_power, power, i, j);
                        h->hands[set+i].rank = i+1;
                        h->hands[set+j].rank = j+1;
                        i++;
                        break;
                    }
                }
            }
            if(!same_power) {
                printf("hand: %d| %s |i:%d\n", set, h->hands[set+i].power, i);
                h->hands[set+i].rank = i+1;
            }
        }
    }

    int total_winnings = 0;
    /* Accumulate total winnings from each set */ 
    for(int set = 0; set < h->size; set += MAX_SETS) {
        printf("Set:%d\n", set);
        for(int i = 0; i < MAX_SETS; i++) {
            total_winnings += h->hands[set+i].bid * h->hands[set+i].rank;
            printf("(%d * %d) + ",h->hands[set+i].bid, h->hands[set+i].rank);
        }
        printf("\n");
    }

    print_hands(h);
    if(h->hands != NULL) {
        free(h->hands);
    }
    if(h != NULL) {
        free(h);
    }

    return total_winnings;
}



int main(int argc, char **argv) 
{
    char *input = NULL;
    if(argc > 1 && ((input = read_data(argv[1])) != NULL)) {
        //printf("%s\n", input);       
    } else {
		printf("[Main]: Couldn't open file argv[1].\n");
		return EXIT_FAILURE;
	}
    printf("_____________________________________\n\n");
    printf("Total winnings: %d\n", winnings(input));
	free(input);
	return EXIT_SUCCESS;
}
