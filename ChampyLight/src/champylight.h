#ifndef CHAMPYLIGHT_H
#define CHAMPYLIGHT_H

int initConnection();
void closeConnection();
int initDMX();
void setDMXColor(unsigned int from, int length, double values[]);
void exitDMX();

#endif
