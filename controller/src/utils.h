/**
 * @file utils.h
 * @author Louis Blin
 * @date June 2015
 *
 * @brief Header for utils.c.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>

void printSHM(uint8_t values[]);
uint8_t* arrcp(uint8_t const *src, size_t length);
void sleep_ms (int millis);
void switch_to_idle(uint8_t *values);

#endif
