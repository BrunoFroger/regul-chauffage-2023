//----------------------------------------------
//
//      pilotageChaudiere.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __PILOTAGE_CHAUDIERE__
#define __PILOTAGE_CHAUDIERE__

    extern void initChaudiere(void);
    extern void refreshChaudiere(void);
    extern bool getCirculateurOnOff(void);
    extern void handleCommandeChauffage();
    extern void handleSwitchChauffageOnOff(void);
    extern void handleSwitchModeRegulation(void);
    extern bool getChauffageOnOff();
    extern void setChauffageOnOff(bool onOff);
    extern int getCommandeVanneChauffage();
    extern int getPinRelai(void);
    extern void setPinRelai(int pin);
    extern void setRegulation(bool onOff);
    extern bool getRegulationMode(void);

#endif