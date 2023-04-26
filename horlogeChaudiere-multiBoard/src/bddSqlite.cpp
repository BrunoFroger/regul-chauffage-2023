//----------------------------------------------
//
//      bddSqlite.cpp
//
//----------------------------------------------

#include <Arduino.h>
#include <string.h>

#include "sdCard.hpp"

char bddFilename[25];

//----------------------------------------------
//
//      openDatabase
//
//----------------------------------------------
void openDatabase(char *filename){
    if (sdCardOpenFile(filename) == NULL){
        Serial.print("ERREUR : impossible d'ouvrir ");
        Serial.println(filename);
    }
}

//----------------------------------------------
//
//      initBddSqlite
//
//----------------------------------------------
void initBddSqlite(void){
    Serial.println("init bdd sqlite .....");
    strcpy(bddFilename,"chaudiere/chaudiere.db");
    openDatabase(bddFilename);
    Serial.println("fin init bdd sqlite");
}