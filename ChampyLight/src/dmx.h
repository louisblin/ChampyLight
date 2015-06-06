// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
// ==========================================================================

#ifndef DMX_H
#define DMX_H

int     dmxOpen           ();
void    dmxClose          ();
void    dmxSetValue       (uint8_t channel, uint8_t value);
uint8_t dmxGetValue       (uint8_t channel);
bool    isRunning         ();

#endif
