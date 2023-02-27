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
#ifdef lolin_s2_mini
    const int chipSelect = D4;
#elif wemos_d1_mini32 
    const int chipSelect = D8;
#elif adafruit_feather_m0 
    const int chipSelect = D8;
#else 
    #error Unsupported board selection.
#endif

File myFile;
String environnement;
bool insideEnvironnement;
int pin_relai;
bool SdChauffageOnOff;
int SdConsigne;

#define NB_ENVIRONNEMENTS   10

struct structEnvironnement{
    int index;
    char nom[20];
    char wifiSsid[20];
    char wifiPwd[25];
    char ipTempInt[20];
};

structEnvironnement listeEnvironnement[NB_ENVIRONNEMENTS];
//structEnvironnement *newEnvironnement;
int indexEnvironnementCourant;

String tmpValeur;

//----------------------------------------------
//
//      getValeur
//
//----------------------------------------------
String getValeur(String ligne){
    tmpValeur = "";
    //Serial.print("environnement : ");
    //Serial.println(indexEnvironnementCourant);
    int i = ligne.indexOf('=') + 1; // i = debut chaine apres le =
    while (tmpValeur[i++] == ' '); // on supprime les blancs apres le =
    tmpValeur = ligne.substring(i); // i correspond au debut de la chaine definissant la valeur
    //Serial.println("getValeur : <" + tmpValeur + ">");
    return tmpValeur;
}

//----------------------------------------------
//
//      listeEnvironnements
//
//----------------------------------------------
void listeEnvironnements(void){
    char ligne[100];
    Serial.print("+-----+-----------+----------------------+----------------------+-------------------+\n");
    Serial.print("| idx |   nom     |      ssid            |     pwd              |   ip capt temp    |\n");
    Serial.print("+-----+-----------+----------------------+----------------------+-------------------+\n");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") != 0){
            structEnvironnement *env = &listeEnvironnement[i];
            sprintf(ligne, "|  %d  |%10s | %20s | %20s | %17s |\n", i, env->nom, env->wifiSsid, env->wifiPwd, env->ipTempInt);
            Serial.print(ligne);
        }
    }
    Serial.print("+-----+-----------+----------------------+----------------------+-------------------+\n");
}

//----------------------------------------------
//
//      initEnvironnement
//
//----------------------------------------------
void initEnvironnement(String ligne){
    char nomEnvironnement[20];
    strcpy(nomEnvironnement,ligne.substring(1, ligne.indexOf(']')).c_str());
    //Serial.print("initEnvironnement : nom de l'environemment => <");
    //Serial.print(nomEnvironnement);
    //Serial.println(">");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") == 0){
            structEnvironnement *env = &listeEnvironnement[i];
            env->index = i;
            strcpy(env->nom, nomEnvironnement);
            strcpy(env->wifiSsid, "");
            strcpy(env->wifiPwd, "");
            strcpy(env->ipTempInt, "");
            indexEnvironnementCourant = i;
            break;
        }
    }
    insideEnvironnement = true;
}

//----------------------------------------------
//
//      setEnvironnement
//
//----------------------------------------------
void setEnvironnement(String envName){
    //Serial.println("setEnvironnement : nom de l'environemment => <" + envName + ">");
    for (int i = 0 ; i < NB_ENVIRONNEMENTS ; i++){
        structEnvironnement *env = &listeEnvironnement[i];
        if (envName = env->nom){
            setIPCapteurTemperature(env->ipTempInt);
            setWifiParameters(env->wifiSsid, env->wifiPwd);
            return;
        }
    }
    Serial.print("ERREUR : environnement ");
    Serial.print(envName);
    Serial.println(" inconnu");

}

//----------------------------------------------
//
//      analyseLigne
//
//----------------------------------------------
void analyseLigne(String ligne){
    String tmpData;
    //Serial.println("sdCard => analyse ligne : " + ligne);
    //Serial.println(ligne);
    if (ligne.length() == 0){
        if (insideEnvironnement){
            //listeEnvironnements();
            insideEnvironnement = false;
        }
        return;
    } 
    if (ligne[0] == '#') return;
    //Serial.println("----------------------------");
    tmpData = getValeur(ligne);
    if (ligne.startsWith(String("["))){
        if (!insideEnvironnement){
            // traitement de debut d'environnement
            //Serial.println("traitement de debut d'environnement");
            initEnvironnement(ligne);
        } else {
            Serial.print("ERREUR ......\n");
            Serial.print("Impossible de commencer un nouvel environnement\n");
            Serial.print("un environnement est deja ouvert\n");
        }
    } else if (ligne.startsWith(String("CONSIGNE"))){
        // traitement de la valeur de consigne
        //Serial.println("traitement de la consigne");
        SdConsigne = tmpData.toInt();
        setConsigne(SdConsigne);

        //Serial.print("consigne = ");
        ///Serial.print(getConsigne());
        //Serial.println();
    } else if (ligne.startsWith("ENV")){
        // fixe l'environnement a traiter
        //Serial.println("fixe l'environnement a traiter");
        environnement = tmpData;
        //Serial.print("Utilisation de l'environnement : <");
        //Serial.print(environnement);
        //Serial.println(">");
        setEnvironnement(environnement);
    } else if (ligne.startsWith("WIFI_SSID")){
        // fixe le nom du ssid
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->wifiSsid, tmpData.c_str());
        //Serial.println("fixe nom du ssid");
        //Serial.print("ssid = <");
        //Serial.print(env->wifiSsid);
        //Serial.println(">");
    } else if (ligne.startsWith("WIFI_PWD")){
        // fixe le nom du wifi Pwd
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->wifiPwd, tmpData.c_str());
        //Serial.println("fixe nom du wifi Pwd");
        //Serial.print("pwd = <");
        //Serial.print(env->wifiPwd);
        //Serial.println(">");
    } else if (ligne.startsWith("IP_TEMP_INT")){
        // fixe l'adresse du capteur de temperature
        structEnvironnement *env = &listeEnvironnement[indexEnvironnementCourant];
        strcpy(env->ipTempInt, tmpData.c_str());
        //Serial.println("fixe l'adresse du capteur de temperature");
        //Serial.print("ip temp int = <");
        //Serial.print(env->ipTempInt);
        //Serial.println(">");
    } else if (ligne.startsWith("CHAUFFAGE")){
        // fixe l'activation ou non du chauffage
        if (tmpData == "ON"){
            SdChauffageOnOff = true;
        } else {
            SdChauffageOnOff = false;
        }
        setChauffageOnOff(SdChauffageOnOff);
    } else if (ligne.startsWith("PIN_RELAI")){
        // fixe la broche sur laquelle est connectee le relai de pilotage
        //Serial.print("AnalyseLigne : pinRelai = ");
        //Serial.println(tmpData.toInt());
        setPinRelai(tmpData.toInt());
    } else if (ligne.startsWith("REGULATION")){
        // fixe la broche sur laquelle est connectee le relai de pilotage
        Serial.print("AnalyseLigne : regulation = ");
        Serial.println(tmpData);
        if (tmpData == "ON"){
            setRegulation(true);
        } else {
            setRegulation(false);
        }
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
    String filename = "/chaudiere/config.txt";
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
void sauvegardeFichier(String filename, String datas){
    if (SD.exists(filename)){
        SD.remove(filename);
    }
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile){
        Serial.println("sdCard => sauvegarde de " + filename);
        myFile.print(datas);
        myFile.close();
        Serial.print("sdCard => taille du fichier : ");
        Serial.println(sizeof(datas));
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
    page += "# Parametres generaux :\n";
    page += "# CONSIGNE = 200 : consigne de temperature a maintenir 20,0° (exprime en 1/10 de degre)\n";
    page += "# CHAUFFAGE = OFF : determine si le chauffage est active ou non lors du demarrage du systeme\n";
    page += "# ENV = maison : definition de l'environnement a utiliser\n";
    page += "# PIN_RELAI = pinNumber : definition de la broche de l'arduino sur laquelle est connectee le relai\n";
    page += "\n";
    page += "# Parametres d'environnement :\n";
    page += "# [NOM_ENV] => liste de definitions de données specifiques a un environnement jusqu'a une ligne vide\n";
    page += "# WIFI_SSID = ssid : nom du point d'accces wifi\n";
    page += "# WIFI_PWD = passwd : passwd du point d'acces wifi\n";
    page += "# IP_TEMP_INT : adresse IP du capteur de temperature interieure (reference pour la regulation)\n";
    page += "# PIN_RELAI = pinNumber : definition de la broche de l'arduino sur laquelle est connectee le relai (26 correspond a D0 sur wemos esp 32, 16 = D0 sur wemos D1 mini)\n";
    page += "# REGULATION = OFF : prise en compte ou non de la temperature pour piloter la chaudiere (si OFF seul le calendrier pilote la chaudiere)\n";
    page += "\n";
    
    for (int i = 0 ; i < 10 ; i++){
        if (strcmp(listeEnvironnement[i].nom, "") != 0){
            page += "[";
            page += listeEnvironnement[i].nom ;
            page += "]\n";
            page += "WIFI_SSID = ";
            page += listeEnvironnement[i].wifiSsid;
            page += "\n";
            page += "WIFI_PWD = ";
            page += listeEnvironnement[i].wifiPwd;
            page += "\n";
            page += "IP_TEMP_INT = ";
            page += listeEnvironnement[i].ipTempInt;
            page += "\n";
            page += "\n";
        } else {
            break;
        }
    }

    page += "CONSIGNE = ";
    page += getConsigne();
    page += "\n" ;

    if (SdChauffageOnOff){
        page += "CHAUFFAGE = ON\n";
    } else {
        page += "CHAUFFAGE = OFF\n";
    }
    page += "ENV = " + environnement + "\n";
    page += "PIN_RELAI = ";
    page += getPinRelai();
    page += "\n" ;
    if (getRegulationMode()){
        page += "REGULATION = ON\n";
    } else {
        page += "REGULATION = OFF\n";
    }
    page += "\n" ;
    page += "\n";
    Serial.print(page);


    String filename = "/chaudiere/config.txt";
    if (SD.exists(filename)){
        SD.remove(filename);
    }
    myFile = SD.open(filename, FILE_WRITE);
    if (myFile){
        myFile.print(page);
        myFile.close();
    }

}

//----------------------------------------------
//
//      sdcardInit
//
//----------------------------------------------
bool sdcardInit(void){

    Serial.print("Initializing SD card  ...   ");
    insideEnvironnement=false;
    for (int i = 0 ; i < 10 ; i++){
        strcpy(listeEnvironnement[i].nom, "");
    }
    //listeEnvironnements();

    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return false;
    }
    Serial.println("initialization done.");
    readConfig();
    Serial.print("consigne        = ");
    Serial.println(getConsigne());
    Serial.print("chauffage       = ");
    if (getChauffageOnOff()) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    Serial.print("environnement   = ");
    Serial.println(environnement);
    Serial.print("pin relai       = ");
    Serial.println(getPinRelai());
    Serial.print("mode regulation = ");
    if (getRegulationMode()) {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    listeEnvironnements();
    return true;
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
    page += enteteDePage;
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
    page +=                         (double)(getConsigne())/10.0;
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>Environnement utilise</td>\n";
    page += "                   <td>";
    page +=                         environnement;
    page += "                   </td>\n";
    page += "               <tr>\n";
    page += "                   <td>Pin relai</td>\n";
    page += "                   <td>";
    page +=                         getPinRelai();
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>Regulation</td>\n";
    page += "                   <td>";
    if (getRegulationMode()){
        page += "                  ON\n";
    } else {
        page += "                  OFF\n";
    }
    page += "                   </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td>chauffage</td>\n";
    page += "                   <td>";
    if (getChauffageOnOff()){
        page += "                  ON\n";
    } else {
        page += "                  OFF\n";
    }
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
        if (strcmp(listeEnvironnement[i].nom,"") != 0){
            page += "       <tr>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i].nom;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i].wifiSsid;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i].wifiPwd;
            page += "           </td>\n";
            page += "           <td>\n";
            page +=                 listeEnvironnement[i].ipTempInt;
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

    page += "    <div>\n";
    page += "        <p><a href='/listFichierConfig'>  afficher fichier de configuration </a></p>\n";
    page += "    </div>\n";

    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}



//----------------------------------------------
//
//      handleListFichierConfiguration
//
//----------------------------------------------
void handleListFichierConfiguration(void){
    Serial.println("affichage page liste fichier configuration");
    String ligne;
    String filename = "/chaudiere/config.txt";
    myFile = SD.open(filename);

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
    page += "   <title> fichier de configuration sur carte SD</title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";

    page += "<body>\n";
    page += "   <h1>fichier de configuration sur carte SD</h1>\n";
    if (myFile){
        while (myFile.available()){
            ligne = myFile.readStringUntil('\n');
            page += ligne;
            page += "<br>\n";
        }
        myFile.close();
    } else {
        Serial.print("Le fichier ");
        Serial.print(filename);
        Serial.println(" n'existe pas");
    }

    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML
    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}