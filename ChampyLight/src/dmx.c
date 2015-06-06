// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
//
// Original code from www.dmxwheel.com
// ==========================================================================

#include <stdio.h>
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

int dmxOpen() {

    // Get the shared memory created by the deamon
    shmid = shmget(0x56444D58, sizeof(uint8_t) * SHM_SIZE, 0666);

    if (shmid == -1) {
        printf("error[%d] - is dmxd running?\n", errno);
        return errno;
    }

    // Set up control and data registers
    shm = (uint8_t*) shmat (shmid, NULL, 0);

    maxChanAddr  = (uint16_t*) shm;
    exitAddr     = (uint8_t*) maxChanAddr + EXIT_INDEX;
    chanData     = (uint8_t*) maxChanAddr + CHAN_INDEX;
    *maxChanAddr = CHANNELS_COUNT;

#ifdef VERBOSE
    printSHM(CHANNELS_COUNT, shm);
#endif

    return 0;
}

void dmxClose() {
    if (shmid != -1) {
        shmdt(shm);
    }
}

void dmxSetValue(uint8_t channel, uint8_t data) {
    chanData[channel] = data;
}

uint8_t dmxGetValue(uint8_t channel) {
    return chanData[channel];
}

bool isRunning() {
    return *exitAddr == 0;
}
