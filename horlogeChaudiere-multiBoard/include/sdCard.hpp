//----------------------------------------------
//
//      sdCard.hpp
//
//----------------------------------------------

#include <Arduino.h>
#include <SD.h>

#define SSID_UNAVAILABLE    0
#define SSID_AVAILABLE      1
#define SSID_NOTCHECKED     2

#ifndef __SDCARD__
#define __SDCARD__

    extern bool sdcardInit(void);
    extern void handleConfig(void);
    extern void handleSauveConfig(void);
    extern void handleSelectEnvironnement(void);
    extern void sauvegardeFichier(String filename, String datas);
    extern String lireFichier(String filename);
    extern void handleListFichierConfiguration(void);
    extern File sdCardOpenFile(char *filename);
    extern bool getNewSsid(void);
    extern void setWifiMode(char *ssid, int available);
    extern void handleUpdateIpTempInt(void);
    
#endif