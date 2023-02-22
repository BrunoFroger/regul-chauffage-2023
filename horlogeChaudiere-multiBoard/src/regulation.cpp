//----------------------------------------------
//
//      regulation.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include "temperatures.hpp"

int delayRefreshRegulation = 5000;
int lastRefreshRegulation= 0;

//----------------------------------------------
//
//      calculRegulation
//
//----------------------------------------------
int calculDeltaRegulation(int vanne, int consigne, int tempInt){
    char ligne[200]="";
// TODO version simpliste, a ameliorer
    int tmp;
    tmp = millis() - lastRefreshRegulation;
    //Serial.println(tmp);
    if (tmp > delayRefreshRegulation){
        lastRefreshRegulation = millis();
        Serial.println("-------------------------------------");
        
        
        int delta = abs(consigne - tempInt);
        int increment = 0;
        char sens;
        if (delta < 5) {
            increment = 0;
            sens ='=';
        } else {
            increment = delta / 20;
            if (consigne < tempInt){
                sens = '-';
            } else {
                sens = '+';
            }
        }
        vanne += increment;
        if (vanne < 0) vanne = 0;
        if (vanne > 100) vanne = 100;
        sprintf(ligne, "calcul delta vanne chauffage consigne=%d, capteur=%d,  vanne=%d increment=%d => %c ", consigne, tempInt, vanne, increment, sens);
        Serial.println(ligne);
    }
    return vanne;
}