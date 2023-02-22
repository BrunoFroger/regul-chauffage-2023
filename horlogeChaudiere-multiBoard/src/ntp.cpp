//----------------------------------------------
//
//      ntp.cpp
//
//----------------------------------------------

#include <Arduino.h>

#ifdef lolin_s2_mini
    #pragma message("lolin_s2_mini")
    #include <ESP8266WiFi.h>
#elif adafruit_feather_m0 
    #pragma message("adafruit_feather_m0")
    #include <WiFi.h>
#elif wemos_d1_mini32 
    #pragma message("wemos_d1_mini32")
    #include <WiFi.h>
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
/*
* Choix du serveur NTP pour récupérer l'heure, 3600 =1h est le fuseau horaire et 60000=60s est le * taux de rafraichissement
*/
NTPClient temps(ntpUDP, "fr.pool.ntp.org", 3600, 60000);
char formatedTimeFull[30];
char formatedTime[30];
int dayOfWeek, heure, minute;
char dayString[15];

//----------------------------------------------
//
//      getDayOfWeek
//
//----------------------------------------------
char *getDayString(int idx){
    switch(idx){
        case 0 : return ((char *)"dimanche"); break;
        case 1 : return ((char *)"lundi"); break;
        case 2 : return ((char *)"mardi"); break;
        case 3 : return ((char *)"mercredi"); break;
        case 4 : return ((char *)"jeudi"); break;
        case 5 : return ((char *)"vendredi"); break;
        case 6 : return ((char *)"samedi"); break;
    }
    return (char *)"inconnu";
}

//----------------------------------------------
//
//      refreshNtp
//
//----------------------------------------------
void refreshNtp(void){
    if ((heure == 0) && (minute == 0)) temps.update();
    if (temps.getDay() != dayOfWeek){
        dayOfWeek = temps.getDay();
        strcpy(dayString, getDayString(dayOfWeek));
    }
    heure = temps.getHours();
    minute = temps.getMinutes();
    sprintf(formatedTimeFull, temps.getFormattedTime().c_str());
    sprintf(formatedTime, "%02d:%02d", heure, minute);
    //Serial.print("refresh Ntp : "); Serial.println(formatedTime);
}

//----------------------------------------------
//
//      initNtp
//
//----------------------------------------------
void initNtp(){
    Serial.println("======================");
    Serial.println("     Init NTP         ");
    Serial.println("----------------------");
    temps.begin(); 
    refreshNtp();
    Serial.println("initialisation NTP => OK");
    delay(10);
    Serial.println(formatedTime);
}

//----------------------------------------------
//
//      getFrmatedTime
//
//----------------------------------------------
char *getFormatedTimeFull(void){
    //sprintf(formatedTime,"%s", temps.getFormattedTime().c_str() );
    return formatedTimeFull;
}

//----------------------------------------------
//
//      getFrmatedTime
//
//----------------------------------------------
char *getFormatedTime(void){
    //sprintf(formatedTime,"%s", temps.getFormattedTime().c_str() );
    return formatedTime;
}

//----------------------------------------------
//
//      getDayOfWeek
//
//----------------------------------------------
int getDayOfWeek(void){
    //refreshNtp();
    return temps.getDay();
}

//----------------------------------------------
//
//      getHour
//
//----------------------------------------------
int getHour(void){
    //refreshNtp();
    return temps.getHours();
}

//----------------------------------------------
//
//      getHour
//
//----------------------------------------------
int getMinute(void){
    //refreshNtp();
    return temps.getMinutes();
}