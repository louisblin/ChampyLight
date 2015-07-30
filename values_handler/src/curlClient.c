#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "constants.h"
#include "curlClient.h"
#include "utils.h"

/**
 * Fetches the values stored on the web interface at `REMOTE_ADDR`, and stores
 * them into `values`.
 */
void getWebValues(uint8_t *values) {
    
    CURL *curl = NULL;
    CURLcode res = -1;
    FILE *stream = NULL;
   
    if((stream = freopen(TCP_OUT, "w", stdout)) == NULL) {
        fprintf(stderr, "Couldn't redirect stdout...\n");
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
            return;
        }
 
        // Always cleanup
        curl_easy_cleanup(curl);
    }
    
    // Redirect stdout to console
    stream = freopen("/dev/tty", "a", stdout);

    #ifdef VERBOSE
    printf("\nStream captured from the %s:\n", REMOTE_ADDR);
    #endif

    parseAndStoreFile(values);
}

/**
 *  Parses the fetched values, and stores them into `values`. If a values has
 *  an illegal values, it is ignored and a message is raised on stderr.
 */
void parseAndStoreFile(uint8_t *values) {

    FILE *fp = NULL;
    if ((fp = fopen(TCP_OUT, "r")) == NULL) {
        fprintf(stderr, "parseAndStoreFile: error opening file\n");
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
        
        // Ignore numbers out of range
        if (intNb < 0 || intNb > DEPTH) {
            fprintf(stderr, "parseAndStoreFile: Number format of %d\n", intNb);
            continue;
        }

        values[count++] = (uint8_t) atoi(buffer);
    }
    #ifdef VERBOSE
    printf("\nTOTAL READ : %d\n", count);
    #endif

    // TODO: Log of errors with further error handling in file rw... 

    fclose(fp);
}
