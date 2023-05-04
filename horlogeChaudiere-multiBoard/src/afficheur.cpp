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
#include "pilotageChaudiere.hpp"

char bufferLigne1[20];
char bufferLigne2[20];
int cptValeurs=0;
int delayRefreshAfficheur = 100;
int lastRefreshAfficheur = 0;
bool afficheurOnOff = true;
//int delayVeilleAfficheur = 100;
//int lastVeilleAfficheur = 0;

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
int hauteurligne;
int ligneCourante = 0;

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
    int x=numLigne * hauteurligne;
    u8g2.drawStr(0,x,texte);
    u8g2.sendBuffer();
    ligneCourante=numLigne;
}

//----------------------------------------------
//
//      ecritLigneSuivante
//
//----------------------------------------------
void ecritLigneSuivante(char *texte){
    ligneCourante++;
    ecritLigne(ligneCourante, texte);
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
            break;
        case font_5x7_tr:
            Serial.println("u8g2_font_5x7_tr");
            hauteurligne=8;
            u8g2.setFont(u8g2_font_5x7_tr);
            break;
        case font_5x8_tr:
            Serial.println("u8g2_font_5x8_tr");
            hauteurligne=9;
            u8g2.setFont(u8g2_font_5x8_tr);
            break;
        case font_6x10_tr:
            Serial.println("u8g2_font_6x10_tr");
            hauteurligne=11;
            u8g2.setFont(u8g2_font_6x10_tr);
            break;
        default:
            Serial.println("inconnue");
    }
    int x=hauteurligne, y=1;
    u8g2.drawStr(0,x*y++," chaudiere");
    u8g2.drawStr(0,x*y++,"OK");
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
    return;
    if (afficheurOnOff){  
        //lcd.setBacklight(HIGH); 
        int tmp;
        tmp = millis() - lastRefreshAfficheur;
        //Serial.println(tmp);
        if (tmp > delayRefreshAfficheur){
            lastRefreshAfficheur = millis();
            //temps.update();
            //sprintf(bufferLigne1, "temps = %s\n", temps.getFormattedTime());
            //Serial.print("refreshAfficheur : "); Serial.println(getFrmatedTime());
            u8g2.clearDisplay();
            u8g2.setCursor(0,0);
            if (getCirculateurOnOff()){
                sprintf(bufferLigne1, "%s ON (%d%%)", getFormatedTime(), getCommandeVanneChauffage());
            } else {
                sprintf(bufferLigne1, "%s OFF (%d%%)", getFormatedTime(), getCommandeVanneChauffage());
            }
            u8g2.print(bufferLigne1);
            u8g2.setCursor(0,1);
            sprintf(bufferLigne2,"%s", getIpAddress());
            u8g2.print(bufferLigne2);
            u8g2.display();
        }
    } else {
        u8g2.clearDisplay();
        //lcd.setBacklight(LOW); 
    }
}