// ===========================================================================
// champylight.c - a simple program that sets the color on a DMX device using
//                 values fetched from the web.
//
// Code based on DMXWHEEL project - www.dmxwheel.com.
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "dmx.h"                              // DMX interface library
#include "champylight.h"

int main( int argc, char *argv[] ) {

    int error;

    // Initialize connection with web page
    error = initConnection();
    if (error < 0) return (error);

    // Initialize DMX connection
    error = initDMX();
    if (error < 0) return (error);

    // Main program loop
    while (isRunning()) {
    
    }
    
    // Terminate
    exitDMX();
    closeConnection(); 

    return EXIT_SUCCESS;
}

// ===========================================================================
// initConnection -- initialize web connection
// ===========================================================================

int initConnection() {

    return -1;
}

// ===========================================================================
// closeConnection -- closes web connection
// ===========================================================================

void closeConnection() {

}

// ===========================================================================
// initDMX -- initialize DMX interface
// ===========================================================================

int initDMX() {

  // Open DMX interface
  int success = dmxOpen();
  if (success < 0) return success;

  // Return valid status
  return 0;
}

// ===========================================================================
// setDMXColor -- set the color values for the DMX device
// ===========================================================================

void setDMXColor(unsigned int from, int count, double values[]) {
    
    // Set the channel colors
    for (int ch = from; ch < count; ch++) {
        dmxSetValue(ch, values[ch]);
    }
}

// ===========================================================================
// exitDMX -- terminate the DMX interface
// ===========================================================================

void exitDMX() {

    // Blackout
    double zeros[CHANNELS_COUNT] = {0};
    setDMXColor(0, CHANNELS_COUNT, zeros);

    // Close the DMX connection
    dmxClose();
}
