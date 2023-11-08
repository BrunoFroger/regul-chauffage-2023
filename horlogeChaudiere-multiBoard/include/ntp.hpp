//----------------------------------------------
//
//      ntp.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __NTP__
#define __NTP__

    extern void initNtp(void);
    extern void refreshNtp(void);
    extern char *getFormatedTime(void);
    extern char *getFormatedTimeFull(void);
    extern int getDayOfWeek(void);
    extern int getHour(void);
    extern int getMinute(void);
    extern int getJour(void);
    extern int getMois(void);
    extern char *getDayString(int idx);
    extern bool getHeureEteHivers(void);
    extern char *getFormatedDate(void);
    extern char *getFullDate(void);
    extern void switchEteHiver(void);
#endif