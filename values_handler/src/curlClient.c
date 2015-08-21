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
 * Fetches the values stored on the web interface at `REMOTE_ADDR`, and stores
 * them into `values`.
 * Returns:
 * - true if fetched values are new.
 * - false if these values were already fetched in the last request, or
 *   fetching failed.
 */
bool getWebValues(uint8_t *values) {
    
    CURL *curl = NULL;
    CURLcode res = -1;
    FILE *stream = NULL;
   
    if((stream = freopen(TCP_OUT, "w", stdout)) == NULL) {
        fprintf(stderr, "\n\nCouldn't redirect stdout...\n");
        switch_to_idle_state(values);
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
    printf("\nStream captured from the %s:\n", REMOTE_ADDR);
    #endif

    return parseAndStoreFile(values);
}

/**
 *  Parses the fetched values, and stores them into `values`. If a values has
 *  an illegal values, it is ignored and a message is raised on stderr.
 */
bool parseAndStoreFile(uint8_t *values) {

    bool isNew = false;
    //uint8_t *values_copy = arrcp(values, CH_COUNT);

    FILE *fp = NULL;
    if ((fp = fopen(TCP_OUT, "r")) == NULL) {
        fprintf(stderr, "\n\nparseAndStoreFile: error opening file\n");
        switch_to_idle_state(values);
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
        
        // TODO
        if (intNb < 0 || intNb > DEPTH) {
            fprintf(stderr, "parseAndStoreFile: Number format of %d. \
                    Ignoring this stream...\n", intNb);
            //goto finally;
            count++;
            continue;
        }

        // Is this value new?
        uint8_t currValue = atoi(buffer);
        //if (values_copy[count] != currValue) { // Value is new
        if (values[count] != currValue) { // Value is new
            isNew = true;
        }

        //values_copy[count++] = (uint8_t) atoi(buffer);
        values[count++] = (uint8_t) atoi(buffer);
    }

    // Replace values by values_copy
    //dmxSetValues(0, CH_COUNT, values_copy);
    //for (int i = 0; i < CH_COUNT; i++) {
    //    values[i] = values_copy[i];
    //}

    #ifdef VERBOSE
    printf("\nTOTAL READ : %d\n\n", count);
    printSHMState();
    #endif

    goto finally;

    // TODO: Log of errors with further error handling in file rw... 

finally:
    fclose(fp);
    //free(values_copy);

    return isNew;
}
