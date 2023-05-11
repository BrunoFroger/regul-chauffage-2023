//----------------------------------------------
//
//      roueCodeuse.cpp
//
//----------------------------------------------

#include <Arduino.h>
#include <RotaryEncoder.h>

#include "afficheur.hpp"

#define PIN_CLK     D5     // signal A
#define PIN_DT      D6     // signal B
#define PIN_SW      D7     // switch

RotaryEncoder roueCodeuse(D6, D5);   // DT, CLK
int position, lastPosition, variation;
bool bouton=false, boutonAppuye=false;

//----------------------------------------------
//
//      getBoutonRoueCodeuse
//
//----------------------------------------------
bool getBoutonRoueCodeuse(void){
    return bouton;
}

//----------------------------------------------
//
//      getVariationRoueCodeuse
//
//----------------------------------------------
int getVariationRoueCodeuse(void){
    variation = position - lastPosition;
    lastPosition = position;
    //Serial.print("Variation Roue codeuse : "); Serial.println(variation);
    return variation;
}

//----------------------------------------------
//
//      getValeurRoueCodeuse
//
//----------------------------------------------
int getValeurRoueCodeuse(void){
    return position;
}

//----------------------------------------------
//
//      initRoueCodeuse
//
//----------------------------------------------
void initRoueCodeuse(void){
    Serial.println("======================");
    Serial.println("  Init roue codeuse   ");
    Serial.println("----------------------");
    pinMode(PIN_CLK, INPUT);
    pinMode(PIN_DT, INPUT);
    pinMode(PIN_SW, INPUT_PULLUP);
    roueCodeuse.setPosition(0);
    lastPosition = roueCodeuse.getPosition();
    Serial.println("Fin init roue codeuse");
    Serial.println("----------------------");
}

//----------------------------------------------
//
//      refreshRoueCodeuse
//
//----------------------------------------------
void refreshRoueCodeuse(void){
    roueCodeuse.tick();
    position = roueCodeuse.getPosition();
    bouton = !digitalRead(PIN_SW);
    //if (bouton) Serial.println("bouton");
    if (bouton && !boutonAppuye){
        boutonAppuye = true;
        // bascule allumage ecran
        //Serial.println("switch ecran ON/OFF");
        switchEcranAfficheur();
    } 
    if (!bouton && boutonAppuye) boutonAppuye = false;
    //Serial.println(position);
}