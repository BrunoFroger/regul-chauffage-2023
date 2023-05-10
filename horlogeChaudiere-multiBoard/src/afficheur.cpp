//----------------------------------------------
//
//      afficheur.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <U8g2lib.h>

#include "wifiTools.hpp"
#include "ntp.hpp"
#include "calendrier.hpp"
#include "temperatures.hpp"
#include "pilotageChaudiere.hpp"

#define NB_ECRANS    3

int cptValeurs=0;
int delayRefreshAfficheur = 3000;
int lastRefreshAfficheur = 0;
bool afficheurOnOff = true;
//int delayVeilleAfficheur = 100;
//int lastVeilleAfficheur = 0;

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int hauteurligne;
int ligneCourante = 0;
int nbLignesDisponibles;
int ecranCourant=0;
char bufferLignesEcran[NB_ECRANS][10][20];

//----------------------------------------------
//
//      getAfficheurOnOff
//
//----------------------------------------------
void switchAfficheurOnOff(){
    afficheurOnOff = !afficheurOnOff;
    if (afficheurOnOff){
        Serial.println("afficheur on");
    } else {
        Serial.println("afficheur off");
    }
}

//----------------------------------------------
//
//      getAfficheurOnOff
//
//----------------------------------------------
bool getAfficheurOnOff(){
    return afficheurOnOff;
}

//----------------------------------------------
//
//      setAfficheurOnOff
//
//----------------------------------------------
void setAfficheurOnOff(bool state){
    afficheurOnOff = state;
}

//----------------------------------------------
//
//      ecritLigne
//
//----------------------------------------------
void ecritLigne(int ecran, int numLigne, char *texte){
    //char tmp[200]; sprintf(tmp,"ligne %d => %s", numLigne, texte); Serial.println(tmp);
    int x=(numLigne + 1) * hauteurligne;
    ligneCourante=numLigne;
    strcpy(bufferLignesEcran[ecran][ligneCourante], texte);
    u8g2.drawStr(0,x,texte);
    u8g2.sendBuffer();
    //delay(1000);
}

//----------------------------------------------
//
//      ecritLigneSuivante
//
//----------------------------------------------
void ecritLigneSuivante(int ecran, char *texte){
    //Serial.println("-------- debut ecritLigneSuivante -----------");
    if (ligneCourante >= nbLignesDisponibles - 1){
        // scroll vertical
        u8g2.clearDisplay();
        for (int i = 0 ; i < nbLignesDisponibles - 1 ; i++){
            ecritLigne(ecran, i, bufferLignesEcran[ecran][i+1]);
        }
        ecritLigne(ecran, nbLignesDisponibles - 1, texte);
    } else {
        ecritLigne(ecran, ++ligneCourante, texte);
    }
}

//----------------------------------------------
//
//      initAfficheur.cpp
//
//----------------------------------------------
void initAfficheur(void){
    Serial.println("======================");
    Serial.println("     Init Afficheur   ");
    Serial.println("----------------------");
    Serial.println("initialisation LCD .....");
    u8g2.begin();
    u8g2.clearBuffer();

    #define font_4x6_tr    0   // 6 lignes de 16 caracteres (peu lisible)
    #define font_5x7_tr    1   // 6 lignes de 13 caracteres
    #define font_5x8_tr    2   // 5 lignes de 13 caracteres
    #define font_6x10_tr   3   // 4 lignes de 11 caracteres
    int font = font_5x7_tr;
    Serial.print("Font selectionnée : ");
    switch(font){
        case font_4x6_tr:
            Serial.println("u8g2_font_4x6_tr");
            hauteurligne=7;
            u8g2.setFont(u8g2_font_4x6_tr);
            nbLignesDisponibles = 6;
            break;
        case font_5x7_tr:
            Serial.println("u8g2_font_5x7_tr");
            hauteurligne=8;
            u8g2.setFont(u8g2_font_5x7_tr);
            nbLignesDisponibles = 6;
            break;
        case font_5x8_tr:
            Serial.println("u8g2_font_5x8_tr");
            hauteurligne=9;
            u8g2.setFont(u8g2_font_5x8_tr);
            nbLignesDisponibles = 5;
            break;
        case font_6x10_tr:
            Serial.println("u8g2_font_6x10_tr");
            hauteurligne=11;
            u8g2.setFont(u8g2_font_6x10_tr);
            nbLignesDisponibles = 4;
            break;
        default:
            Serial.println("inconnue");
    }
    int x=hauteurligne, y=1;
    u8g2.drawStr(0,x*y++," chaudiere");
    u8g2.sendBuffer();
    delay(100);
    Serial.println("init Afficheur => fin ");
    Serial.println("======================");
}

//----------------------------------------------
//
//      clearBufferEcran
//
//----------------------------------------------
void clearBufferEcran(int ecran){
    for (int i = 0 ; i < nbLignesDisponibles ; i++){
        strcpy(bufferLignesEcran[ecran][i], (char*)"");
    }
}

//----------------------------------------------
//
//      refreshAfficheur
//
//----------------------------------------------
void refreshAfficheur(void){
    if (afficheurOnOff){  
        int tmp;
        tmp = millis() - lastRefreshAfficheur;
        //Serial.println(tmp);
        if (tmp > delayRefreshAfficheur){
            //Serial.println("refresh afficheur");
            lastRefreshAfficheur = millis();
            int idx=0;
            clearBufferEcran(ecranCourant);
            sprintf(bufferLignesEcran[ecranCourant][idx++], "%s  (%d/%d)",getFormatedTime(), ecranCourant + 1, NB_ECRANS);
            switch(ecranCourant){
                case 0 :
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "Cons   : %.1f", (double)(getConsigne() / 10));
                    if (getChauffageOnOff()){
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Chauff : ON");
                    } else {
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Chauff : OFF");
                    }
                    if (getChauffageStatus()){
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Prog   : ON");
                    } else {
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Prog   : OFF");
                    }
                    if (getCirculateurOnOff()){
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Pompe  : ON");
                    } else {
                        sprintf(bufferLignesEcran[ecranCourant][idx++], "Pompe  : OFF");
                    }
                    break;
                case 1 :
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "Cons   : %.1f", (double)getConsigne() / 10);
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "T Int  : %.1f", (double)getTemperatureInterieure()/10);
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "T Ext  : %.1f", (double)getTemperatureExterieure()/10);
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "Pompe  : %d", getCommandeVanneChauffage());
                    break;
                case 2 :
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "IP locale");
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "%s", getIpAddress());
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "IP T int");
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "%s", getIPCapteurTemperatureInterieure());
                    sprintf(bufferLignesEcran[ecranCourant][idx++], "rssi : %ddb", getRSSI()); 
                    break;
            }
            u8g2.clearDisplay();
            for (int i = 0 ; i < nbLignesDisponibles ; i++){
                ecritLigne(ecranCourant, i, bufferLignesEcran[ecranCourant][i]);
            }
            ecranCourant++;
            if (ecranCourant >= NB_ECRANS) ecranCourant = 0;
        }
    } else {
        u8g2.clearDisplay();
        //lcd.setBacklight(LOW); 
    }
}