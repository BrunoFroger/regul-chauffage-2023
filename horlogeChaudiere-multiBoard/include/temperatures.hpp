//----------------------------------------------
//
//      temperatures.hpp
//
//----------------------------------------------

#ifndef __TEMPERATURE__
#define __TEMPERATURE__

    #define TEMPERATURE_INTERIEURE_MIN     0
    #define TEMPERATURE_INTERIEURE_MAX     30

    // les temperatures sont exprimées en 1/10 de degres
    extern void initTemperatures(void);
    extern void refreshTemperatures(void);
    extern void setConsigne(int consigne);
    extern void handleReceiveTemeratureInterieure(void);
    extern int getTemperatureInterieure(void);
    extern int getTemperatureExterieure(void);
    extern int getConsigne(void);
    extern void handleTemperatures(void);
    extern void handleUpdateConsigne(void);
    extern bool temperatureAtteinte(void);
    extern char *getTemperatureSring(int temperature);
    extern int updateTemperature(int *ptrTemp, char *newValue);
    extern char *getIPCapteurTemperature(void);
    extern void setIPCapteurTemperature(char *);

#endif