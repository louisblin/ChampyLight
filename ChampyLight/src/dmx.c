// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
//
// Original code from www.dmxwheel.com
// ==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "utils.h"
#include "dmx.h"

uint16_t *maxChanAddr;    // control register for # of channels to update
uint8_t  *exitAddr;       // control register to exit deamon
uint8_t  *chanData;       // 512 byte array of channel data

uint8_t  *shm;            // shared memory segment containing data & ctrl regs
int      shmid = -1;      // handel to shared memory segment

int dmxOpen(uint8_t **shmValues) {

#ifndef DEBUG
    // Get the shared memory created by the deamon
    shmid = shmget(0x56444D58, sizeof(uint8_t) * SHM_SIZE, 0666);

    if (shmid == -1) {
        printf("error[%d] - is dmxd running?\n", errno);
        return errno;
    }

    shm = (uint8_t*) shmat (shmid, NULL, 0);

#else  // DEBUG MODE: simulating shared memory segment
    
    shm = (uint8_t*) calloc(SHM_SIZE, sizeof(uint8_t));

#endif

    // Warning: if you modify the size of these parameters, set accordingly
    // the #defines SHM_* for them.
    maxChanAddr  = (uint16_t*) shm + SHM_NO_CH_I;
    exitAddr     = (uint8_t*)  shm + SHM_EXIT_I;
    chanData     = (uint8_t*)  shm + SHM_CHAN_I;
    
    *shmValues   = chanData;
    *maxChanAddr = CH_COUNT;
    
    #ifdef VERBOSE
    printSHMState();
    #endif
    
    return 0;

}

void dmxClose() {
    if (shmid != -1) {
        shmdt(shm);
    }

#ifdef DEBUG
    free(shm);
#endif
}

void dmxSetValue(uint8_t channel, uint8_t data) {
    chanData[channel] = data;
}

void dmxSetValues(unsigned int fromCh, int count, uint8_t values[]) {
    
    // Set the channel colors
    for (int ch = fromCh; ch < count; ch++) {
        chanData[ch] = values[ch];
    }
}

uint8_t dmxGetValue(uint8_t channel) {
    return chanData[channel];
}

bool isRunning() {
    
    #ifdef VERBOSE
    printf("isRunning = 1\n");
    #endif
    return 1;

    return *exitAddr == 0;
}

#ifdef VERBOSE
void printSHMState() {
    printSHM(shm);
}
#endif
