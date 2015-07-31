/**
 *  utils.c - Helper functions for printing debugging purposes
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "constants.h"
#include "dmx.h"
#include "utils.h"

/**
 *  Prints the state of the shared memory to stdout in a readable way.
 */
void printSHM(uint8_t values[]) {

    printf("\nPrinting shared memory...\n");
    for (int i = 0; i < SLOT_TO_SHOW && i < SHM_SIZE; i += MEM_PER_LINE) {
        for (int j = MEM_PER_LINE - 1, index = i + MEM_PER_LINE - 1; 
             j >= 0; 
             j--, index--) {

            if (index < SHM_SIZE) {
                printf("  [%03d] = %03d  %s", index, values[index], 
                        j == 0 ? "" : "|");
            }
            else {
                printf("  [---] = ---  |");
            }
        }
        printf("\n"); 
    }
}

/**
 *  Copies the first `length` elements of `src` array in a new array and return
 *  a pointer to this array.
 */
uint8_t* arrcp(uint8_t const *src, size_t length) {

    uint8_t *p = malloc(length * sizeof(uint8_t));
    memcpy(p, src, length * sizeof(uint8_t));
    return p;
}

/** 
 *  Pauses the execution for `millis` ms.
 */
void sleep_ms (int millis) {

    if (millis <= 0) {
        return;
    }

#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = millis * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(millis * 1000);
#endif
}

/** 
 *  Called if the program has to close after an unexpected error occured.
 */
void switch_to_idle_state(uint8_t *values) {

    // Sets the spotlights to full power (better than a blackout...)
    fprintf(stderr, "+====================================================\n");
    fprintf(stderr, "| Switching %d channels to idle state with value %d.\n", 
                        CH_COUNT, DEPTH);
    fprintf(stderr, "+====================================================\n");
    
    memset((values + SHM_CHAN_I), DEPTH, CH_COUNT);

    fprintf(stderr, "\nPrinting SHM final state...\n");
    printSHMState();
}

