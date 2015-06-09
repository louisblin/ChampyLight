// ===========================================================================
// champylight.c - a simple program that sets the color on a DMX device using
//                 values fetched from the web.
//
// Code based on DMXWHEEL project - www.dmxwheel.com.
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h> 
#include <unistd.h>

#include "curlClient.h"
#include "constants.h"
#include "dmx.h"                              // DMX interface library
#include "champylight.h"

int main( int argc, char *argv[]) {

    // Initialize DMX connection
    int error = initDMX();
    if (error < 0) return (error);

    uint8_t *values = calloc(CHANNELS_COUNT, sizeof(uint8_t));
    printf("values[0] = %d\n", values[0]);

    // Main program loop
    while (isRunning()) {
   
        //getWebValues(values);
        //setDMXColor(0, CHANNELS_COUNT, values);

        // Suspend before next update
        sleep(REFRESH_TI);
    }
    
    // Terminate
    exitDMX(values);

    return EXIT_SUCCESS;
}

int initDMX() {

    // Open DMX interface
    int success = dmxOpen();
    if (success < 0) return success;

    // Return valid status
    return 0;
}

void setDMXColor(unsigned int fromCh, int count, uint8_t values[]) {
    
    // Set the channel colors
    for (int ch = fromCh + CHAN_INDEX; ch < count; ch++) {
        dmxSetValue(ch, values[ch]);
    }
}

void exitDMX(uint8_t *values) {

    // Blackout
    memset(values, 0, CHANNELS_COUNT);
    free(values);

    // Close the DMX connection
    dmxClose();
}
