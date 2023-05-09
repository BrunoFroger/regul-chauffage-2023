//----------------------------------------------
//
//      main.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "wifiTools.hpp"
#include "afficheur.hpp"
#include "temperature.hpp"
#include "ntp.hpp"

#define LOOP_DELAY 250

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

    initAfficheur();
    ecritLigneSuivante((char *)"Afficheur OK");
    initWifi();
    ecritLigneSuivante((char *)"wifi OK");
    initNtp();
    ecritLigneSuivante((char *)"ntp OK");
    initTemperatures();
    ecritLigneSuivante((char *)"temp OK");
}

//=================================================
//
//      loop
//
//=================================================
void loop() {
  refreshAfficheur();
  refreshTemperature();
  if (isWifiConnected() || isApMode()){
    handleWebRequete();
  }
  delay(LOOP_DELAY);
}