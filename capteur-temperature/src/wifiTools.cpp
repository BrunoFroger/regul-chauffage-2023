//----------------------------------------------
//
//      wifiTools.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "ntp.hpp"
#include "afficheur.hpp"
#include "temperature.hpp"

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
char wifiMode[10];

int cptTryWifi = 0;
char ipLocale[50] = "";
char ipGateway[50];
char netMask[50];
String piedDePage = "";
String enteteDePage = "";
bool wifiConnected = false;

//----------------------------------------------
//
//      isWifiConnected
//
//----------------------------------------------
bool isWifiConnected(void){
    return wifiConnected;
}

//----------------------------------------------
//
//      getWifiSsid
//
//----------------------------------------------
char *getWifiSsid(void){
    return wifiSsid;
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
//      isApMode
//
//----------------------------------------------
bool isApMode(){
    if (strcmp(wifiMode, "AP") == 0)
        return true;
    else
        return false;
}

//----------------------------------------------
//
//      getRSSI
//
//----------------------------------------------
int getRSSI(){
    return WiFi.RSSI();
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
    wifiConnected = false;
}

//----------------------------------------------
//
//      handleWebRequete
//
//----------------------------------------------
void handleWebRequete(void){
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
void handleSwitchAfficheurOnOff(void){
    switchAfficheurOnOff();
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleRoot
//
//----------------------------------------------
void handleRoot() {
    char tmp[20];
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
    page += "    <title> Capteur temperature </title>\n";
    page += "    <meta charset='UTF-8'/>\n";
    page += "</head>\n";

    page += "<body>\n";
    page += "    <h1> Capteur temperature </h1>\n";

    page += "    <div>\n";
    page += "        <p>Nous sommes \n";
    page +=          getDayString(getDayOfWeek());
    page +=          ", il est ";
    page +=          getFormatedTime();
    page += "    <div>\n";

    page += "    <div class='w3-center w3-padding-16'>\n";
    page += "    <table>";
    page += "       <tr>";
    page += "           <td>";
    page += "               Consigne : ";
    page += "           </td>";
    page += "           <td>";
    sprintf(tmp, "%.1f", (double)getConsigne()/10);
    page +=                 tmp;
    page += "           </td>";
    page += "       </tr>";
    page += "       <tr>";
    page += "           <td>";
    page += "               Temperature interieure";
    page += "           </td>";
    page += "           <td>";
    sprintf(tmp, "%.1f", (double)getTemperatureInterieure()/10);
    page +=                 tmp;
    page += "           </td>";
    page += "       </tr>";
    page += "       <tr>";
    page += "           <td>";
    page += "               Temperature exterieure";
    page += "           </td>";
    page += "           <td>";
    sprintf(tmp, "%.1f", (double)getTemperatureExterieure()/10);
    page +=                 tmp;
    page += "           </td>";
    page += "       </tr>";
    page += "       <tr>";
    page += "           <td>";
    page += "               Qualité du signal Wifi (RSSI)";
    page += "           </td>";
    page += "           <td>";
    page +=                 getRSSI();
    page += "           db</td>";
    page += "       </tr>";
    page += "    </table>";
    page += "    </div>\n";
    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}

//----------------------------------------------
//
//      connectWifi
//
//----------------------------------------------
bool connectWifi(void){
    Serial.println("connectWifi => debut");
    //WiFi.onEvent(onWiFiEvent);
    if (strcmp(wifiMode, "AP") == 0){
        Serial.println("Initialisation du mode Acces Point");
        WiFi.mode(WIFI_AP);
    } else {
        Serial.println("Initialisation du mode Station");
        WiFi.mode(WIFI_STA);
    }

    if (strcmp(wifiMode, "AP") == 0){
        delay(500);
        IPAddress apLocalIp(192,168,10,1);
        IPAddress apGateway(192,168,10,0);
        IPAddress apSubnetMask(255,255,255,0);
        WiFi.softAPConfig(apLocalIp, apGateway, apSubnetMask);
        WiFi.softAP(wifiSsid, wifiPwd);
        delay(1000);
        sprintf(ipGateway,"gateway = %d.%d.%d.%d",apGateway[0],apGateway[1],apGateway[2],apGateway[3]); Serial.println(ipGateway);
        sprintf(ipLocale,"LocalIp = %d.%d.%d.%d",apLocalIp[0],apLocalIp[1],apLocalIp[2],apLocalIp[3]); Serial.println(ipLocale);
        sprintf(netMask,"netmask = %d.%d.%d.%d",apSubnetMask[0],apSubnetMask[1],apSubnetMask[2],apSubnetMask[3]); Serial.println(netMask);
    } else {
        WiFi.disconnect();
        delay(100);
        //Serial.println("initWifi => check wifi status");
        if (WiFi.status() == WL_NO_SHIELD){
            Serial.println("initWifi => ERROR : No shield detected !!");
            Serial.print("error code = ");
            Serial.println(WiFi.status());
            wifiConnected = false;
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
            wifiConnected = false;
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
        Serial.println((String)"RSSI = " + WiFi.RSSI() + " db");
        wifiConnected = true;
    }
    Serial.println("wifi connecte ok");

    server.begin();
    server.onNotFound(handlePageNotFound);
    server.on("/", handleRoot);
    server.on("/switchAfficheurOnOff", handleSwitchAfficheurOnOff);
    server.on("/getTemperatureInterieure", handleGetTemeratureInterieure);
    server.on("/getTemperatureExterieure", handleGetTemeratureExterieure);
    Serial.println("======================");

    Serial.println("connectWifi => fin");
    return true;
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
    //Serial.println("Connexion en mode : " + String(wifiMode));
    if (strcmp(wifiSsid, "") == 0){
        strcpy(wifiSsid, localWifiSsid);
        strcpy(wifiPwd, localWifiPwd);
        strcpy(wifiMode, "STATION");
    }
    if (connectWifi() == false){
        return false;
    }

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
    /*enteteDePage += "        <table>";
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
    enteteDePage += "        </table>";*/
    enteteDePage += "    </div>";
    return true;
}

//----------------------------------------------
//
//      setWifiParameters
//
//----------------------------------------------
void setWifiParameters(char *ssid, char *pwd, char *mode){
    char ligne[100];
    strcpy(wifiSsid, ssid);
    strcpy(wifiPwd, pwd);
    strcpy(wifiMode, mode);
    sprintf(ligne, "setWifiParameters : ssid = <%s> / pwd = <%s> / mode = <%s>\n", ssid, pwd, mode); Serial.print(ligne);
    //connectWifi();
}
