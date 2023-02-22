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
    extern bool getChauffageOnOff();
    extern int getCommandeVanneChauffage();
    extern int getPinRelai(void);

#endif