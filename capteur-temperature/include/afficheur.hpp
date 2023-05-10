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
    extern void switchEcranAfficheur(void);
    extern void ecritLigneSuivante(int ecran, char *texte);
    extern void ecritLigne(int ecran, int numLigne, char *texte);

#endif