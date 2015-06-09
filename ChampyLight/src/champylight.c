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
#include <unistd.h>

#include "curlClient.h"
#include "constants.h"
#include "dmx.h"                              // DMX interface library
#include "champylight.h"

int main( int argc, char *argv[] ) {

    // Initialize DMX connection
    int error = initDMX();
    if (error < 0) return (error);

    // Main program loop
    while (isRunning()) {
   
        uint8_t values[CHANNELS_COUNT] = {0};
        fetchValues(values);
        setDMXColor(0, CHANNELS_COUNT, values);

        // Suspend before next update
        sleep(REFRESH_TI);
    }
    
    // Terminate
    exitDMX();

    return EXIT_SUCCESS;
}

void fetchValues(uint8_t values[]) {
    #ifdef VERBOSE
    printf("Fetching...\n");
    #endif
    //getWebValues(values);
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

void exitDMX() {

    // Blackout
    uint8_t zeros[CHANNELS_COUNT] = {0};
    setDMXColor(0, CHANNELS_COUNT, zeros);

    // Close the DMX connection
    dmxClose();
}
