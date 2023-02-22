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
    extern char *getDayString(int idx);
#endif