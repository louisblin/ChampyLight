#ifndef CURLCLIENT_H
#define CURLCLIENT_H

// Receiving function
bool getWebValues(uint8_t *values);
bool fetchValues();
bool parseAndStoreFile(uint8_t *values);

#endif
