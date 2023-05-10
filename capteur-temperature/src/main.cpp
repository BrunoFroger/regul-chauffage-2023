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
#include "roueCodeuse.hpp"

#define LOOP_DELAY 10

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
    ecritLigneSuivante(0, (char *)"Afficheur OK");
    initWifi();
    ecritLigneSuivante(0, (char *)"wifi OK");
    initNtp();
    ecritLigneSuivante(0, (char *)"ntp OK");
    initTemperatures();
    ecritLigneSuivante(0, (char *)"temp OK");
    initRoueCodeuse();
    ecritLigneSuivante(0, (char *)"roue OK");
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
  refreshRoueCodeuse();
  delay(LOOP_DELAY);
}