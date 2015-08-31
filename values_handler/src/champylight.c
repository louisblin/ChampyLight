/**
 * @file champylight.c
 * @author Louis Blin
 * @date June 2015
 *
 * @brief Main file for the values handler.
 *
 * This simple program runs on a device connected to the spotlights. It
 * handles the values recovery from the web, and the runs the transitions
 * between light states.
 *
 * Code using the modified version of the dmx.h library used by the
 * DMXWHEEL project - www.dmxwheel.com.
 *
 * @see www.dmxwheel.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"
#include "utils.h"
#include "dmx.h"
#include "curlClient.h"
#include "dmxOperations.h"
#include "champylight.h"

/**
 *  @brief Entry point of the values handler.
 *
 *  Aim of the main function:
 *  - init the dmx connection on start.
 *  - run fetch-decode-execute cycles in order to update the DMX values. NB: if
 *    fetched values are the same as on the last cycle, the decode-execute part
 *    of the cycle is dropped.
 *  - closes the dmx connection on exit.
 */
int main(int argc, char *argv[]) {

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
            executeDMX(shmValues, exec);
        }

        // Suspend before next update
        sleep_ms(REFRESH_TI);
    }

    // Terminate
    exitDMX(webValues);

    return EXIT_SUCCESS;
}

/**
 *  @brief Initialises the DMX connection.
 *
 *  The connection uses he shared memory segment which is to communicates with
 *  the deamon program that continuously sends the values to the spotlights.
 *
 *  @param shmValues a pointer to the array of values in the shared memory
 *  segment.
 *  @return Returns 0 on success, a negative value on failure.
 */
int initDMX(uint8_t **shmValues) {

    // Open DMX interface
    int success = dmxOpen(shmValues);
    if (success < 0) return success;

    // Return valid status
    return 0;
}

/**
 *  @brief Closes the DMX connection and frees any variable on the heap.
 *
 *  @param webValues the array containing the last fetched values.
 */
void exitDMX(uint8_t *webValues) {

    // Blackout
    memset(webValues, 0, CH_COUNT);
    free(webValues);

    // Close the DMX connection
    dmxClose();
}
