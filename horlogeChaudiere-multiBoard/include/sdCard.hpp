//----------------------------------------------
//
//      sdCard.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __SDCARD__
#define __SDCARD__

    extern bool sdcardInit(void);
    extern void handleConfig(void);
    extern void handleSauveConfig(void);
    extern void sauvegardeFichier(String filename, const uint8_t *datas, int size);
    extern void lireFichier(String filename, void *datas);
    extern void handleListFichierConfiguration(void);
    
#endif