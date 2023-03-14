#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// This code is based off of Jeff Zarnett's "SE 350 Exercise: There and Back Again". 
// If re-sharing this code is not allowed per University of Waterloo guidelines, please let me know.

/* Number of dishes that fit on the table */
#define TABLE_SPACE 5

typedef struct {
    int dish; /* The identifier of the dish currently on this platter */
    int uses; /* The number of times a dwarf has requested the dish on this platter */ 
} platter;

/* The platters for serving the dishes during this feast */
platter* platters;

/* Gets the next dish order from the very hungry dwarves.
   Returns -1 if we have (finally) fed the dwarves all 
   they could eat (reached the end of input).
 */
int get_next_dish_order();

/* Prints to the console, in a (relatively) pretty format,
   the contents of the platters currently on the table. 
   Run this after each step of your algorithm to see what's
   going on on the table. Helps you visualize what is going
   on, even if dishes have numbers instead of names
 */
void print_state();

/*  Serve dishes to the dwarves based on their requests. The 
    replacement strategy is LEAST FREQUENTLY USED (LFU).
    This function returns an integer count of the number of 
    times Bilbo has had to fetch a dish to put on one of the platters.
    Remember to call print_state() after each time a dish request
    is dealt with! 
*/
int serve_dishes() {

    int fetch_count = 0;
    int next_dish = 0;
    while (next_dish = get_next_dish_order(), next_dish != -1) {
        // check if the dish exists on the table:
        bool isOnTable = false;
        for (int i = 0; i < 5; i++) {
            if (platters[i].dish == next_dish) {
                isOnTable = true;
                platters[i].uses += 1;
                break;
            }
        }
        if (!isOnTable) {
            fetch_count++;
            // Find the least frequently used dish:
            int minCount = platters[0].uses;
            int indexOfRemoval = -1;
            for (int i = 4; i >= 0; i--) {
                if (platters[i].uses <= minCount) {
                    indexOfRemoval = i;
                    minCount = platters[i].uses;
                }
            }
            platters[indexOfRemoval].dish = next_dish;
            platters[indexOfRemoval].uses = 0;
        }
        print_state();
    }
    return fetch_count;
}

int main( int argc, char** argv ) {

    /* Allocate and initialize platters */
    platters = malloc(sizeof(platter) * TABLE_SPACE);
    for (int i = 0; i < 5; i++){
        platter temp;
        temp.dish = -1;
        temp.uses = -1; 
        platters[i] = temp;
    }
    
    int fetches = serve_dishes(); 
    printf("Bilbo had to fetch dishes %d times using LFU.\n", fetches );
    
    /* Deallocate platters */
    free(platters);

    return 0;
}


/****** Do not change anything below this line -- Internal Functionality Only ********/ 

int get_next_dish_order( ) {
    if ( feof(stdin) ) {
        return -1;
    }
    int next;
    scanf("%d", &next);
    return next;
}

void print_state( ) { 
    printf("("); 
    for ( int i = 0; i < TABLE_SPACE - 1; i++ ) { 
        if (platters[i].uses == -1) {
            printf("- [], ");
        } else {
            printf( "%d [%d], ", platters[i].dish, platters[i].uses );
        }
    }
    if ( platters[TABLE_SPACE - 1].uses == -1 ) {
        printf("- [])\n");
    } else {
        printf("%d [%d])\n", platters[TABLE_SPACE - 1].dish, platters[TABLE_SPACE -1].uses );
    }
}
