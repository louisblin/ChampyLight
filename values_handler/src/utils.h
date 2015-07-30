#ifndef UTILS_H
#define UTILS_H

void printSHM(uint8_t values[]);
uint8_t* arrcp(uint8_t const *src, size_t length);
void sleep_ms (int millis);
void switch_to_idle_state(uint8_t *values);

#endif
