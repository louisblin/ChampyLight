#ifndef CHAMPYLIGHT_H
#define CHAMPYLIGHT_H

int  initDMX();
void setDMXColor(unsigned int from, int length, uint8_t values[]);
void exitDMX();

#endif
