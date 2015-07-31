#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>

#include "utils.h"
#include "dmx.h"
#include "constants.h"
#include "dmxOperations.h"

// Static prototypes
inline static double lin(const double t);
inline static double cres(const double t);
inline static double decr(const double t);

/**
 *  Decode the fetched values and creates a `dmx_exec_t` structure that represents
 *  a transition. This object is then returns to the caller.`
 *  If the transition fade time interval is out of range, it is set to 1.
 */
dmx_exec_t* decodeDMX(uint8_t values[]) {

    dmx_exec_t *out = calloc(1, sizeof(dmx_exec_t));
    
    // All the following values are in the range [0..255], after the parsing
    // in curlClient.c

    // If not in range, handled in executeDMX...
    out->type = values[WEB_TRANS_ID];
    
    // If not in range [1..255], set fade_ti to 1...
    out->fade_ti = values[WEB_FADE_TI];
    if (out->fade_ti < 1) {
        out->fade_ti = (uint8_t) 1;
    }

    // Already all in range
    out->values = &values[WEB_CHAN_I]; 

    return out;
}

/**
 *  Execute the transition, given the transition structure `in` and the current
 *  values of the spotlights before the transition start `startValues`.
 */
void executeDMX(dmx_exec_t *in, uint8_t startValues[]) {

    if (in == NULL) {
        fprintf(stderr, "dmx_exec_t pointer was null... abording.\n");
        switch_to_idle_state(startValues);
        exit(EXIT_FAILURE);
    }

    uint8_t *startValuesCopy = arrcp(startValues, CH_COUNT);

    switch(in->type) {
        case LINEAR:
            executeRegular(in, startValuesCopy, &lin);
            break;
        case CRES:
            executeRegular(in, startValuesCopy, &cres);
            break;
        case DRES:
            executeRegular(in, startValuesCopy, &decr);
            break;
        //case CRES_DECRES:

        case STROBE:
            executeStrobe(in, startValuesCopy);
            break;

        default:
            fprintf(stderr, "'Invalid transition type %d... Using default(linear).\n", in->type);
            executeRegular(in, startValuesCopy, &lin);
    }

    free(startValuesCopy);
    free(in);
}

// Helper function: type related execution

inline static double lin(const double t) {
    return t;
}

inline static double cres(const double t) {
    return round(pow(t, CRES_EXP));
}

inline static double decr(const double t) {
    return round(pow(t, DECR_EXP));
}

/**
 *  Execute a regular transition (i.e. with a transition speed that a function
 *  can represent).
 */
void executeRegular(dmx_exec_t *in, 
                    uint8_t startValues[],
                    double (*coef_func)(const double)) {

    // Timing
    int millis_fade_ti = in->fade_ti * 1000;
    int millis_elapsed = 0;
    struct timeval t0, t1;

    // Dmx
    uint8_t *endValues = arrcp(in->values, CH_COUNT);
    int16_t diffValues[CH_COUNT] = {0};
    uint8_t *currValues =  arrcp(startValues, CH_COUNT);

    // Computing diff (end - start)
    printf("diffValues: ");
    for (int i = 0; i < CH_COUNT; i++) {
        diffValues[i] = ((int16_t) endValues[i]) - ((int16_t) startValues[i]);
    }

    do {
        startTime(&t0);

        // Perform dmx operation...
        for (int i = 0; i < CH_COUNT; i++) {
            currValues[i] = startValues[i] + diffValues[i] 
                        * coef_func((double) millis_elapsed / millis_fade_ti);
        }
        dmxSetValues(0, CH_COUNT, currValues);

        #ifdef VERBOSE
        printf("\n\nREG: Time elapsed is %dms... ", millis_elapsed);
        #endif

        millis_elapsed += updateTime(&t0, &t1, (unsigned int) DMX_REFRESH_TI);

    } while (millis_elapsed < millis_fade_ti);

    // Set to end values
    dmxSetValues(0, CH_COUNT, endValues);
    free(endValues);
    free(currValues);
}

/**
 * Executes a speical type of transition, a `special effect` that don't have any
 * similarity as regular transition have.
 */

/**
 * Executes a strobe effect: flashes the received values during in->fade_ti 
 * seconds, and at a fixed frequency of 1000/STROBE_PERIOD (NB: STROBE_PERIOD
 * is in millis).
 * At the end of the effect, the original values are restored.
 */
void executeStrobe(dmx_exec_t *in, uint8_t startValues[]) {

    bool isBlack = false;
    
    // Timing
    int millis_elapsed = 0;
    struct timeval t0, t1;

    // Dmx
    uint8_t *strobeValues = in->values;
    uint16_t strobeLength  = in->fade_ti * 1000;
    uint8_t *blackValues  = calloc(CH_COUNT, sizeof(uint8_t));

    do {
        startTime(&t0);
        
        // Perform dmx operation...
        if (isBlack) {
            dmxSetValues(0, CH_COUNT, strobeValues);
        }
        else {
            dmxSetValues(0, CH_COUNT, blackValues);
        }
        isBlack = !isBlack;

        #ifdef VERBOSE
        printf("\n\nSTOBE: Time elapsed is %dms... ", millis_elapsed);
        #endif
        
        // Sleep until next period begins
        millis_elapsed += updateTime(&t0, &t1, (unsigned int) STROBE_PERIOD);

    } while(millis_elapsed < strobeLength);

    // Set values back to their orignal ones.
    dmxSetValues(0, CH_COUNT, startValues);
    free(blackValues);
}

/**
 *  Returns the current time and stores it in t0.
 */
void startTime(struct timeval *t0) {
        
    // Start timer... | t0
    gettimeofday(t0, 0);
}

/**
 *  Updates the transition time, i.e. wait the right number of seconds between
 *  the next transition cycle.
 */
int updateTime(struct timeval *t0, struct timeval *t1, 
                                         unsigned int sleepTime) {
    #ifdef VERBOSE
    printSHMState();
    #endif

    sleep_ms(sleepTime);

    // Compute elapsed time... | t1
    gettimeofday(t1, 0);
    return (t1->tv_sec - t0->tv_sec) * 1000.0 
                + (t1->tv_usec - t0->tv_usec) / 1000.0; 
}
