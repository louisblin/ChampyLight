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

#include "constants.h"
#include "utils.h"

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

uint8_t* arrcp(uint8_t const *src, size_t length) {

    uint8_t *p = malloc(length * sizeof(uint8_t));
    memcpy(p, src, length * sizeof(uint8_t));
    return p;
}

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

