//----------------------------------------------
//
//      sdCard.hpp
//
//----------------------------------------------

#include <Arduino.h>
#include <SD.h>

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
    
#endif