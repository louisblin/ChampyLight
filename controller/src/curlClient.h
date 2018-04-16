/**
 * @file curlClient.h
 * @author Louis Blin
 * @date June 2015
 *
 * @brief Header for curlClient.c.
 */

#ifndef CURLCLIENT_H
#define CURLCLIENT_H

#include <stdint.h>

// Receiving function
bool getWebValues(uint8_t *values);
bool fetchValues();
bool parseAndStoreFile(uint8_t *values);

#endif
