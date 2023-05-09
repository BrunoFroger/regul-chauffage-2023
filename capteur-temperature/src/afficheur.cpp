//----------------------------------------------
//
//      afficheur.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <U8g2lib.h>

#include "wifiTools.hpp"
#include "ntp.hpp"
#include "temperature.hpp"

//char bufferLigne1[20];
//char bufferLigne2[20];
int cptValeurs=0;
int delayRefreshAfficheur = 1000;
int lastRefreshAfficheur = 0;
bool afficheurOnOff = true;
//int delayVeilleAfficheur = 100;
//int lastVeilleAfficheur = 0;

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int hauteurligne;
int ligneCourante = 0;
int nbLignesDisponibles;
char bufferLignes[10][20];

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
void ecritLigne(int numLigne, char *texte){
    //char tmp[200]; sprintf(tmp,"ligne %d => %s", numLigne, texte); Serial.println(tmp);
    int x=(numLigne + 1) * hauteurligne;
    ligneCourante=numLigne;
    strcpy(bufferLignes[ligneCourante], texte);
    u8g2.drawStr(0,x,texte);
    u8g2.sendBuffer();
    //delay(1000);
}

//----------------------------------------------
//
//      ecritLigneSuivante
//
//----------------------------------------------
void ecritLigneSuivante(char *texte){
    //Serial.println("-------- debut ecritLigneSuivante -----------");
    if (ligneCourante >= nbLignesDisponibles - 1){
        // scroll vertical
        u8g2.clearDisplay();
        for (int i = 0 ; i < nbLignesDisponibles - 1 ; i++){
            ecritLigne(i, bufferLignes[i+1]);
        }
        ecritLigne(nbLignesDisponibles - 1, texte);
    } else {
        ecritLigne(++ligneCourante, texte);
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
    u8g2.drawStr(0,x*y++," Consigne");
    u8g2.sendBuffer();
    delay(100);
    Serial.println("init Afficheur => fin ");
    Serial.println("======================");
}

//----------------------------------------------
//
//      refreshAfficheur.cpp
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
            sprintf(bufferLignes[idx++], "   %s",getFormatedTime());
            sprintf(bufferLignes[idx++], "%s", getIpAddress());
            sprintf(bufferLignes[idx++], "Cons  : %.1f", (double) (getConsigne() / 10));
            sprintf(bufferLignes[idx++], "T int : %.1f", (double) (getTemperatureInterieure() / 10));
            sprintf(bufferLignes[idx++], "T ext : %.1f", (double) (getTemperatureExterieure() / 10));
            sprintf(bufferLignes[idx++], "");
            u8g2.clearDisplay();
            for (int i = 0 ; i < nbLignesDisponibles ; i++){
                ecritLigne(i, bufferLignes[i]);
            }
        }
    } else {
        u8g2.clearDisplay();
        //lcd.setBacklight(LOW); 
    }
}