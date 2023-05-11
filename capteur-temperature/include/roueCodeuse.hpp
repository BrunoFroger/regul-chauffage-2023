//----------------------------------------------
//
//      roueCodeuse.hpp
//
//----------------------------------------------

#include <Arduino.h>

#ifndef __ROUECODEUSE__
#define __ROUECODEUSE__

    extern void initRoueCodeuse(void);
    extern void refreshRoueCodeuse(void);
    extern bool getBoutonRoueCodeuse(void);
    extern int getValeurRoueCodeuse(void);
    extern int getVariationRoueCodeuse(void);

#endif