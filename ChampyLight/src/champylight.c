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

int main( int argc, char *argv[]) {

    uint8_t *webValues = calloc(WEB_SIZE, sizeof(uint8_t));
    uint8_t *shmValues = NULL;

    // Initialize DMX connection
    int error = initDMX(&shmValues);
    if (error < 0) return (error);

    // Main program loop
    while (isRunning()) {
   
        getWebValues(webValues);
        dmx_exec_t *exec = decodeDMX(webValues);
        executeDMX(exec, shmValues);

        // Suspend before next update
        sleep_ms(REFRESH_TI);
        break;
    }
    
    // Terminate
    exitDMX(webValues);

    return EXIT_SUCCESS;
}

int initDMX(uint8_t **shmValues) {

    // Open DMX interface
    int success = dmxOpen(shmValues);
    if (success < 0) return success;

    // Return valid status
    return 0;
}

void exitDMX(uint8_t *webValues) {

    // Blackout
    memset(webValues, 0, CH_COUNT);
    free(webValues);

    // Close the DMX connection
    dmxClose();
}
