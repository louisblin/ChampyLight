#ifndef CONSTANTS_H
#define CONSTANTS_H

// Common values between SHM and WEB
#define CH_COUNT        512
#define DEPTH           255   // MAX value for a channel

// Shared memory
#define SHM_NO_CH_I     0     // Index of # of chans
#define SHM_EXIT_I      2     // Index of exit control reg
#define SHM_CHAN_I      3     // Index of first channel
#define SHM_SIZE        (CH_COUNT + SHM_CHAN_I)  // bytes

// Web values
#define WEB_TRANS_ID    0     // Index of transition id control reg
#define WEB_FADE_TI     1     // Index of fade time interval control reg
#define WEB_CHAN_I      2     // Index of first channel
#define WEB_SIZE        (CH_COUNT + WEB_CHAN_I)  // bytes

// Curl
#define TIMEOUT         2
#define REMOTE_ADDR     "www.doc.ic.ac.uk/~lb3214/champyvote/renderCSV.php"
#define TCP_OUT         "curl_output.txt"

// Dmx operations
#define REFRESH_TI      1.0f  // Time interval between each fetch-dec-ex cycle
#define CRES_EXP        2.85f // Exponent of crescendo coef function
#define DECR_EXP        0.35f // Exponent of decrescendo coef function
#define DMX_REFRESH_TI  100   // Time interval between to DMX packets
#define STROBE_LENGTH   3.0f  // Time length of the strobe effect          

// Utils
#define VERBOSE
#define DEBUG
#define SLOT_TO_SHOW    20
//#define BEBUG_TCP

#define MEM_PER_LINE    4     // Number of consecutive mem slots per line 

#endif
