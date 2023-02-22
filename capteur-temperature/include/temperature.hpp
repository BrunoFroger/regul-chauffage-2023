//----------------------------------------------
//
//      temperature.hpp
//
//----------------------------------------------
#include <Arduino.h>

#ifndef __TEMPERATURE
#define __TEMPERATURE

    extern void initTemperature(void);
    extern void refreshTemperature(void);
    extern int getTemperature(void);
    extern void handleGetTemeratureInterieure(void);

#endif