// ===========================================================================
// champylight.c - a simple program that sets the color on a DMX device using
//                 Values fetched from the web.
//
// Code based on DMXWHEEL project - www.dmxwheel.com.
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h> 
#include <unistd.h>

#include "constants.h"
#include "utils.h"
#include "dmx.h"                              // DMX interface library
#include "curlClient.h"
#include "dmxOperations.h"
#include "champylight.h"

/**
 *  Aim of the main function:
 *  - init the dmx connection on start.
 *  - run fetch-decode-execute cycles in order to update the DMX values. NB: if 
 *    fetched values are the same as on the last cycle, the decode-execute part
 *    of the cycle is dropped.
 *  - closes the dmx connection on exit.
 */
int main( int argc, char *argv[]) {

    uint8_t *webValues = calloc(WEB_SIZE, sizeof(uint8_t));
    uint8_t *shmValues = NULL;

    // Initialize DMX connection
    int error = initDMX(&shmValues);
    if (error < 0) return (error);

    // Main program loop
    while (isRunning()) {
   
        bool areNewValues = getWebValues(webValues);
        
        if (areNewValues) {
            dmx_exec_t *exec = decodeDMX(webValues);
            executeDMX(exec, shmValues);
        }

        // Suspend before next update
        sleep_ms(REFRESH_TI);
    }
    
    // Terminate
    exitDMX(webValues);

    return EXIT_SUCCESS;
}

/**
 *  Initialises the DMX connection, i.e. the shared memory segment which is the
 *  way the program communicates with the deamon that sends the values to the
 *  spotlights.
 */
int initDMX(uint8_t **shmValues) {

    // Open DMX interface
    int success = dmxOpen(shmValues);
    if (success < 0) return success;

    // Return valid status
    return 0;
}

/**
 *  Closes the DMX connection, and frees any variable on the heap.
 */
void exitDMX(uint8_t *webValues) {

    // Blackout
    memset(webValues, 0, CH_COUNT);
    free(webValues);

    // Close the DMX connection
    dmxClose();
}
