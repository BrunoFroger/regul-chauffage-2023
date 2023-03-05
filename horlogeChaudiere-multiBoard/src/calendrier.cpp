//----------------------------------------------
//
//      calendrier.cpp
//
//----------------------------------------------


#include <Arduino.h>
#include <stdlib.h>

#include "ntp.hpp"
#include "wifiTools.hpp"
#include "calendrier.hpp"
#include "temperatures.hpp"
#include "sdCard.hpp"

structCalendrier calendrier;
bool chauffageCalendrier = false;
int jourEnEdition, plageEnEdition;

//----------------------------------------------
//
//      setPlage
//
//----------------------------------------------
void setPlage(plageHoraire *ptrPlage, int8_t modele, char *nom, 
    int8_t hDeb, int8_t mDeb, int8_t hFin, int8_t mFin, 
    bool chauffage, int consigne, bool active){
    
    ptrPlage->modele = modele;
    strcpy(ptrPlage->nomPlage, nom);
    ptrPlage->heureDebut = hDeb;
    ptrPlage->minuteDebut = mDeb;
    ptrPlage->heureFin = hFin;
    ptrPlage->minuteFin = mFin;
    ptrPlage->chauffageOnOff = chauffage;
    ptrPlage->consigne = consigne;
    ptrPlage->plageActive = active;
}

//----------------------------------------------
//
//      setPlageNuit
//
//----------------------------------------------
void setPlageNuit(plageHoraire *ptrPlage){
    setPlage(ptrPlage, PLAGE_MODELE_NUIT, "Nuit", 22, 30, 6, 30, true, consigneReferenceNuit, true);

    /*ptrPlage->modele = PLAGE_MODELE_NUIT;
    strcpy(ptrPlage->nomPlage,"Nuit");
    ptrPlage->heureDebut = 22;
    ptrPlage->minuteDebut = 30;
    ptrPlage->heureFin = 6;
    ptrPlage->minuteFin = 30;
    ptrPlage->chauffageOnOff = true;
    ptrPlage->consigne = consigneReferenceNuit;
    ptrPlage->plageActive = true;*/
    //Serial.println("plage Nuit selectionnee 22:30 => 6:30");
}

//----------------------------------------------
//
//      setPlageLever
//
//----------------------------------------------
void setPlageLever(plageHoraire *ptrPlage){
    setPlage(ptrPlage, PLAGE_MODELE_LEVER, "Lever", 6, 30, 8, 30, true, consigneReferenceJour, true);

    /*ptrPlage->modele = PLAGE_MODELE_LEVER;
    strcpy(ptrPlage->nomPlage,"Lever");
    ptrPlage->heureDebut = 6;
    ptrPlage->minuteDebut = 30;
    ptrPlage->heureFin = 8;
    ptrPlage->minuteFin = 30;
    ptrPlage->chauffageOnOff = true;
    ptrPlage->consigne = consigneReferenceJour;
    ptrPlage->plageActive = true;*/
    //Serial.println("plage Lever selectionnee 6:30 => 8:30");
}

//----------------------------------------------
//
//      setPlageCoucher
//
//----------------------------------------------
void setPlageCoucher(plageHoraire *ptrPlage){
    ptrPlage->modele = PLAGE_MODELE_COUCHER;
    strcpy(ptrPlage->nomPlage,"Coucher");
    ptrPlage->heureDebut = 21;
    ptrPlage->minuteDebut = 0;
    ptrPlage->heureFin = 23;
    ptrPlage->minuteFin = 0;
    ptrPlage->chauffageOnOff = true;
    ptrPlage->consigne = consigneReferenceJour;
    ptrPlage->plageActive = true;
    //Serial.println("plage Coucher selectionnee 21:00 => 23:00");
}

//----------------------------------------------
//
//      setPlageAll
//
//----------------------------------------------
void setPlageAll(plageHoraire *ptrPlage){
    ptrPlage->modele = PLAGE_MODELE_ALL;
    strcpy(ptrPlage->nomPlage,"Entiere");
    ptrPlage->heureDebut = 0;
    ptrPlage->minuteDebut = 0;
    ptrPlage->heureFin = 23;
    ptrPlage->minuteFin = 59;
    ptrPlage->chauffageOnOff = true;
    ptrPlage->consigne = consigneReferenceJour;
    ptrPlage->plageActive = false;
    //Serial.println("plage 24H selectionnee 0:00 => 23:59");
}

//----------------------------------------------
//
//      setPlageJournee
//
//----------------------------------------------
void setPlageJournee(plageHoraire *ptrPlage){
    ptrPlage->modele = PLAGE_MODELE_JOUNEE;
    strcpy(ptrPlage->nomPlage,"Journee");
    ptrPlage->heureDebut = 8;
    ptrPlage->minuteDebut = 0;
    ptrPlage->heureFin = 19;
    ptrPlage->minuteFin = 59;
    ptrPlage->chauffageOnOff = true;
    ptrPlage->consigne = consigneReferenceJour;
    ptrPlage->plageActive = true;
    //Serial.println("plage Journee selectionnee 8:00 => 20:00");
}

//----------------------------------------------
//
//      initCalendrier
//
//----------------------------------------------
void initCalendrier(void){
    Serial.println("======================");
    Serial.println("     Init Calendrier ");
    Serial.println("----------------------");
    Serial.println("initialisation des donnees calendrier");
    Serial.print ("la taille du calendrier est : ");
    Serial.println(sizeof(calendrier));

    if (0){
        handleChargeCalendrier();
    } else {
        for (int j = 0 ; j < NB_JOURS ; j++){
            for (int i = 0 ; i < NB_PLAGES_PAR_JOUR - 1; i++){
                calendrier.plagesHoraires[j][i].modele = -1;
                calendrier.plagesHoraires[j][i].heureDebut = -1;
                calendrier.plagesHoraires[j][i].minuteDebut = -1;
                calendrier.plagesHoraires[j][i].heureFin = -1;
                calendrier.plagesHoraires[j][i].minuteFin = -1;
                calendrier.plagesHoraires[j][i].chauffageOnOff = false;
                calendrier.plagesHoraires[j][i].consigne = -1;
                calendrier.plagesHoraires[j][i].plageActive = false;
            }
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].modele = 0;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].heureDebut = 0;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].minuteDebut = 0;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].heureFin = 23;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].minuteFin = 59;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].chauffageOnOff = false;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].consigne = consigneReferenceJour;
            calendrier.plagesHoraires[j][NB_PLAGES_PAR_JOUR - 1].plageActive = true;
        }
    }

    // plage modele NUIT
    setPlageNuit(&calendrier.plagesModeles[PLAGE_MODELE_NUIT]);
    // plage modele LEVER
    setPlageLever(&calendrier.plagesModeles[PLAGE_MODELE_LEVER]);
    // plage modele COUCHER
    setPlageCoucher(&calendrier.plagesModeles[PLAGE_MODELE_COUCHER]);
    // plage modele ALL
    setPlageAll(&calendrier.plagesModeles[PLAGE_MODELE_ALL]);
    // plage modele JOURNEE
    setPlageJournee(&calendrier.plagesModeles[PLAGE_MODELE_JOUNEE]);
    //Serial.print(listeDonneesCalendrier());
}

//----------------------------------------------
//
//      getSizeofCalendrier
//
//----------------------------------------------
int getSizeofCalendrier(void){
    return sizeof(calendrier);
}

//----------------------------------------------
//
//      getCalendrier
//
//----------------------------------------------
structCalendrier *getCalendrier(void){
    return &calendrier;
}

//----------------------------------------------
//
//      refreshcalendrier
//
//----------------------------------------------
void refreshCalendrier(){

}

//----------------------------------------------
//
//      getChauffageStatus
//
//----------------------------------------------
bool getChauffageStatus(void){
    int heureActuelle = getHour();
    int minuteActuelle = getMinute();
    int jourActuel = getDayOfWeek();
    chauffageCalendrier = false;
    int heureD, heureF, minuteD, minuteF;
    bool onOff[NB_PLAGES_PAR_JOUR], actif;


    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        heureD = calendrier.plagesHoraires[jourActuel][i].heureDebut;
        heureF = calendrier.plagesHoraires[jourActuel][i].heureFin;
        minuteD = calendrier.plagesHoraires[jourActuel][i].minuteDebut;
        minuteF = calendrier.plagesHoraires[jourActuel][i].minuteFin;
        onOff[i] = false;
        actif = calendrier.plagesHoraires[jourActuel][i].plageActive;

        if (actif){
            if ((heureActuelle >= heureD) && (minuteActuelle >= minuteD)){
                // on est au dela du début de la plage
                onOff[i] = true;
                if (heureActuelle > heureF + 1){
                    // fin de plage en debut d'heure
                    onOff[i] = false;
                } else if (heureF == heureActuelle){
                    if (minuteActuelle >= minuteF){
                        onOff[i] = false;
                    }
                }
            }
        }
    }

    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        chauffageCalendrier |= onOff[i]; 
    }
    return chauffageCalendrier;
}

//----------------------------------------------
//
//      handleChargeCalendrier
//
//----------------------------------------------
void handleChargeCalendrier(){
    String datas;
    Serial.print("recuperation du calendrier sauvegarde sur carte SD\n");
    datas = lireFichier("chaudiere/calendrier.txt");
    //Serial.println(datas);
    Serial.print("datas.length() = "); Serial.println(datas.length());
    String ligne = "";
    int numLigne = 0;
    int idx = 0;
    int itemJour, itemPlage, itemModele;
    char itemNomPlage[20];
    int itemHeureDeb, itemMinuteDeb;
    int itemHeureFin, itemMinuteFin, itemConsigne;
    bool itemChauffageOnOff, itemPlageActive;
    while(idx < datas.length()){
        while (datas[idx] != '\n'){
            ligne += datas[idx];
            idx++;
        }
        idx++;
        Serial.print(numLigne); Serial.print(" ligne = "); Serial.println(ligne);
        plageHoraire plage;
        int k = 0;
        int numItem = 0;
        String item = "";
        //Serial.println(ligne.substring(0,4));
        if (ligne.substring(0,4) != "jour"){
            while (k < ligne.length()){
                while ((ligne[k] != ';') && (k < ligne.length())){
                    //Serial.print(ligne[k]);
                    item += ligne[k++];
                }
                k++;
                Serial.print(numItem); Serial.print(" item = "); Serial.println(item);
                item = "";
                numItem++;
                switch(numItem){
                    case 0: // jour
                        itemJour = item.toInt();
                        break;
                    case 1: // num plage
                        itemPlage = item.toInt();
                        break;
                    case 2: // modele
                        itemModele = item.toInt();
                        break;
                    case 3: // nom
                        strcpy(itemNomPlage, item.c_str());
                        break;
                    case 4: // heure debut
                        itemHeureDeb = item.toInt();
                        break;
                    case 5: // minute debut
                        itemMinuteDeb = item.toInt();
                        break;
                    case 6: // heure fin
                        itemHeureFin = item.toInt();
                        break;
                    case 7: // minute fin
                        itemMinuteFin = item.toInt();
                        break;
                    case 8: // chauffage on off
                        if (item.toInt() == 0){
                            itemChauffageOnOff = false;
                        } else {
                            itemChauffageOnOff = true;
                        }
                        break;
                    case 9: // consigne
                        itemConsigne = item.toInt();
                        break;
                    case 10: // plage active
                        if (item.toInt() == 0){
                            itemPlageActive = false;
                        } else {
                            itemPlageActive = true;
                        }
                        break;
                }
            }
            setPlage(&calendrier.plagesHoraires[itemJour][itemPlage], 
                itemModele, itemNomPlage, itemHeureDeb, itemMinuteDeb, 
                itemHeureFin, itemMinuteFin, itemChauffageOnOff, 
                itemConsigne, itemPlageActive);

            if (numLigne >= 2) break;
        }
        ligne = "";
        numLigne++;
    }
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleSauveCalendrier
//
//----------------------------------------------
void handleSauveCalendrier() {
    String ligne;
    plageHoraire *ptrPlage;
    Serial.print("sauvegarde du calendrier \n");
    ligne = String("modele;nom;heureDeb;MinuteDeb;heureFin;minuteFin;chauffageOnOff;consigne;active\n");

    for (int i = 0 ; i < NB_JOURS ; i++){
        for (int j = 0 ; j < NB_PLAGES_PAR_JOUR ; j++){
            ptrPlage = &(calendrier.plagesHoraires[i][j]);
            ligne += i; 
            ligne += ";";
            ligne += j;
            ligne += ";";
            ligne += ptrPlage->modele;
            ligne += ";";
            ligne += String(ptrPlage->nomPlage);
            ligne += ";";
            ligne += ptrPlage->heureDebut;
            ligne += ";";
            ligne += ptrPlage->minuteDebut;
            ligne += ";";
            ligne += ptrPlage->heureFin;
            ligne += ";";
            ligne += ptrPlage->minuteFin;
            ligne += ";";
            ligne += ptrPlage->chauffageOnOff;
            ligne += ";";
            ligne += ptrPlage->consigne;
            ligne += ";";
            ligne += ptrPlage->plageActive;
            ligne += "\n";
        }
    }
    Serial.print(ligne);
    sauvegardeFichier("chaudiere/calendrier.txt", ligne);
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleDeletePlage
//
//----------------------------------------------
void handleDeletePlage() {
    Serial.print("suppression de la plage ");
    Serial.print(plageEnEdition);
    Serial.print(" du jour ");
    Serial.println(jourEnEdition);
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureDebut = -1;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteDebut = -1;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureFin = -1;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteFin = -1;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].chauffageOnOff = false;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].consigne = -1;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].plageActive = false;
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleCreatePlage
//
//----------------------------------------------
void handleCreatePlage() {
    char ligne[100];
    jourEnEdition = server.arg("jour").toInt();
    plageEnEdition = server.arg("plage").toInt();
    Serial.print("creation de la plage ");
    Serial.print(plageEnEdition);
    Serial.print(" du jour ");
    Serial.println(jourEnEdition);
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureDebut = 0;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteDebut = 0;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureFin = 0;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteFin = 0;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].chauffageOnOff = true;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].consigne = consigneReferenceJour;
    calendrier.plagesHoraires[jourEnEdition][plageEnEdition].plageActive = true;
    sprintf(ligne, "/editeHeurePlage?jour=%d&plage=%d", jourEnEdition, plageEnEdition);
    server.sendHeader("Location", String(ligne), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleSwitchPlageOnOff
//
//----------------------------------------------
void handleSwitchPlageOnOff() {
    int jour = server.arg("jour").toInt();
    int plage = server.arg("plage").toInt();
    Serial.print("mise a jour de la plage ");
    Serial.print(plage);
    Serial.print(" du jour ");
    Serial.println(jour);
    calendrier.plagesHoraires[jour][plage].plageActive = !calendrier.plagesHoraires[jour][plage].plageActive;
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleUpdatePlage
//
//----------------------------------------------
void handleUpdatePlage() {
    int heure, minute;
    char debutPlage[20], finPlage[20], tmp[20];
    char ligne[100];
    char newConsigne[10];
    Serial.println("---------------------------");
    Serial.println("    handleUpdatePlage");
    sprintf(ligne, "nb parametres = %d", server.args());Serial.println(ligne);
    String modele = server.arg("modele");
    sprintf(ligne, "modele selectionne = %s", modele.c_str());Serial.println(ligne);
    if (modele == "nuit"){
        setPlageNuit(&calendrier.plagesHoraires[jourEnEdition][plageEnEdition]);
    } else if (modele == "lever"){
        setPlageLever(&calendrier.plagesHoraires[jourEnEdition][plageEnEdition]);
    } else if (modele == "coucher"){
        setPlageCoucher(&calendrier.plagesHoraires[jourEnEdition][plageEnEdition]);
    } else if (modele == "24h"){
        setPlageAll(&calendrier.plagesHoraires[jourEnEdition][plageEnEdition]);
    } else if (modele == "journee"){
        setPlageJournee(&calendrier.plagesHoraires[jourEnEdition][plageEnEdition]);
    } else {
        strcpy(debutPlage, server.arg("heureDeb").c_str());
        strcpy(finPlage, server.arg("heureFin").c_str());
        sprintf(ligne, "nouvelles valeur : debut = %s, fin = %s", debutPlage, finPlage); Serial.println(ligne);
        // traitement de l'heure de debut
        strcpy(tmp, debutPlage);
        tmp[2]='\0';
        heure = atoi(tmp);
        strcpy(tmp, &debutPlage[3]);
        minute = atoi(tmp);
        calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureDebut = heure;
        calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteDebut = minute;
        sprintf(ligne, "mise a jour de la plage %d du jour %d", plageEnEdition, jourEnEdition); Serial.println(ligne);
        sprintf(ligne, "nouvelles heureDebut : heure = %d, fin = %d", heure, minute); Serial.println(ligne);
        // traitement de l'heure de fin
        strcpy(tmp, finPlage);
        tmp[2]='\0';
        heure = atoi(tmp);
        strcpy(tmp, &finPlage[3]);
        minute = atoi(tmp);
        calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureFin = heure;
        calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteFin = minute;
        sprintf(ligne, "nouvelles heureFin : heure = %d, fin = %d", heure, minute); Serial.println(ligne);
        // traitement de la consigne
        strcpy(newConsigne,server.arg("consigne").c_str());
        sprintf(ligne, "nouvelle consigne = %s", newConsigne); Serial.println(ligne);
        updateTemperature(&(calendrier.plagesHoraires[jourEnEdition][plageEnEdition].consigne), newConsigne);
    }
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleEditHeurePlage
//
//----------------------------------------------
void handleEditHeurePlage() {
    Serial.println("---------------------------");
    Serial.println("    handleEditHeurePlage");
    char ligne[100];
    jourEnEdition = server.arg("jour").toInt();
    plageEnEdition = server.arg("plage").toInt();
    //String heure = server.arg("heure");
    sprintf(ligne, "handleEditHeurePlage => mise a jour de l'horaire de %d de la plage %d", jourEnEdition, plageEnEdition); Serial.println(ligne);
    char ancienDebut[15], ancienFin[15];
    sprintf(ancienDebut, "%02d:%02d", calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureDebut, calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteDebut);
    sprintf(ancienFin, "%02d:%02d", calendrier.plagesHoraires[jourEnEdition][plageEnEdition].heureFin, calendrier.plagesHoraires[jourEnEdition][plageEnEdition].minuteFin);
    Serial.print("anciennes valeurs : debut = ");
    Serial.print(ancienDebut);
    Serial.print(", fin = ");
    Serial.println(ancienFin);
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
    page += "   <title> EDITION D'UN HORAIRE DE PLAGE </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "   <h1> EDITION D'UN HORAIRE DE PLAGE </h1>\n";
    page += "   <p> modification de la plage ";
    page +=     plageEnEdition;
    page +=     " du ";
    page +=     getDayString(jourEnEdition);
    page += "   </p>\n";
    //page +=     ligne;
    page += "       <form action='/updatePlage'";
    page += "       <div>";
    page += "           <table>";
    page += "               <thead>";
    page += "                   <tr>";
    page += "                       <th>  </th>";
    page += "                       <th> heure debut </th>";
    page += "                       <th> heure fin </th>";
    page += "                       <th> consigne </th>";
    page += "                       <th> modele </th>";
    page += "                   </tr>";
    page += "               </thead>";
    page += "               <tbody>";
    page += "                   <tr>";
    page += "                       <td> Nouvelle valeur </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureDeb' name='heureDeb' value='%s'>",ancienDebut);
    page +=                             ligne;
    //page += "                           <input type='time' id='heureDeb' name='heureDeb' >";
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureFin' name='heureFin' value='%s'>",ancienFin);
    page +=                             ligne;
    //page += "                           <input type='time' id='heureFin' name='heureFin' >";
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='text' id='consigne' name='consigne' value=%s>",getTemperatureSring(calendrier.plagesHoraires[jourEnEdition][plageEnEdition].consigne));
    page +=                             ligne;
    //page += "                           <input type='number' id='consigne' name='consigne' >";
    page += "                       </td>";
    page += "                       <td>";
    page += "                           <select name='modele'>";
    page += "                              <option valeur='libre' selected='selected'>libre</option>";
    page += "                               <option valeur='nuit'>nuit</option>";
    page += "                               <option valeur='lever'>lever</option>";
    page += "                               <option valeur='coucher'>coucher</option>";
    page += "                               <option valeur='all'>24h</option>";
    page += "                               <option valeur='journee'>journee</option>";
    page += "                           </select>";
    page += "                       </td>";
    page += "                   </tr>";
    page += "               </tbody>";
    page += "           </table>";
    page += "       </div>";
    page += "       <div>";
    page += "           <button>Valider</button>";
    page += "       </div>";
    page += "   </form>";


    page += "    <div class='w3-center w3-padding-16'>\n";
    page += "        <p><a href='/deletePlage'>  Supprimer la plage </a></p>\n";
    page += "    </div>\n";

    page += "    <div class='w3-center w3-padding-16'>\n";
    page += "        <p><a href='/calendrier'>  retour au calendrier</a></p>\n";
    page += "    </div>\n";
    page +=      piedDePage;
    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML
    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}

//----------------------------------------------
//
//      pageCalendrier
//
//----------------------------------------------
void pageCalendrier() {
    Serial.println("affichage page calendrier");
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
    page += "   <title> calendrier de chauffage </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";

    page += "<body>\n";
    page += "   <h1> calendrier de chauffage </h1>\n";

    page += "   <div>\n";
    page += "        <p>Nous sommes \n";
    page +=          getDayString(getDayOfWeek());
    page +=          ", il est ";
    page +=          getFormatedTime();
    page += "   <div>\n";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th >  </th>\n";
    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        page += "               <th colspan='4'> plage ";
        page +=                      i;
        page += "               </th>\n";
    }
    page += "               </tr>\n";
    page += "               <tr>\n";
    page += "                   <th>  </th>\n";
    for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
        page += "               <th> debut </th>\n";
        page += "               <th> fin </th>\n";
        page += "               <th> consigne </th>\n";
        page += "               <th> actif </th>\n";
    }
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    for (int j = 0 ; j < NB_JOURS ; j++){
        page += "           <tr>\n";
        page += "               <td> ";
        page +=                     getDayString(j);
        page += "               </td>\n";
        char heurePlage[20];
        char ligne[200];
        for (int i = 0 ; i < NB_PLAGES_PAR_JOUR ; i++){
            if (calendrier.plagesHoraires[j][i].heureDebut != -1){
                page += "       <td align='center'> ";
                sprintf(heurePlage,"%02d:%02d", calendrier.plagesHoraires[j][i].heureDebut, calendrier.plagesHoraires[j][i].minuteDebut);
                sprintf(ligne, "<a href='/editeHeurePlage?jour=%d&plage=%d&heure=debut'> %s </a>", j, i, heurePlage);
                page +=         ligne;
                page += "       </td>\n";
                page += "       <td align='center'> ";
                sprintf(heurePlage,"%02d:%02d", calendrier.plagesHoraires[j][i].heureFin, calendrier.plagesHoraires[j][i].minuteFin);
                sprintf(ligne, "<a href='/editeHeurePlage?jour=%d&plage=%d&heure=fin'> %s </a>", j, i, heurePlage);
                page +=         ligne;
                page += "       </td>\n";
                page += "       <td align='center'> ";
                sprintf(ligne, "<a href='/editeHeurePlage?jour=%d&plage=%d&heure=fin'> %s </a>", j, i, getTemperatureSring(calendrier.plagesHoraires[j][i].consigne));
                page +=         ligne;
                //page +=         getTemperatureSring(calendrier.plagesHoraires[j][i].consigne);
                page += "       </td>\n";
                page += "       <td align='center'> ";
                if (calendrier.plagesHoraires[j][i].plageActive){
                    sprintf(ligne, "<a href='/switchPlageOnOff?jour=%d&plage=%d'> oui </a>", j, i);
                    page +=     ligne;
                } else {
                    sprintf(ligne, "<a href='/switchPlageOnOff?jour=%d&plage=%d'> non </a>", j, i);
                    page +=     ligne;
                }
                page += "       </td>\n";
            } else {
                page += "       <td colspan='4' align='center'>";
                sprintf(ligne, "<a href='/createPlage?jour=%d&plage=%d'> inactive </a>", j, i);
                page +=         ligne;
                page += "       </td>";
            }
        }
        page += "           </tr>\n";
    }
    page += "           </tbody>\n";
    page += "       </table>\n";
    page += "    </div>\n";
    page += "    <div>\n";
    page += "       <p>Les plages predefinies sont les suivantes</p>\n";
    page += "       <table>\n";
    page += "           <thead>\n";
    page += "               <tr>\n";
    page += "                   <th>nom</th>\n";
    page += "                   <th>heure debut</th>\n";
    page += "                   <th>minute debut</th>\n";
    page += "                   <th>heure fin</th>\n";
    page += "                   <th>minute fin</th>\n";
    page += "                   <th>chauffage on/off</th>\n";
    page += "                   <th>consigne</th>\n";
    page += "                   <th>active</th>\n";
    page += "               </tr>\n";
    page += "           </thead>\n";
    page += "           <tbody>\n";
    plageHoraire *ptrModele;
    for (int i = 0 ; i < NB_PLAGES_MODELE ; i ++){
        ptrModele = &calendrier.plagesModeles[i];
        page += "           <tr>";
        page += "               <td align='center'>";
        page +=                     ptrModele->nomPlage;
        page += "               </td align='center'>";
        page += "               <td align='center'>";
        page +=                     ptrModele->heureDebut;
        page += "               </td>";
        page += "               <td align='center'>";
        page +=                     ptrModele->minuteDebut;
        page += "               </td>";
        page += "               <td align='center'>";
        page +=                     ptrModele->heureFin;
        page += "               </td>";
        page += "               <td align='center'>";
        page +=                     ptrModele->minuteFin;
        page += "               </td>";
        page += "               <td align='center'>";
        if (ptrModele->chauffageOnOff){
            page += "               ON";
        } else {
            page += "               OFF";
        }
        page += "               </td>";
        page += "               <td align='center'>";
        page +=                     ptrModele->consigne;
        page += "               </td>";
        page += "               <td align='center'>";
        if (ptrModele->plageActive){
            page += "               ON";
        } else {
            page += "               OFF";
        }
        page += "              </td>";
        page += "           </tr>";
    }
    page += "           </tbody>\n";
    page += "       </table>\n";
    page += "    </div>\n";
    page += "    <div class='w3-center w3-padding-16'>\n";
    if (chauffageCalendrier){
        page += "    <p>Le chauffage programmé est actuellement : ON</p>\n";
    } else {
        page += "    <p>Le chauffage programmé est actuellement : OFF</p>\n";
    }
    page += "        <p><a href='/'>  retour </a></p>\n";
    page += "        <p><a href='/sauveCalendrier'>  sauvegarde du calendrier </a></p>\n";
    page += "        <p><a href='/chargeCalendrier'>  recuperation du calendrier sauvegarde</a></p>\n";
    page += "    </div>\n";
    page +=      piedDePage;

    page += "</body>\n";
    page += "</html>\n";  // Fin de la page HTML

    server.setContentLength(page.length());  // Permet l'affichage plus rapide après chaque clic sur les boutons
    server.send(200, "text/html", page);
}


//----------------------------------------------
//
//      listeDonneesCalendrier
//
//----------------------------------------------
String listeDonneesCalendrier(void){
    char ligne[200];
    String page;
    page += "Données calendrier :\n";
    //page += "|012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789|\n";
    page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
    page += "|           |            plage 0           |            plage 0           |            plage 0           |            plage 0           |            plage 0           |\n";
    page += "|           +------------------------------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
    page += "|           | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons | debut |  fin  | actif | cons |\n";
    page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
    for (int j = 0 ; j < NB_JOURS ; j++){
        sprintf(ligne, "| %9s |", getDayString(j) );
        page += ligne;
        for (int p = 0 ; p < NB_PLAGES_PAR_JOUR ; p++){
            if (calendrier.plagesHoraires[j][p].heureDebut != -1){
                sprintf(ligne, " %02d:%02d |", calendrier.plagesHoraires[j][p].heureDebut, calendrier.plagesHoraires[j][p].minuteDebut);
                page += ligne;
                sprintf(ligne, " %02d:%02d |", calendrier.plagesHoraires[j][p].heureFin, calendrier.plagesHoraires[j][p].minuteFin);
                page += ligne;
                if (calendrier.plagesHoraires[j][p].plageActive){
                    page += "  OUI  |";
                } else {
                    page += "  NON  |";
                }
                sprintf(ligne, " %02.1f |", (float)calendrier.plagesHoraires[j][p].consigne / 10.0);
                page += ligne;
            } else {
                page += "           inactive           |";
            }

        }
        page += "\n";
    page += "+-----------+------------------------------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
    }
    //Serial.print(page);
    return page;
}
