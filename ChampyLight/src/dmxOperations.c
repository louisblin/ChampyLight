#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "utils.h"
#include "dmx.h"
#include "constants.h"
#include "dmxOperations.h"

dmx_exec_t* decodeDMX(uint8_t values[]) {

    dmx_exec_t *out = calloc(1, sizeof(dmx_exec_t));
    out->type = values[WEB_TRANS_ID];
    out->fade_ti = values[WEB_FADE_TI];
    out->values = &values[WEB_CHAN_I]; 

    return out;
}

void executeDMX(dmx_exec_t *in, uint8_t startValues[]) {

    if (in == NULL) {
        fprintf(stderr, "dmx_exec_t pointer was null... abording.\n");
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
            fprintf(stderr, "'Invalid transition type %d.\n", in->type);
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

void executeRegular(dmx_exec_t *in, 
                    uint8_t startValues[],
                    double (*coef_func)(const double)) {

    // Timing
    int millis_fade_ti = in->fade_ti * 1000;
    int millis_elapsed = 0;
    struct timeval t0, t1;

    // Dmx
    uint8_t *endValues = arrcp(in->values, CH_COUNT);
    uint8_t diffValues[CH_COUNT] = {0};
    uint8_t *currValues =  arrcp(startValues, CH_COUNT);

    // Computing diff (end - start)
    for (int i = 0; i < CH_COUNT; i++) {
         diffValues[i] = endValues[i] - startValues[i];
    } 

    do {
        startTime(&t0);

        // Perform dmx operation...
        for (int i = 0; i < CH_COUNT; i++) {
            currValues[i] = startValues[i] + diffValues[i] 
                        * coef_func((double) millis_elapsed / millis_fade_ti);
        }
        dmxSetValues(0, CH_COUNT, currValues);

        printf("\n\nTime elapsed is %dms... ", millis_elapsed);
        millis_elapsed += updateTime(&t0, &t1, (unsigned int) DMX_REFRESH_TI);

    } while (millis_elapsed < millis_fade_ti);

    // Set to end values
    dmxSetValues(0, CH_COUNT, endValues);
    free(endValues);
    free(currValues);
    printSHMState();
}

void executeStrobe(dmx_exec_t *in, uint8_t startValues[]) {

    bool isBlack = false;
    
    // Timing
    int millis_elapsed = 0;
    struct timeval t0, t1;

    // Dmx
    uint8_t *strobeValues = in->values;
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

        // Sleep for the time interval given: fade_ti
        millis_elapsed += updateTime(&t0, &t1, in->fade_ti);

    } while(millis_elapsed < STROBE_LENGTH * 1000);

    // Set to end values
    dmxSetValues(0, CH_COUNT, startValues);
    free(blackValues);
}

void startTime(struct timeval *t0) {
        
    // Start timer... | t0
    gettimeofday(t0, 0);
}

int updateTime(struct timeval *t0, struct timeval *t1, 
                                         unsigned int sleepTime) {
#ifdef VERBOSE
    printSHMState();
#endif

    sleep_ms(sleepTime);
    //sleep(sleepTime / 100);

    // Compute elapsed time... | t1
    gettimeofday(t1, 0);
    return (t1->tv_sec - t0->tv_sec) * 1000.0 
                + (t1->tv_usec - t0->tv_usec) / 1000.0; 
}

void sleep_ms (int millis) {

    if (millis <= 0) {
        return;
    }

#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = millis * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(millis * 1000);
#endif
}
