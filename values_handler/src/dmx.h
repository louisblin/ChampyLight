// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
// ==========================================================================

#include "constants.h"

#ifndef DMX_H
#define DMX_H

int     dmxOpen           (uint8_t **shmValues);
void    dmxClose          ();
void    dmxSetValue       (uint8_t channel, uint8_t value);
void    dmxSetValues      (unsigned int fromCh, int count, uint8_t values[]);
bool    isRunning         ();

void    printSHMState     ();

#endif
