//----------------------------------------------
//
//      wifiTools.cpp
//
//----------------------------------------------

#include <Arduino.h>


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "temperature.hpp"

ESP8266WebServer server(80);

int cptTryWifi = 0;
char ipLocale[50] = "";
char ipGateway[50];


//----------------------------------------------
//
//      refreshWifi
//
//----------------------------------------------
void refreshWifi(void)
{
    // test si requete http reçue
    server.handleClient();
}

//----------------------------------------------
//
//      getIpAdresse
//
//----------------------------------------------
char *getIpAdresse(void){
    return ipLocale;
}

//----------------------------------------------
//
//      wifiNotFound
//
//----------------------------------------------
void wifiNotFound(void){
    Serial.println("Page wifiNotFound");
    String page = "Page inexistante";
    page += "\n";
    page += "\n";
    Serial.println(page);
    server.send(404, "text/plain", page);
}

//=========================================
//
//          deconnecteWifi
//
//=========================================
void deconnecteWifi(){
    //Serial.println("deconnecteWifi => debut");
    if (WiFi.status() == WL_CONNECTED){
        //Serial.println("deconnecteWifi => try");
        delay(100);
        WiFi.disconnect();
        //Serial.println("deconnecteWifi => OK");
    }
    //Serial.println("deconnecteWifi => fin");
}

//----------------------------------------------
//
//      handleRoot
//
//----------------------------------------------
void handleRoot() {
    Serial.println("affichage page root");
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
    page += "    <title> Capteur de temperature </title>\n";
    page += "    <meta charset='UTF-8'/>\n";
    page += "    <meta http-equiv='refresh' content='60'> ";
    page += "</head>\n";

    page += "<body>\n";
    page += "    <h1> Capteur de temperature </h1>\n";

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}

//----------------------------------------------
//
//      initWifi
//
//----------------------------------------------
void initWifi(char *ssid, char *pwd){

    Serial.println("======================");
    Serial.println("|     Init Wifi      |");
    Serial.println("----------------------");
    Serial.println("initWifi => debut");
    delay(100);
    //Serial.println("initWifi => check wifi status");
    if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("initWifi => ERROR : No shield detected !!");
        return;
    }
    Serial.println("initWifi => a shield is detected");
    delay(1000);
    deconnecteWifi();
    delay(1000);
    cptTryWifi = 0;
    
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    delay(100);
    WiFi.begin(ssid, pwd);
    int cpt=0;
    int cpt2=0;
    int connected = WiFi.status() != WL_CONNECTED;
    while (connected) {  //Attente de la connexion
        delay(500);
        Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
        if (cpt++ >= 10){
            Serial.println();
            cpt=0;
        }
        if (cpt2++ > 20){
            break;
        }
        connected = WiFi.status() != WL_CONNECTED;
    }

    if (cpt2 > 20){
        // on a  fait 20 tentatives
        // imposible de se connecter au wifi !
        Serial.println("\nWifi non connecte");
        return;
    } else {
        // on a reussit a se connecter au wifi
        Serial.println("");
        Serial.println("WiFi connecte");
    }

    // Print the IP address
    Serial.print("ip locale = ");
    IPAddress tmpIp = WiFi.localIP();
    sprintf(ipLocale,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]); Serial.println(ipLocale);

    //IPAddress gatewayIp = WiFi.gatewayIP();
    tmpIp = WiFi.gatewayIP();
    sprintf(ipGateway,"gateway = %d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]); Serial.println(ipGateway);


    server.begin();
    delay (10);
    server.onNotFound(wifiNotFound);
    server.on("/getTemperatureInterieure", handleGetTemeratureInterieure);
    server.on("/", handleRoot);
    Serial.println("======================");


    Serial.println("initWifi => fin");
}
