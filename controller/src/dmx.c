/**
 * @file dmx.c
 * @author Louis Blin
 * NB: based on Velleman K8062 DMX controller library for VM116/K8062 used in
 * the DMXWheel project - www.dmxwheel.com
 * @date June 2015
 *
 * @brief Handles the connection with the deamon program dmxd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <constants.h>
#include <utils.h>
#include <dmx.h>

/** Control register for # of channels to update. */
uint16_t *maxChanAddr;
/** Control register to exit deamon. */
uint8_t  *exitAddr;
/** 512 byte array of channel data. */
uint8_t  *chanData;

/** Shared memory segment containing data & ctrl regs. */
uint8_t  *shm;
/** Handle to shared memory segment. */
int      shmid = -1;

/**
 *  @brief Initialise the connection with the deamon.
 *
 *  Opens the dmx shared memory segment with the deamon dmxd.c.
 *
 *  @param shmValues a pointer to be assigned to the first values in the SHM.
 *  @return Returns a negative value if something went wrong.
 */
int dmxOpen(uint8_t **shmValues) {

#ifndef DEBUG
    // Get the shared memory created by the deamon
    shmid = shmget(0x56444D58, sizeof(uint8_t) * SHM_SIZE, 0666);

    if (shmid == -1) {
        printf("error[%d] - is dmxd running?\n", errno);
        return -1;
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
    printSHM(shm);
    #endif

    return 0;

}

/**
 *  @brief End connection.
 *
 *  Detaches from shared memory segment, and frees any variable still allocated.
 */
void dmxClose() {
    if (shmid != -1) {
        shmdt(shm);
    }

#ifdef DEBUG
    free(shm);
#endif
}

/**
 *  @brief Updates values in SHM.
 *
 *  Sets a continuous block of `count` channels starting at `fromCh` with the
 *  values `values[]`.
 *
 *  @param fromCh the index of the first value to update.
 *  @param count the number of channels to update.
 *  @param values the array of new values.
 */
void dmxSetValues(unsigned int fromCh, int count, uint8_t values[]) {

    // Set the channel colors
    for (int ch = fromCh; ch < count; ch++) {
        chanData[ch] = values[ch];
    }
}

/**
 * @brief Checks if the deamon is running.
 *
 * @return Returns true if the control register of the SHM indicates that the
 * deamon is running. False otherwise.
 */
bool isRunning() {

    return *exitAddr == 0;
}

/**
 *  @brief SHM dump helper.
 *
 *  Prints the shared memory using the util.c function printSHM().
 */
void printSHMState() {
    printSHM(shm);
}

/**
 *  @brief Idle state switcher helper.
 *
 *  Called if the program has to close after an unexpected error occured.
 */
void switch_to_idle_state() {
    switch_to_idle(chanData);
}

