/**
    Code adapted from:
     Simple TCP client to fetch a web page
     Silver Moon (m00n.silv3r@gmail.com)
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include "constants.h"
#include "tcpclient.h"

int main(void) {
    getWebValues(NULL);
    return 0;
}

void getWebValues(uint8_t *values) {

    char arg[ARG_SIZE] = REMOTE_ADDR;
    char firstHalf[ARG_SIZE];
    char secondHalf[ARG_SIZE];
    char request[REQ_SIZE];
    struct hostent *server;
    struct sockaddr_in serveraddr;
    int port = 80;
     
    int i;
    for (i = 0; i < strlen(arg); i++) {
        if (arg[i] == '/') {
                strncpy(firstHalf, arg, i);
                firstHalf[i] = '\0';
                break;
        }     
    }
     
    for (; i < strlen(arg); i++) {
        strcat(secondHalf, &arg[i]);
        break;
    }
     
    #ifdef VERBOSE
    printf("\nFirst Half: %s", firstHalf);
    printf("\nSecond Half: %s", secondHalf);
    #endif

    // Openning socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket < 0) {
        fprintf(stderr, "\nError opening socket");
    }
    else {
        #ifdef VERBOSE
        fprintf(stdout, "\nSuccessfully opened socket");
        #endif
    }
     
    // Creating host
    server = gethostbyname(firstHalf);
    if (server == NULL) {
        fprintf(stderr, "gethostbyname() failed\n");
    }
    else {
        #ifdef VERBOSE
        printf("\n%s = ", server->h_name);
        #endif
        unsigned int j = 0;
        while (server -> h_addr_list[j] != NULL) {
            printf("%s", inet_ntoa(*(struct in_addr*)(server -> h_addr_list[j])));
            j++;
        }
    }
     
    printf("\n");
 
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
 
    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port);
     
    if (connect(tcpSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        fprintf(stderr, "\nError Connecting");
    }
    else {
        #ifdef VERBOSE
        fprintf(stdout, "\nSuccessfully Connected");
        #endif
    }
   
    // Send some data
    bzero(request, REQ_SIZE);    
    sprintf(request, "Get %s HTTP/1.1\r\nHost: %s\r\n\r\n", secondHalf, firstHalf);
    #ifdef VERBOSE
    printf("\n%s\n", request);
    #endif

    if( send(tcpSocket , request , strlen(request) , 0) < 0) {
        fprintf(stderr, "\nSending request failed");
        exit(EXIT_FAILURE);
    }
    else {
        #ifdef VERBOSE
        fprintf(stdout, "\nRequest sent");
        #endif
    }
     
    // Now receive full data
    int total_recv = recv_timeout(tcpSocket, TIMEOUT, values);
     
    #ifdef VERBOSE
    printf("\n\nDone. Received a total of %d bytes\n\n" , total_recv);
    #endif
}
 
/*
    Receive data in multiple chunks by checking a non-blocking socket
    Timeout in seconds
*/
int recv_timeout(int s, int timeout, uint8_t *values) {

    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[CHUNK_SIZE];
    double timediff;
    int count = 0;

    //make socket non blocking
    fcntl(s, F_SETFL, O_NONBLOCK);
     
    //beginning time
    gettimeofday(&begin , NULL);
     
    while(1) {

        gettimeofday(&now , NULL);
         
        // Time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        // If you got some data, then break after timeout
        if( total_size > 0 && timediff > timeout ) {
            break;
        }
        // If you got no data at all, wait a little longer, twice the timeout
        else if( timediff > timeout*2) {
            break;
        }
         
        memset(chunk ,0 , CHUNK_SIZE);  // Clear the variable
        if((size_recv =  recv(s , chunk , CHUNK_SIZE , 0) ) < 0) {
            // If nothing was received, wait 0.1sec before trying again.
            usleep(100000);
        }
        else {
            parseAndStoreChunck(total_size, size_recv, values, chunk);
            total_size += size_recv;
            #ifdef VERBOSE
            printf("\nChunck no %d |->\n%s" , ++count, chunk);
            #endif

            // Reset beginning time
            gettimeofday(&begin , NULL);
        }
    }
     
    return total_size;
}

void parseAndStoreChunck(int from, int to, uint8_t *dest, char *src) {

}

