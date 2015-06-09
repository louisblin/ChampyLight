#ifndef CONSTANTS_H
#define CONSTANTS_H

// Shared memory
//                      0        Index of # of chans
#define EXIT_INDEX      2     // Index of exit control reg
#define CHAN_INDEX      3     // Index of first channel
#define CHANNELS_COUNT  512   // # of DMX channels used
#define SHM_SIZE        (CHANNELS_COUNT + CHAN_INDEX)  // bytes

// Other
#define REFRESH_TI      1.0f
#define TIMEOUT         2
#define REMOTE_ADDR     "www.doc.ic.ac.uk/~lb3214/champyvote/renderCSV.php"
#define TCP_OUT         "curl_output.txt"

#define VERBOSE

#endif
