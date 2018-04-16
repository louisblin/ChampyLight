/**
 * @file constants.h
 * @author Louis Blin
 * @date June 2015
 *
 * @brief List of the constants used by the values handler.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Common values between SHM and WEB
/** Number on channels handled. Max is 512. */
#define CH_COUNT        60    
/** Max value for a channel. */
#define DEPTH           255   

// Shared memory
/** Index of the register for the number of channels. */
#define SHM_NO_CH_I     0     
/** Index of the register for the exit control. */
#define SHM_EXIT_I      2     
/** Index of the register for the first channel. */
#define SHM_CHAN_I      3     
/** Size (byte) of the shared memory segment (SHM). */
#define SHM_SIZE        (CH_COUNT + SHM_CHAN_I)  

// Web values
/** Index of the register for the transition id. */
#define WEB_TRANS_ID    0     
/** Index of the register for the fade time interval. */
#define WEB_FADE_TI     1     
/** Index of the register for the first channel. */
#define WEB_CHAN_I      2     
/** Size (byte) of the array of web values. */
#define WEB_SIZE        (CH_COUNT + WEB_CHAN_I)  

// Curl
/** URL where the web values are fetched.*/
#define REMOTE_ADDR     "champylight.estivales-puisaye.com/web/render"  
// Debug address
//#define REMOTE_ADDR     "http://localhost:8000/render"                
/** Name of the temp file storing the web value. */
#define TCP_OUT         "curl_output.txt"                               

// Dmx operations
/** Time interval (TI) between 2 fetch-decode-execute cycles (MILLIS). */
#define REFRESH_TI      1000  
/** Exponent of the crescendo coef function. */
#define CRES_EXP        2.85f 
/** Exponent of the decrescendo coef function. */
#define DECR_EXP        0.35f 
/** Time interval between 2 DMX packets (MILLIS). */
#define DMX_REFRESH_TI  100   
/** Length of a period of the strobe (MILLIS). */
#define STROBE_PERIOD   100   

// Utils
//#define VERBOSE             // Used for
//#define DEBUG               //    debugging purposes...

/** Number of slots display in the memory dump.*/
#define SLOT_TO_SHOW    512   
/** Number of consecutive mem slots per line. */
#define MEM_PER_LINE    4     

#endif


// Documentation main page

/*! 
 * \mainpage Values Handler
 *
 * \section Presentation
 *
 * This program fetches the values for the spolights on the web and handles the
 * transition between different spotlight states.
 *
 * \section Notes
 *
 * Tools:
 * cURL request performed using the libcurl library to fetch the values from the 
 * web interface. 
 * 
 * Edge cases behaviour:
 * - No service: leave the spotlights with their original values.
 * - Invalid range of value [0..255]: ignore new stream.
 * - Invalid transition type {linear, cres, decres, cres-decres}: set transition to default (linear).
 * - Invalid fade time interval [1..255]: set it to 1.
 */

