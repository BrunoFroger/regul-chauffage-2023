//----------------------------------------------
//
//      afficheur.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "wifiTools.hpp"
#include "temperature.hpp"

LiquidCrystal_I2C lcd(0x23,16,2);
char bufferLigne1[20];
char bufferLigne2[20];
int cptValeurs=0;
int delayRefreshAfficheur = 500;
int lastRefreshAfficheur = 0;

//----------------------------------------------
//
//      initAfficheur.cpp
//
//----------------------------------------------
void initAfficheur(void){
    Serial.println("======================");
    Serial.println("     Init Afficheur   ");
    Serial.println("----------------------");
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
    int tmp;
    tmp = millis() - lastRefreshAfficheur;
    //Serial.println(tmp);
    if (tmp > delayRefreshAfficheur){
        lastRefreshAfficheur = millis();
        lcd.clear();
        lcd.setCursor(0,0);
        sprintf(bufferLigne1, "temp : %.1f", (float)getTemperature()/10.0);
        //Serial.println(bufferLigne1);
        lcd.print(bufferLigne1);
        lcd.setCursor(0,1);
        sprintf(bufferLigne2,"%s", getIpAdresse());
        lcd.print(bufferLigne2);
        //Serial.println(bufferLigne2);
    }
}