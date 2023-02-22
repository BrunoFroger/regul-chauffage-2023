//----------------------------------------------
//
//      sdCard.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

#include "temperatures.hpp"
#include "wifiTools.hpp"
#include "pilotageChaudiere.hpp"

// change this to match your SD shield or module;
// WeMos Micro SD Shield V1.0.0: D8
// LOLIN Micro SD Shield V1.2.0: D4 (Default)
const int chipSelect = D4;

File myFile;
String environnement;
bool insideEnvironnement;

struct structEnvironnement{
    String nom;
    String wifiSsid;
    String wifiPwd;
    String ipTempInt;
};

structEnvironnement *listeEnvironnement[10];
structEnvironnement *newEnvironnement;

//----------------------------------------------
//
//      getValeur
//
//----------------------------------------------
String getValeur(String ligne){
    String tmp;
    int i = ligne.indexOf('=') + 1;
    while (tmp[i++] == ' ');
    tmp = ligne.substring(i);
    return tmp;
}

//----------------------------------------------
//
//      initEnvironnement
//
//----------------------------------------------
void initEnvironnement(String ligne){
    String nomEnvironnement = ligne.substring(1, ligne.indexOf(']'));
    Serial.println("initEnvironnement : nom de l'environemment => <" + nomEnvironnement + ">");
    newEnvironnement = (structEnvironnement*) malloc(sizeof(structEnvironnement));
    newEnvironnement->nom = nomEnvironnement;
    newEnvironnement->ipTempInt = "";
    newEnvironnement->wifiSsid = "";
    newEnvironnement->wifiPwd = "";
    for (int i = 0 ; i < 10 ; i++){
        if (listeEnvironnement[i] == nullptr){
            listeEnvironnement[i] = newEnvironnement;
            break;
        }
    }
    insideEnvironnement = true;
}

//----------------------------------------------
//
//      analyseLigne
//
//----------------------------------------------
void analyseLigne(String ligne){
    //Serial.println("sdCard => analyse ligne : " + ligne);
    //Serial.println(ligne);
    if (ligne.length() == 0){
        insideEnvironnement = false;
        return;
    } 
    if (ligne[0] == '#') return;
    else if (ligne.startsWith(String("["))){
        if (!insideEnvironnement){
            // traitement de debut d'environnement
            Serial.println("traitement de debut d'environnement");
            initEnvironnement(ligne);
        } else {
            Serial.print("ERREUR ......\n");
            Serial.print("Impossible de commencer un nouvel environnement\n");
            Serial.print("un environnement est deja ouvert\n");
        }
    } else if (ligne.startsWith(String("CONSIGNE"))){
        // traitement de la valeur de consigne
        Serial.println("traitement de la consigne");
        setConsigne(getValeur(ligne).toInt());
    } else if (ligne.startsWith("ENV")){
        // fixe l'environnement a traiter
        Serial.println("fixe l'environnement a traiter");
        environnement = getValeur(ligne);
        Serial.print("Utilisation de l'environnement : <");
        Serial.print(environnement);
        Serial.println(">");
    } else if (ligne.startsWith("WIFI_SSID")){
        // fixe le nom du ssid
        newEnvironnement->wifiSsid = getValeur(ligne);
        Serial.println("fixe nom du ssid");
        Serial.print("ssid = <");
        Serial.print(newEnvironnement->wifiSsid);
        Serial.println(">");
    } else if (ligne.startsWith("WIFI_PWD")){
        // fixe le nom du wifi Pwd
        newEnvironnement->wifiPwd = getValeur(ligne);
        Serial.println("fixe nom du wifi Pwd");
        Serial.print("ssid = <");
        Serial.print(newEnvironnement->wifiPwd);
        Serial.println(">");
    } else if (ligne.startsWith("IP_TEMP_INT")){
        // fixe l'adresse du capteur de temperature
        newEnvironnement->ipTempInt = getValeur(ligne);
        Serial.println("fixe l'adresse du capteur de temperature");
        Serial.print("ssid = <");
        Serial.print(newEnvironnement->ipTempInt);
        Serial.println(">");
    } else {
        Serial.print("ligne invalide : ");
        Serial.println(ligne);
    }
}

//----------------------------------------------
//
//      readConfig
//
//----------------------------------------------
void readConfig(void){
    String ligne;
    String filename = "chaudiere/config.txt";
    myFile = SD.open(filename);

    if (myFile){
        Serial.println("Analyse du fichier de config sur carte SD");
        while (myFile.available()){
            ligne = myFile.readStringUntil('\n');
            analyseLigne(ligne);
        }
        myFile.close();
    } else {
        Serial.print("Le fichier ");
        Serial.print(filename);
        Serial.println(" n'existe pas");
    }
}

//----------------------------------------------
//
//      sauvegardeFichier
//
//----------------------------------------------
void sauvegardeFichier(String filename, const uint8_t *datas, int size){
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile){
        Serial.println("sdCard => sauvegarde de " + filename);
        myFile.write(datas, size);
        myFile.close();
        Serial.print("sdCard => taille du fichier : ");
        Serial.println(size);
    } else {
        Serial.print("impossible d'ouvrir le fichier ");
        Serial.print(filename);
        Serial.println();
    }
}

//----------------------------------------------
//
//      lireFichier
//
//----------------------------------------------
void lireFichier(String filename, void *datas){
    myFile = SD.open(filename);
    if (myFile){
        Serial.println("lecture de " + filename);
        while(myFile.available()){
            myFile.readBytes((char *)datas++, 1);
        }
        myFile.close();
    } else {
        Serial.print("impossible d'ouvrir le fichier ");
        Serial.print(filename);
        Serial.println();
    }
}

//----------------------------------------------
//
//      writeConfig
//
//----------------------------------------------
void writeConfig(void){
    String page="";
    page += "###########################################\n";
    page += "#\n";
    page += "#           Chauffage (config)\n";
    page += "#\n";
    page += "###########################################\n";
    page += "\n";
    page += "# Ce fichier contient les parametres\n";
    page += "# de pilotage d'une chaudiere\n";
    page += "\n";
    page += "# les regles de syntaxe de ce fichier sont les suivantes :\n";
    page += "#	- les lignes qui commences par # sont des commentaires\n";
    page += "#	- le fichier doit se terminer par une ligne vide\n";
    page += "#	- le nom d'une variable doit etre en debut de ligne\n";
    page += "#	- une seule variable par ligne\n";
    page += "#	- les temperatures sont exprimees en 1/10eme de degres (200 pour une temperature de 20°)\n";
    page += "#  - les environnements doivent etre definis avant les parametres generaux\n";
    page += "\n";
    page += "# Les paramètres disponibles sont listés ci dessous \n";
    page += "# avec la valeur par defaut s'ils ne sont pas renseignés\n";
    page += "\n";
    page += "Parametres generaux :\n";
    page += "# CONSIGNE = 200 : consigne de temperature a maintenir 20,0°\n";
    page += "# CHAUFFAGE = OFF : determine si le chauffage est active ou non lors du demarrage du systeme\n";
    page += "# ENV = maison : definition de l'environnement a utiliser\n";
    page += "# PIN_RELAI = pinNumber : definition de la broche de l'arduino sur laquelle est connectee le relai\n";
    page += "\n";
    page += "Parametres d'environnement :\n";
    page += "# [NOM_ENV] => liste de definitions de données specifiques a un environnement jusqu'a une ligne vide\n";
    page += "# WIFI_SSID = ssid : nom du point d'accces wifi\n";
    page += "# WIFI_PWD = passwd : passwd du point d'acces wifi\n";
    page += "# IP_TEMP_INT : adresse IP du capteur de temperature interieure (reference pour la regulation)\n";
    page += "\n";
    
    for (int i = 0 ; i < 10 ; i++){
        if (listeEnvironnement[i] != nullptr){
            page += "[" + listeEnvironnement[i]->nom + "]\n";
            page += "WIFI_SSID = " + listeEnvironnement[i]->wifiSsid + "\n";
            page += "WIFI_PWD = " + listeEnvironnement[i]->wifiPwd + "\n";
            page += "IP_TEMP_INT = " + listeEnvironnement[i]->ipTempInt + "\n";
            page += "\n";
        } else {
            break;
        }
    }

    if (getConsigne() != consigneReferenceJour){
        page += "CONSIGNE = " + getConsigne();
        page += "]\n";
    }
    if (getChauffageOnOff()){
        page += "CHAUFFAGE = ON\n";
    } else {
        page += "CHAUFFAGE = OFF\n";
    }
    page += "ENV = " + environnement + "\n";
    page += "PIN_RELAI = ";
    page += getPinRelai();
    page += "\n" ;
    page += "\n";
    Serial.print(page);

}

//----------------------------------------------
//
//      sdcardInit
//
//----------------------------------------------
void sdcardInit(void){

    Serial.print("Initializing SD card  ...   ");
    insideEnvironnement=false;

    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");
    readConfig();
}

//----------------------------------------------
//
//      handleSauveConfig
//
//----------------------------------------------
void handleSauveConfig(void){
    writeConfig();
    server.sendHeader("Location", String("/config"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleConfig
//
//----------------------------------------------
void handleConfig(void){
    Serial.println("affichage page configuration");
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
    page += "   <title> Configuration </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";

    page += "<body>\n";
    page += "   <h1> Configuration </h1>\n";
    page += "   <p>Cette page affiche les parametres contenus dans la carte SD\n";
    page += "   <div>\n";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th>variable</th>\n";
    page += "                   <th>valeur</th>\n";
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    page += "               <tr>\n";
    page += "                   <td>Consigne</td>\n";
    page += "                   <td>";
    page +=                         (double)getConsigne()/10.0;
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>Environnement</td>\n";
    page += "                   <td>";
    page +=                         environnement;
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "           </tbody>\n";
    page += "       </table>\n";
    page += "    </div>\n";

    page += "   <p>liste des environnements definis\n";
    page += "    <div>\n";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th>nom</th>\n";
    page += "                   <th>Wifi SSID</th>\n";
    page += "                   <th>Wifi pwd</th>\n";
    page += "                   <th>IP capteur</th>\n";
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    for (int i = 0 ; i < 10 ; i++){
        if (listeEnvironnement[i] != nullptr){
            page += "       <tr>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i]->nom;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i]->wifiSsid;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i]->wifiPwd;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i]->ipTempInt;
            page += "           </td>\n";
            page += "       </tr>\n";
        }
    }
    page += "           <tbody>\n";
    page += "       </table>\n";
    page += "    <div>\n";
    page += "        <p><a href='/'>  retour </a></p>\n";
    page += "    </div>\n";

    page += "    <div>\n";
    page += "        <p><a href='/sauveConfig'>  sauver la configuration </a></p>\n";
    page += "    </div>\n";

    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}