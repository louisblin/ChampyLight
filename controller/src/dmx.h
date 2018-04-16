/**
 * @file dmx.h
 * @author Louis Blin
 * NB: based on Velleman K8062 DMX controller library for VM116/K8062 used in
 * the DMXWheel project - www.dmxwheel.com
 * @date June 2015
 *
 * @brief Header file for dmx.c.
 */

#ifndef DMX_H
#define DMX_H

#include <stdint.h>

#include <constants.h>

int   dmxOpen              (uint8_t **shmValues);
void  dmxClose             ();
void  dmxSetValues         (unsigned int fromCh, int count, uint8_t values[]);
bool  isRunning            ();

void  printSHMState        ();
void  switch_to_idle_state ();

#endif
