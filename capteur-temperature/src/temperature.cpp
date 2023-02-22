//----------------------------------------------
//
//      temperature.cpp
//
//----------------------------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "wifiTools.hpp"

#define PIN_TEMPERATURE A0
int temperature;

//----------------------------------------------
//
//      getTemperature
//
//----------------------------------------------
int getTemperature(void){
    return temperature;
}

//----------------------------------------------
//
//      initTemperature
//
//----------------------------------------------
void initTemperature(void){
    pinMode(PIN_TEMPERATURE,INPUT_PULLUP);
}

//----------------------------------------------
//
//      handleGetTemeratureInterieure
//
//----------------------------------------------
void handleGetTemeratureInterieure(void)
{
    Serial.print("requete de temperature interieure recue => ");
    Serial.println(temperature);
    String page = "";
    page += "temperatureInt=";
    page += temperature;
    //page += "\n";
    //page += "\n";
    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/plain", page);
}

//----------------------------------------------
//
//      refreshTemperature
//
//----------------------------------------------
void refreshTemperature(void){
    // temperature en 1/10 de degres
    temperature = map(analogRead(PIN_TEMPERATURE), 0, 1024, -100, 500);
}