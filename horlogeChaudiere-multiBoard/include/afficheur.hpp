//----------------------------------------------
//
//      afficheur.hpp
//
//----------------------------------------------

#ifndef __AFFICHEUR__
#define __AFFICHEUR__

    extern void initAfficheur(void);
    extern void refreshAfficheur(void);
    extern void setAfficheurOnOff(bool state);
    extern bool getAfficheurOnOff();
    extern void switchAfficheurOnOff(void);
    extern void ecritLigneSuivante(char *texte);
    extern void ecritLigne(int numLigne, char *texte);

#endif