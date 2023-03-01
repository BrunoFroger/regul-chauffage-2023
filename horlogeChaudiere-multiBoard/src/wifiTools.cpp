//----------------------------------------------
//
//      wifiTools.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "calendrier.hpp"
#include "ntp.hpp"
#include "afficheur.hpp"
#include "pilotageChaudiere.hpp"
#include "temperatures.hpp"
#include "plagesModeles.hpp"
#include "sdCard.hpp"

#ifdef lolin_s2_mini
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #define BOARD "lolin s2 mini ESP8266"
    #pragma message("lolin_s2_mini")
    ESP8266WebServer server(80);
#elif wemos_d1_mini32 
    #include <SPI.h>
    #include <WiFi.h>
    #include <WebServer.h>
    #define BOARD "Wemos D1 mini esp32"
    #pragma message("wemos_d1_mini32")
    WebServer server(80);
#elif adafruit_feather_m0 
    #include <WiFi.h>
    #include <WebServer.h>
    #define BOARD "feather esp32"
    #pragma message("adafruit_feather_m0")
    WebServer server(80);
#else 
    #pragma message("Unsupported board selection")
    #error Unsupported board selection.
#endif

char wifiSsid[50];
char wifiPwd[50];

int cptTryWifi = 0;
char ipLocale[50] = "";
char ipGateway[50];
String piedDePage = "";
String enteteDePage = "";

//----------------------------------------------
//
//      getWifiSsid
//
//----------------------------------------------
char *getWifiSsid(){
    return wifiSsid;
}

//----------------------------------------------
//
//      setWifiParameters
//
//----------------------------------------------
void setWifiParameters(char *ssid, char *pwd){
    strcpy(wifiSsid, ssid);
    strcpy(wifiPwd, pwd);
}

//----------------------------------------------
//
//      getIpAddress
//
//----------------------------------------------
char *getIpAddress(){
    if (strlen(ipLocale) == 0) return (char *)"non connecte";
    return ipLocale;
}

//----------------------------------------------
//
//      wifiNotFound
//
//----------------------------------------------
void handlePageNotFound(void){
    Serial.println("handlePageNotFound");
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
//      handleWebRequete
//
//----------------------------------------------
void handleWebRequete(void)
{
    // test si requete http reçue 
    //Serial.print("debut handleWebRequete\n");
    server.handleClient();
    //Serial.print("fin handleWebRequete\n");
}

//----------------------------------------------
//
//      handleSwitchAfficheurOnOff
//
//----------------------------------------------
void handleSwitchAfficheurOnOff(void)
{
    switchAfficheurOnOff();
    server.sendHeader("Location", String("/commande"), true);
    server.send ( 302, "text/plain", "");
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
    page += enteteDePage;
    page += "    <title> Gestion chauffage </title>\n";
    page += "    <meta charset='UTF-8'/>\n";
    page += "    <meta http-equiv='refresh' content='60'> ";
    page += "</head>\n";

    page += "<body>\n";
    page += "    <h1> Gestion chauffage </h1>\n";

    page += "    <div>\n";
    page += "        <p>Nous sommes \n";
    page +=          getDayString(getDayOfWeek());
    page +=          ", il est ";
    page +=          getFormatedTime();
    page += "    <div>\n";

    page += "    <div class='w3-center w3-padding-16'>\n";
    page += "        <p>Ce site permet de gerer l'installation de chauffage </p>\n";
    page += "        <p>Vous avez acces aux informations suivantes :\n";
    page += "        <li> <a href='/commande'>commandes</a> de chauffage,<br>\n";
    page += "        <li> gestion des periodes de chauffage dans le <a href='/calendrier'>calendrier</a>\n";
    page += "        <li> definition des <a href='/plagesModeles'>modeles</a> de plages de chauffage \n";
    page += "        <li> <a href='/temperatures'>temperatures</a>\n";
    page += "        <li> gestion des donnees de <a href='/config'>configuration</a> de la carte SD\n";
    page += "        </p>\n";
    page += "        <p>l'arduino capteur de temperature est a l'adresse : ";
    page +=          getIPCapteurTemperature();
    page += "        </p>\n";
    page += "    </div>\n";
    page +=      piedDePage;

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
bool initWifi(void){

    Serial.println("======================");
    Serial.println("|     Init Wifi      |");
    Serial.println("----------------------");
    Serial.println("initWifi => debut");
    strcpy(wifiSsid, localWifiSsid);
    strcpy(wifiPwd, localWifiPwd);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    //Serial.println("initWifi => check wifi status");
    if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("initWifi => ERROR : No shield detected !!");
        Serial.print("error code = ");
        Serial.println(WiFi.status());
        return false;
    }
    Serial.println("initWifi => a shield is detected");
    delay(1000);
    deconnecteWifi();
    delay(1000);
    cptTryWifi = 0;
    
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(wifiSsid);
    WiFi.mode(WIFI_STA);
    delay(100);
    WiFi.begin(wifiSsid, wifiPwd);
    int cpt=0;
    int cpt2=0;
    int connected = WiFi.status() != WL_CONNECTED;
    while (connected) {  //Attente de la connexion
        delay(500);
        //sprintf(buffer,"ssid = %s, pwd = %s", mesDonneesCapteurs.liveboxSsid, mesDonneesCapteurs.liveboxPwd); Serial.println(buffer);
        //Serial.print("wifiStatus = "); Serial.println(connected);
        //WiFi.begin(mesDonneesCapteurs.liveboxSsid, mesDonneesCapteurs.liveboxPwd);
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
        return false;
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
    server.onNotFound(handlePageNotFound);
    server.on("/", handleRoot);
    server.on("/calendrier", pageCalendrier);
    server.on("/switchChauffageOnOff", handleSwitchChauffageOnOff);
    server.on("/switchAfficheurOnOff", handleSwitchAfficheurOnOff);
    server.on("/switchPlageOnOff", handleSwitchPlageOnOff);
    server.on("/editeHeurePlage", handleEditHeurePlage);
    server.on("/updatePlage", handleUpdatePlage);
    server.on("/deletePlage", handleDeletePlage);
    server.on("/createPlage", handleCreatePlage);
    server.on("/commande", handleCommandeChauffage);
    server.on("/temperatures", handleTemperatures);
    server.on("/updateConsigne", handleUpdateConsigne);
    server.on("/plagesModeles", handlePlagesModeles);
    server.on("/updatePlageModele", handleUpdatePlageModele);
    server.on("/editPlageModele", handleEditPlageModele);
    server.on("/config", handleConfig);
    server.on("/sauveConfig", handleSauveConfig);
    server.on("/sauveCalendrier", handleSauveCalendrier);
    server.on("/chargeCalendrier", handleChargeCalendrier);
    server.on("/selectEnvironnement", handleSelectEnvironnement);
    server.on("/listFichierConfig", handleListFichierConfiguration);
    Serial.println("======================");

    Serial.println("initWifi => fin");

    // 
    //  Definition du pied de page
    //
    piedDePage += "    <div class='w3-center w3-padding-16'>\n";
    piedDePage += "        <p>Ce serveur est hébergé sur un arduino (";
    piedDePage +=          BOARD;
    piedDePage +=          ")</p>\n";
    piedDePage += "        <i>Créé par ";
    piedDePage +=          copyright;
    piedDePage +=          "</i>\n";
    piedDePage += "    </div>\n";

    // 
    //  Definition de l'entete de page
    //
    enteteDePage += "    <div>";
    enteteDePage += "        <table>";
    enteteDePage += "            <tr>";
    enteteDePage += "                <td>";
    enteteDePage += "                    <a href='/'>home</a>";
    enteteDePage += "                </td>";
    enteteDePage += "                <td>";
    enteteDePage += "                    <a href='commande'>commande</a>";
    enteteDePage += "                </td>";
    enteteDePage += "                <td>";
    enteteDePage += "                    <a href='calendrier'>calendrier</a>";
    enteteDePage += "                </td>";
    enteteDePage += "                <td>";
    enteteDePage += "                    <a href='temperatures'>temperatures</a>";
    enteteDePage += "                </td>";
    enteteDePage += "                <td>";
    enteteDePage += "                    <a href='config'>config</a>";
    enteteDePage += "                </td>";
    enteteDePage += "            </tr>";
    enteteDePage += "        </table>";
    enteteDePage += "    </div>";
    return true;
}