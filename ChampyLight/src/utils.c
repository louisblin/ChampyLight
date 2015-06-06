/**
 *  utils.c - Helper functions for printing debugging purposes
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h>

#include "utils.h"

void printSHM(int count, uint8_t values[]) {

    printf("Printing shared memory...");
    for (int i = 0; i < count; i++) {
        printf("%03d | %08d \n", count, values[i]);
    }
}


