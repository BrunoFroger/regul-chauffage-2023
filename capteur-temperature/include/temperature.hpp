//----------------------------------------------
//
//      temperature.hpp
//
//----------------------------------------------
#include <Arduino.h>

#ifndef __TEMPERATURE
#define __TEMPERATURE

    extern void initTemperatures(void);
    extern void refreshTemperature(void);
    extern int getTemperatureInterieure(void);
    extern int getTemperatureExterieure(void);
    extern int getConsigne(void);
    extern void setConsigne(int valeur);
    extern void handleGetTemeratureInterieure(void);

#endif