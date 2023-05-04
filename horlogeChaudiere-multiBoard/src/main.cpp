//----------------------------------------------
//
//      main.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "afficheur.hpp"
#include "wifiTools.hpp"
#include "ntp.hpp"
#include "calendrier.hpp"
#include "pilotageChaudiere.hpp"
#include "temperatures.hpp"
#include "sdCard.hpp"
#include "scanI2C.hpp"



#ifdef lolin_s2_mini
    #pragma message("lolin_s2_mini")
    #define TITRE "WeMos ESP8266"
#elif wemos_d1_mini32 
    #pragma message("wemos_d1_mini32")
    #define TITRE "Wemos D1 mini ESP 32"
#elif adafruit_feather_m0 
    #pragma message("adafruit_feather_m0")
    #define TITRE "Feather ESP32"
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif

#define LOOP_DELAY 250


//=================================================
//
//      stop
//
//=================================================
void stop(void){
    Serial.print("une des initialisations est KO ...... ");
    Serial.println ("STOP");
    while(1) {
        delay(10000);
        Serial.print(".");
    } 
}

//=================================================
//
//      setup
//
//=================================================
void setup() {        
    // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
    char titre[50];
    sprintf(titre, "+  %25s    +", TITRE);
    delay(1000);
    Serial.println("Serial initialized");
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println(titre);
    Serial.println("+        Horloge chaudiere      +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
    Serial.println("+        debut setup            +");
    Serial.println("+                               +");
    initAfficheur();
    ecritLigne(0, (char *)"afficheur OK");
    if (1) {
        scanI2C();
        ecritLigneSuivante((char *)"scanI2C OK");
    }
    if (!sdcardInit()){
        ecritLigneSuivante((char *)"sdcard NOK");
        stop();
    }
    ecritLigneSuivante((char *)"sdcard OK");
    if (!initWifi()){
        stop();
    }
    if (isWifiConnected()){
        ecritLigneSuivante((char *)"NTP OK");
        initNtp();
    }
    initCalendrier();
    ecritLigneSuivante((char *)"calend OK");
    initTemperatures();
    ecritLigneSuivante((char *)"temp OK");
    initChaudiere();
    Serial.print("handleWebRequete : ");
    if (isWifiConnected() || isApMode()){
        Serial.println("OK");
    } else {
        Serial.println("NOK");
    }
    Serial.println("+        fin setup              +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");
}


//=================================================
//
//      loop
//
//=================================================
void loop(){ 

    refreshAfficheur();
    if (isWifiConnected()) refreshNtp();
    refreshChaudiere();
    if (getRegulationMode()){
        refreshTemperatures();
    }
    if (isWifiConnected() || isApMode()){
        handleWebRequete();
    }
    delay(LOOP_DELAY);
}