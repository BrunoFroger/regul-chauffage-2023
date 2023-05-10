//----------------------------------------------
//
//      temperature.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "wifiTools.hpp"
#include "roueCodeuse.hpp"

#define PIN_TEMPERATURE_INT A0
#define PIN_TEMPERATURE_EXT 16

int temperatureInterieure;
int temperatureExterieure;
int consigne;

//----------------------------------------------
//
//      getTemperatureInterieure
//
//----------------------------------------------
int getTemperatureInterieure(void){
    return temperatureInterieure;
}

//----------------------------------------------
//
//      getTemperatureExterieure
//
//----------------------------------------------
int getTemperatureExterieure(void){
    return temperatureExterieure;
}

//----------------------------------------------
//
//      getConsigne
//
//----------------------------------------------
int getConsigne(void){
    return consigne;
}

//----------------------------------------------
//
//      setConsigne
//
//----------------------------------------------
void setConsigne(int valeur){
    consigne = valeur;
}

//----------------------------------------------
//
//      handleGetTemeratureInterieure
//
//----------------------------------------------
void handleGetTemeratureInterieure(void)
{
    Serial.print("requete de temperature interieure recue => ");
    Serial.println(temperatureInterieure);
    String page = "";
    page += "temperatureInt=";
    page += temperatureInterieure;
    //page += "\n";
    //page += "\n";
    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/plain", page);
}

//----------------------------------------------
//
//      handleGetTemeratureExterieure
//
//----------------------------------------------
void handleGetTemeratureExterieure(void)
{
    Serial.print("requete de temperature exterieure recue => ");
    Serial.println(temperatureExterieure);
    String page = "";
    page += "temperatureExt=";
    page += temperatureExterieure;
    //page += "\n";
    //page += "\n";
    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/plain", page);
}

//----------------------------------------------
//
//      initTemperature
//
//----------------------------------------------
void initTemperatures(void){
    pinMode(PIN_TEMPERATURE_INT,INPUT_PULLUP);
    pinMode(PIN_TEMPERATURE_EXT,INPUT_PULLUP);
    consigne = 190;
}

//----------------------------------------------
//
//      refreshTemperature
//
//----------------------------------------------
void refreshTemperature(void){
    // temperature en 1/10 de degres
    temperatureInterieure = map(analogRead(PIN_TEMPERATURE_INT), 0, 1024, -100, 500);
    temperatureExterieure = map(analogRead(PIN_TEMPERATURE_EXT), 0, 1024, -100, 500);
    consigne += getVariationRoueCodeuse();
}