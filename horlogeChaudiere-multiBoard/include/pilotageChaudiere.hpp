//----------------------------------------------
//
//      pilotageChaudiere.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __PILOTAGE_CHAUDIERE__
#define __PILOTAGE_CHAUDIERE__

    #define MODE_CHAUFFAGE_OFF      0
    #define MODE_CHAUFFAGE_PROG     1
    #define MODE_CHAUFFAGE_FORCE    2

    extern void initChaudiere(void);
    extern void refreshChaudiere(void);
    extern bool getCirculateurOnOff(void);
    extern void handleCommandeChauffage();
    extern void handleSwitchChauffageOnOff(void);
    extern void handleSwitchModeRegulation(void);
    extern int getChauffageMode();
    extern char *getChauffageModeString();
    extern void setChauffageMode(int mode);
    extern int getCommandeVanneChauffage();
    extern int getPinRelai(void);
    extern void setPinRelai(int pin);
    extern void setRegulation(bool onOff);
    extern bool getRegulationMode(void);

#endif