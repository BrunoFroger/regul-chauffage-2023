//----------------------------------------------
//
//      temperatures.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "ntp.hpp"
#include "wifiTools.hpp"
#include "temperatures.hpp"
#include "pilotageChaudiere.hpp"

#ifdef lolin_s2_mini
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266HTTPClient.h>
#elif wemos_d1_mini32 
    #include <WiFi.h>
    #include <WebServer.h>
    #include <HTTPClient.h>
#elif adafruit_feather_m0 
    #include <WiFi.h>
    #include <WebServer.h>
    #include <HTTPClient.h>
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif


// les temperatures sont exprimées en 1/10 de degres
int temperatureInterieure;
int temperatureExterieure;
int consigneTemperature = -1;
char localIPCapteurTemperature[50] = "";

char baseUrl[50] = "/getTemperatureInterieure";
int delayRefreshTemperatures = 5000;
int lastRefreshTemperatures= 0;
HTTPClient http;
WiFiClient client;

//----------------------------------------------
//
//      initTemperatures
//
//----------------------------------------------
  void initTemperatures(void){
    Serial.println("=======================");
    Serial.println("     Init Temperatures ");
    Serial.println("-----------------------");
    if (consigneTemperature == -1){
        consigneTemperature = consigneReferenceJour;
    }
    if (strcmp(localIPCapteurTemperature, "") == 0){
        strcpy(localIPCapteurTemperature, IPcapteurTemperature);
    }
}

//----------------------------------------------
//
//      setTemperatureInterieure
//
//----------------------------------------------
void setTemperatureInterieure(int temp){
    if (simulation){
        int valeurVanne = getCommandeVanneChauffage();
        temperatureInterieure -= 1;
        if (valeurVanne > 0){
            temperatureInterieure += valeurVanne / 30;
        }
    } else {
        temperatureInterieure=temp;
    }
    Serial.print("setTemperatureInterieure => "); Serial.println(temperatureInterieure);
}

//----------------------------------------------
//
//      temperatureAtteinte
//
//----------------------------------------------
bool temperatureAtteinte(void){
    char ligne[100];
    if (temperatureInterieure > (consigneTemperature + 15)){
        sprintf(ligne, "consigne = %d, tempInt = %d => temperature atteinte\n", consigneTemperature, temperatureInterieure); Serial.print(ligne);
        return true;
    }
    return false;
}

//----------------------------------------------
//
//      refreshTemperatures
//
//----------------------------------------------
void refreshTemperatures(void){
    
    int tmp;
    tmp = millis() - lastRefreshTemperatures;
    //Serial.println(tmp);
    if (tmp > delayRefreshTemperatures){
        lastRefreshTemperatures = millis();
        char ligne[200];
        char url[100];
        sprintf(url, "http://%s%s", localIPCapteurTemperature, baseUrl);
        http.begin(client, url);
        //http.begin(client, IPcapteurTemperature, 80, baseUrl);
        int httpCode = http.GET();
        // httpCode will be negative on error
        if (httpCode >= 0) {
            // HTTP header has been send and Server response header has been handled
            //sprintf(ligne, "[HTTP] GET %s => code:%d", url, httpCode); Serial.println(ligne);
            char payload[50];

            strcpy(payload, http.getString().c_str());
            //Serial.print("payload="); Serial.println(payload);
            char * pch;
            pch = strchr(payload, '=');
            //Serial.print("temperature=<"); Serial.print(pch+1);Serial.println(">");
            setTemperatureInterieure(atoi(pch+1));

            //Serial.println(payload);
        } else {
            sprintf(ligne, "[HTTP] GET %s => failed:%s", url, http.errorToString(httpCode).c_str()); Serial.println(ligne);
        }
        http.end();
        //Serial.print("fin refresh temperature\n");
        //delay(1000);
    }
}

//----------------------------------------------
//
//      getTemperatureSring
//
//----------------------------------------------
char tmp[10];
char *getTemperatureSring(int temperature){
    sprintf(tmp,"%d.%d", temperature / 10, temperature % 10 );
    return tmp;
}

//----------------------------------------------
//
//      updateTemperature
//
//----------------------------------------------
int updateTemperature(int *ptrTemperature, char *newValue){
    char ligne[100];
    sprintf(ligne, "update temperature debut => ancien = %d, nouveau = %s\n", *ptrTemperature, newValue);Serial.print(ligne);
    float tmp;
    tmp = atof(newValue);
    sprintf(ligne, "update temperature => tmp = %f\n", tmp);Serial.print(ligne);
    tmp *= 10;
    *ptrTemperature = (int)tmp;
    sprintf(ligne, "update temperature nouvelle valeur = %d\n", *ptrTemperature);Serial.print(ligne);
    return *ptrTemperature;
}

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
    return consigneTemperature;
}

//----------------------------------------------
//
//      getIPCapteurTemperature
//
//----------------------------------------------
char *getIPCapteurTemperature(void){
    return localIPCapteurTemperature;
}

//----------------------------------------------
//
//      setIPCapteurTemperature
//
//----------------------------------------------
void setIPCapteurTemperature(char *adresse){
    strcpy(localIPCapteurTemperature, adresse);
}

//----------------------------------------------
//
//      setConsigne
//
//----------------------------------------------
void setConsigne(int consigne){
    consigneTemperature=consigne;
    Serial.print("mise a jour de la consigne : ");
    Serial.println(consigneTemperature);
}

//----------------------------------------------
//
//      handleUpdateConsigne
//
//----------------------------------------------
void handleUpdateConsigne(){
    int tmp = server.arg("consigne").toInt();
    setConsigne(tmp*10);
    server.sendHeader("Location", String("/temperatures"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleTemperatures
//
//----------------------------------------------
void handleTemperatures(void){
    Serial.println("affichage page temperatures");
    char ligne[100];
    String page = "<!DOCTYPE html>\n";
    page += "<style type=\"text/css\">\n";
    page += "    table, th, td {\n";
    page += "        padding: 10px;\n";
    page += "        border: 1px solid black;\n";
    page += "        border-collapse: collapse;\n";
    page += "    }\n";
    page += "    body{\n";
    page += "        margin-left:5%;margin-right:5%; }div#global{width:100%;\n";
    page += "    }\n";
    page += "    div{\n";
    page += "        width:100%; height:200%;margin-left:auto;margin-right:auto;max-width:2000px;\n";
    page += "    }\n";
    page += "</style>\n";

    page += "<html lang='fr'>\n";
    page += "<head>\n";
    page += "   <title> Temperatures </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";

    page += "<body>\n";
    page += "   <h1> Temperatures </h1>\n";
    page += "   <div>\n";
    page += "       <form action='/updateConsigne'>";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th>variable</th>\n";
    page += "                   <th>valeur</th>\n";
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    page += "               <tr>\n";
    page += "                   <td>Temperature interieure</td>\n";
    page += "                   <td>";
    page +=                         getTemperatureInterieure();
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>Temperature exterieure</td>\n";
    page += "                   <td>";
    page +=                         getTemperatureExterieure();
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>Consigne de chauffage</td>\n";
    page += "                   <td>";
    sprintf(ligne, "<input type='number' id='consigne' name='consigne' min='0' max='30' value=%.1f>\n", (double)getConsigne()/10);
    page +=                             ligne;
    page += "                           <button>Valider</button>\n";
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "           </tbody>\n";
    page += "       </table>\n";
    page += "       </form>";
    page += "    <div>\n";

    page += "    <div>\n";
    page += "        <p>Nous sommes \n";
    page +=          getDayString(getDayOfWeek());
    page +=          ", il est ";
    page +=          getFormatedTime();
    page += "    </div>\n";
    page += "    <div>\n";
    page += "        <p><a href='/'>  retour </a></p>\n";
    page += "    </div>\n";
    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}

