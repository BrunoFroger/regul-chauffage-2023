//----------------------------------------------
//
//      main.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "wifiTools.hpp"
#include "afficheur.hpp"
#include "temperature.hpp"

char wifiSsid[50];
char wifiPwd[50];

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

    strcpy(wifiSsid, localWifiSsid);
    strcpy(wifiPwd, localWifiPwd);
    initWifi((char *)wifiSsid, (char *)wifiPwd);
    initTemperature();
    initAfficheur();
}

//=================================================
//
//      loop
//
//=================================================
void loop() {
  refreshWifi();
  refreshAfficheur();
  refreshTemperature();
  delay(LOOP_DELAY);
}