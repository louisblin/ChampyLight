#ifndef CONSTANTS_H
#define CONSTANTS_H

//                      0        Index of # of chans
#define EXIT_INDEX      2     // Index of exit control reg
#define CHAN_INDEX      3     // Index of first channel
#define CHANNELS_COUNT  512   // # of DMX channels used
#define SHM_SIZE        (CHANNELS_COUNT + CHAN_INDEX)  // bytes

#define VERBOSE               // Debug

#endif
