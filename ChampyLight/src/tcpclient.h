#ifndef TCPCLIENT_H
#define TCPCLIENT_H

// Receiving function
void getWebValues(uint8_t *values);
int recv_timeout(int s, int timeout, uint8_t *values);
void parseAndStoreChunck(int from, int to, uint8_t *dest, char *src);

#endif
