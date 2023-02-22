//----------------------------------------------
//
//      afficheur.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "wifiTools.hpp"
#include "ntp.hpp"
#include "calendrier.hpp"
#include "pilotageChaudiere.hpp"

LiquidCrystal_I2C lcd(i2cDisplayPort,16,2);
char bufferLigne1[20];
char bufferLigne2[20];
int cptValeurs=0;
int delayRefreshAfficheur = 100;
int lastRefreshAfficheur = 0;
bool afficheurOnOff = true;
//int delayVeilleAfficheur = 100;
//int lastVeilleAfficheur = 0;



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
//      initAfficheur.cpp
//
//----------------------------------------------
void initAfficheur(void){
    Serial.println("======================");
    Serial.println("     Init Afficheur   ");
    Serial.println("----------------------");
    /*byte error, address;
    // Scan I2C bus to find LCD address
    for (address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
        break;
        }
    }
    if (error != 0){
        Serial.print(F("I2C screen found "));
        Serial.println(address);
    } else {
        Serial.println(F("erreur : I2C screen not found "));
    }
    lcd=LiquidCrystal_I2C(address, 16, 2);*/
    Serial.println("initialisation LCD");
    lcd.init(); // initialize the lcd
    Serial.println("lcd initialise");    
    lcd.setBacklight(HIGH); 
    Serial.println("initialisation => OK");
    Serial.println("======================");
}

//----------------------------------------------
//
//      refreshAfficheur.cpp
//
//----------------------------------------------
void refreshAfficheur(void){
    if (afficheurOnOff){  
        lcd.setBacklight(HIGH); 
        int tmp;
        tmp = millis() - lastRefreshAfficheur;
        //Serial.println(tmp);
        if (tmp > delayRefreshAfficheur){
            lastRefreshAfficheur = millis();
            //temps.update();
            //sprintf(bufferLigne1, "temps = %s\n", temps.getFormattedTime());
            //Serial.print("refreshAfficheur : "); Serial.println(getFrmatedTime());
            lcd.clear();
            lcd.setCursor(0,0);
            if (getCirculateurOnOff()){
                sprintf(bufferLigne1, "%s ON (%d%%)", getFormatedTime(), getCommandeVanneChauffage());
            } else {
                sprintf(bufferLigne1, "%s OFF (%d%%)", getFormatedTime(), getCommandeVanneChauffage());
            }
            lcd.print(bufferLigne1);
            lcd.setCursor(0,1);
            sprintf(bufferLigne2,"%s", getIpAddress());
            lcd.print(bufferLigne2);
        }
    } else {
        lcd.clear();
        lcd.setBacklight(LOW); 
    }
}