#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "constants.h"
#include "curlClient.h"

//#define BEBUG_TCP

#ifdef BEBUG_TCP
int main(void) {
    getWebValues(NULL);
    return 0;
}
#endif

void getWebValues(uint8_t *values) {
    
    CURL *curl;
    CURLcode res;

    FILE *stream ;
    if((stream = freopen(TCP_OUT, "w", stdout)) == NULL)
        exit(-1);
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, REMOTE_ADDR);
        
        // If redirection, follow it
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        
        // Check for errors 
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
 
        // Always cleanup
        curl_easy_cleanup(curl);
    }
    
    // Redirect stdout to console
    stream = freopen("/dev/tty", "a", stdout);
    
    #ifdef VERBOSE
    printf("\nStream captured from the web");
    #endif

    parseAndStoreFile(values);
}

void parseAndStoreFile(uint8_t *values) {

    FILE *fp;
    if ((fp = fopen(TCP_OUT, "r")) == NULL) {
        fprintf(stderr, "parseAndStoreFile: error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[5];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        #ifdef VERBOSE
        printf("-%d-", atoi(buffer));
        #endif

        values[count] = (uint8_t) atoi(buffer);
    }
    #ifdef VERBOSE
    printf("\nTOTAL READ : %d\n", count + 1);
    #endif

    // TODO: Log of errors with further error handling in file rw... 

    fclose(fp);
}

