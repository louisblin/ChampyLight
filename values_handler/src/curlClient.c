/**
 * @file curlClient.c
 * @author Louis Blin
 * @date June 2015
 *
 * @brief Handles the cURL requests made to the server in order to fetch the 
 * values for the spotlights.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdbool.h>

#include "constants.h"
#include "curlClient.h"
#include "utils.h"
#include "dmx.h"

/**
 * @brief Fetches the values from the web, and run consistency checks
 *
 * @Param values an array containing the last values fetched. 
 * @Return Returns `true` if fetched values are new, false otherwise or if an
 * error occured.
 */
bool getWebValues(uint8_t *values) {

    bool success = fetchValues();
    if (!success) {
        return false;
    }
    
    return parseAndStoreFile(values);
}

/**
 * @brief Fetches the values stored on the web interface at `REMOTE_ADDR` with
 * libcurl cURL requests, and stores them into a temporary output file.
 * 
 * @Return Returns `false` if an error occured.
 */
bool fetchValues() {
    
    CURL *curl = NULL;
    CURLcode res = -1;
    FILE *stream = NULL;
   
    if((stream = freopen(TCP_OUT, "w", stdout)) == NULL) {
        fprintf(stderr, "\n\nCouldn't redirect stdout...\n");
        switch_to_idle_state();
        exit(EXIT_FAILURE);
    }
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, REMOTE_ADDR);
        
        // If redirection, follow it
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // If error, keep the original values
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return false;
        }
 
        // Always cleanup
        curl_easy_cleanup(curl);
    }
    
    // Redirect stdout to console
    stream = freopen("/dev/tty", "a", stdout);

    #ifdef VERBOSE
    printf("\n\nStream captured from the %s:\n", REMOTE_ADDR);
    #endif

    return true;
}

/**
 * @brief Parses the fetched values from the output file, and stores them 
 * into `values`. 
 *
 * If writing to file fails, or if some values are out of range, an exception
 * is raised on stderr.
 *
 * @Param values an array containing the values just fetched. 
 * @Return Returns `true` on success and if values are new, `false` otherwise.
 */
bool parseAndStoreFile(uint8_t *values) {

    bool isNew = false;
    uint8_t *tmp_values = arrcp(values, WEB_SIZE);

    FILE *fp = NULL;
    if ((fp = fopen(TCP_OUT, "r")) == NULL) {
        fprintf(stderr, "\n\nparseAndStoreFile: error opening file\n");
        switch_to_idle_state();
        exit(EXIT_FAILURE);
    }

    char buffer[4];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            
        int intNb = atoi(buffer);

        #ifdef VERBOSE
        printf("-%d-", intNb);
        #endif

        // All channels read
        if (count >= WEB_SIZE) {
            break;
        }
        
        // Ignore entire stream if number out of range
        if (intNb < 0 || intNb > DEPTH) {
            fprintf(stderr, "parseAndStoreFile: Number format of %d. \
                    Ignoring this stream...\n", intNb);
            isNew = false;
            printSHMState();
            goto finally;
        }

        // Is this value new?
        uint8_t currValue = atoi(buffer);
        if (tmp_values[count] != currValue) { // Value is new
            isNew = true;
        }

        tmp_values[count++] = (uint8_t) atoi(buffer);
    }

    // Replace values by tmp_values
    for (int i = 0; i < WEB_SIZE; i++) {
        values[i] = tmp_values[i];
    }

    #ifdef VERBOSE
    printf("\nTOTAL READ : %d\n\n", count);
    printSHMState();
    #endif

    goto finally;

finally:
    fclose(fp);
    free(tmp_values);

    return isNew;
}
