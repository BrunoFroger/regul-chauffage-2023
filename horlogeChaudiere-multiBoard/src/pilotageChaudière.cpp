//----------------------------------------------
//
//      pilotagePompe.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include "wifiTools.hpp"
#include "calendrier.hpp"
#include "pilotageChaudiere.hpp"
#include "ntp.hpp"
#include "afficheur.hpp"
#include "regulation.hpp"
#include "temperatures.hpp"

// pinoutRelai defini dans platformIO.ini
int pinRelai = pinoutRelai;
bool circulateurOnOff;
bool lastCirculateurOnOff;
bool chauffageOnOff;
int commandeVanneChauffage;
bool modeRegulation=false;

//----------------------------------------------
//
//      setRegulation
//
//----------------------------------------------
void setRegulation(bool etat){
    modeRegulation = etat;
}

//----------------------------------------------
//
//      getRegulationMode
//
//----------------------------------------------
bool getRegulationMode(void){
    return modeRegulation;
}

//----------------------------------------------
//
//      setRelai
//
//----------------------------------------------
void setRelai(bool etat){
    if (etat){
        digitalWrite(pinRelai, LOW);
        Serial.println("set circulateur ON");
    } else {
        digitalWrite(pinRelai, HIGH);
        Serial.println("set circulateur OFF");
    }
}

//----------------------------------------------
//
//      setPinRelai
//
//----------------------------------------------
void setPinRelai(int pin){
    Serial.print("nouvelle valeur de pin relai : ");
    Serial.println(pin);
    pinRelai = pin;
    pinMode(pinRelai, OUTPUT);
    setRelai(false);
}

//----------------------------------------------
//
//      initChaudiere
//
//----------------------------------------------
void initChaudiere(void){
    Serial.println("=====================================");
    Serial.println("     Init pilotage chaudière (relai) ");
    Serial.println("------------------------------------");
    commandeVanneChauffage=0;
    chauffageOnOff=false;
    setPinRelai(pinRelai);
}

//----------------------------------------------
//
//      getCommandeVanneChauffage
//
//----------------------------------------------
int getCommandeVanneChauffage(){
    return commandeVanneChauffage;
}

//----------------------------------------------
//
//      getPinRelai
//
//----------------------------------------------
int getPinRelai(){
    return pinRelai;
}

//----------------------------------------------
//
//      getCirculateurOnOff
//
//----------------------------------------------
bool getCirculateurOnOff(){
    return circulateurOnOff;
}

//----------------------------------------------
//
//      refreshChaudiere
//
//----------------------------------------------
void refreshChaudiere(void){
    if (!temperatureAtteinte()){
        circulateurOnOff = getChauffageOnOff() && getChauffageStatus();
        if (circulateurOnOff){
            if (getRegulationMode()){
                commandeVanneChauffage = calculDeltaRegulation(commandeVanneChauffage, getConsigne(), getTemperatureInterieure());
            }
        }
    } else {
        circulateurOnOff = false;
        commandeVanneChauffage = 0;
    }

    // pilotage du relai de chauffage
    if (lastCirculateurOnOff != circulateurOnOff){    
        if (circulateurOnOff){
            setRelai(true);
        } else {
            setRelai(false);
        }
        lastCirculateurOnOff = circulateurOnOff;
    }

}

//----------------------------------------------
//
//      handleCommandeChauffage
//
//----------------------------------------------
void handleCommandeChauffage() {
    Serial.println("affichage page commande chauffage");
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
    page += "   <title> Gestion commande de chauffage </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    //page += "   <meta http-equiv='refresh' content='60'> ";
    page += "</head>\n";

    page += "<body>\n";
    page += "   <h1> Gestion commande de chauffage </h1>\n";

    page += "   <div>\n";
    page += "        <p>Nous sommes \n";
    page +=          getDayString(getDayOfWeek());
    page +=          ", il est ";
    page +=          getFormatedTime();
    page += "   <div>\n";

    page += "   <div>\n";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th> variable </th>\n";
    page += "                   <th> valeur </th>\n";
    page += "                   <th> commentaire </th>\n";
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    page += "               <tr>\n";
    page += "                   <td> chauffage </td>\n";
    if (chauffageOnOff){
        page += "               <td align='center'> <a href='/switchChauffageOnOff'>  ON </a></td>\n";
    }else {
        page += "               <td align='center'> <a href='/switchChauffageOnOff'>  OFF </a></td>\n";
    }
    page += "                   <td> activation du chauffage : <br> ON : le calendrier (et la temperature) definissent la mise en route du circulateur <br> OFF : chauffage eteint</td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td> plage <a href='/calendrier'>calendrier</a> </td>\n";
    if (getChauffageStatus()){
        page += "               <td align='center'> ON </td>\n";
    }else {
        page += "               <td align='center'> OFF </td>\n";
    }
    page += "                   <td> ON si dans une plage active dans le calendrier</td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td> mode regulation </td>\n";
    if (getRegulationMode()){
        page += "               <td align='center'> ON </td>\n";
    }else {
        page += "               <td align='center'> OFF </td>\n";
    }
    page += "                   <td> OFF : seul le calendrier pilote l'allumage de la chaudiere<br> ON : la temperature regule l'allulage de la chaudiere </td>\n";
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <td> circulateur </td>\n";
    if (getCirculateurOnOff()){
        page += "               <td align='center'> ON </td>\n";
    }else {
        page += "               <td align='center'> OFF </td>\n";
    }
    page += "                   <td> circulateur en route si chauffage ON et dans une plage calendrier ON </td>\n";
    page += "               </tr>\n";
    if (getRegulationMode()){
        page += "           <tr>\n";
        page += "               <td> vanne chauffage </td>";
        page += "               <td>\n";
        page +=                     commandeVanneChauffage;
        page += "               </td>\n";
        page += "               <td> valeur de la commande appliquee a la vanne de chauffage </td>\n";
        page += "           </tr>\n";
        page += "           <tr>\n";
        page += "               <td> Consigne </td>";
        page += "               <td>\n";
        page +=                     getConsigne();;
        page += "               </td>\n";
        page += "               <td> Consigne de temperature </td>\n";
        page += "           </tr>\n";
        page += "           <tr>\n";
        page += "               <td> temperature interieure </td>";
        page += "               <td>\n";
        page +=                     getTemperatureInterieure();;
        page += "               </td>\n";
        page += "               <td> Valeur mesuree de temperature interieure </td>\n";
        page += "           </tr>\n";
    }
    page += "               <tr>\n";
    page += "                   <td> afficheur </td>\n";
    if (getAfficheurOnOff()){
        page += "               <td align='center'> <a href='/switchAfficheurOnOff'>  ON </a></td>\n";
    }else {
        page += "               <td align='center'> <a href='/switchAfficheurOnOff'>  OFF </a></td>\n";
    }
    page += "                   <td> active ou non l'afficheur de l'arduino </td>\n";
    page += "               </tr>\n";
    page += "           </tbody>\n";
    page += "       </table>\n";
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

//----------------------------------------------
//
//      handleSwitchChauffageOnOff
//
//----------------------------------------------
void handleSwitchChauffageOnOff(void){
    chauffageOnOff = !chauffageOnOff;
    if (chauffageOnOff){
        Serial.println("web : debut chauffage");
    } else {
        Serial.println("web : fin chauffage");
        circulateurOnOff = false;
        commandeVanneChauffage = 0;
    }
    server.sendHeader("Location", String("/commande"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      getChauffageOnOff
//
//----------------------------------------------
bool getChauffageOnOff(){
    return chauffageOnOff;
}

//----------------------------------------------
//
//      getChauffageOnOff
//
//----------------------------------------------
void setChauffageOnOff(bool onOff){
    chauffageOnOff = onOff ;
}