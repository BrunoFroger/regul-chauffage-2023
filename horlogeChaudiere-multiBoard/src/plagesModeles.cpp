//----------------------------------------------
//
//      plagesModeles.cpp
//
//----------------------------------------------

#include <Arduino.h>

#include "calendrier.hpp"
#include "temperatures.hpp"
#include "wifiTools.hpp"

int indexToUpdate;

//----------------------------------------------
//
//      handlePlagesModeles
//
//----------------------------------------------
void handlePlagesModeles(void){
    Serial.println("---------------------------");
    Serial.println("    handlePlagesModeles");
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
    page += "   <title> EDITION DES PLAGES MODELES </title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "   <h1> EDITION DES PLAGES MODELES </h1>\n";
    page += "       <div>";
    page += "           <table>";
    page += "               <thead>";
    page += "                   <tr>";
    page += "                       <th> plage </th>";
    page += "                       <th> heure debut </th>";
    page += "                       <th> heure fin </th>";
    page += "                       <th> consigne </th>";
    page += "                       <th> active </th>";
    page += "                   </tr>";
    page += "               </thead>";
    page += "               <tbody>";
    for (int i = 0 ; i < NB_PLAGES_MODELE ; i++){
        sprintf(ligne, "affichage plage web %d\n", i);Serial.print(ligne);
        page += "                   <tr>";
        page += "                       <td>";
        sprintf(ligne, "<a href='/editPlageModele?id=%d'>%s</a>", i, calendrier.plagesModeles[i].nomPlage);
        page +=                             ligne;
        page += "                       </td>";
        page += "                       <td>";
        sprintf(ligne,"%02d:%02d",calendrier.plagesModeles[i].heureDebut, calendrier.plagesModeles[i].minuteDebut);
        page +=                             ligne;
        page += "                       </td>";
        page += "                       <td>";
        sprintf(ligne,"%02d:%02d",calendrier.plagesModeles[i].heureFin, calendrier.plagesModeles[i].minuteFin);
        page +=                             ligne;
        page += "                       </td>";
        page += "                       <td>";
        page +=                             getTemperatureSring(calendrier.plagesModeles[i].consigne);
        page += "                       </td>";
        page += "                       <td>";
        if (calendrier.plagesModeles[i].plageActive){
            page += "                        ON";
        } else {
            page += "                        OFF";
        }
        page += "                       </td>";
        page += "                   </tr>";
    }
    page += "               </tbody>";
    page += "           </table>";
    page += "       </div>";

    page += "    <div class='w3-center w3-padding-16'>\n";
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
//      handleUpdatePlageModele
//
//----------------------------------------------
void handleUpdatePlageModele(void) {
    Serial.println("---------------------------");
    Serial.println("    handleUpdatePlageModele");
    char ligne[100];
    String heure, minute;
    sprintf(ligne, "mise a jour des donnes de la plage %d => %s\n", indexToUpdate, calendrier.plagesModeles[indexToUpdate].nomPlage); Serial.print(ligne);
    String heureDebut = server.arg("heureDeb");
    calendrier.plagesModeles[indexToUpdate].heureDebut = heureDebut.substring(0,1).toInt();
    calendrier.plagesModeles[indexToUpdate].minuteDebut = heureDebut.substring(3,4).toInt();
    String heureFin = server.arg("heureFin");
    calendrier.plagesModeles[indexToUpdate].heureFin = heureFin.substring(0,1).toInt();
    calendrier.plagesModeles[indexToUpdate].minuteFin = heureFin.substring(3,4).toInt();
    String consigne = server.arg("consigne");
    sprintf(ligne, "nouvelles valeurs => heureDeb=%s, heureFin=%s, consigne=%s\n", heureDebut.c_str(), heureFin.c_str(), consigne.c_str()); Serial.print(ligne);
    // TODO update date debut et fin a verifier
    calendrier.plagesModeles[indexToUpdate].consigne = (int)(consigne.toFloat()*10);
    server.sendHeader("Location", String("/plagesModeles"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleUpdatePlageUser
//
//----------------------------------------------
void handleUpdatePlageUser(void) {
    Serial.println("---------------------------");
    Serial.println("    updatePlageUser");
    char ligne[100];
    String heure, minute;
    sprintf(ligne, "mise a jour des donnes de la plage %d => %s\n", indexToUpdate, calendrier.plagesUtilisateur[indexToUpdate].nomPlage); Serial.print(ligne);
    String heureDebut = server.arg("heureDeb");
    calendrier.plagesUtilisateur[indexToUpdate].heureDebut = heureDebut.substring(0,1).toInt();
    calendrier.plagesUtilisateur[indexToUpdate].minuteDebut = heureDebut.substring(3,4).toInt();
    String heureFin = server.arg("heureFin");
    calendrier.plagesUtilisateur[indexToUpdate].heureFin = heureFin.substring(0,1).toInt();
    calendrier.plagesUtilisateur[indexToUpdate].minuteFin = heureFin.substring(3,4).toInt();
    String consigne = server.arg("consigne");
    sprintf(ligne, "nouvelles valeurs => heureDeb=%s, heureFin=%s, consigne=%s\n", heureDebut.c_str(), heureFin.c_str(), consigne.c_str()); Serial.print(ligne);
    // TODO update date debut et fin a verifier
    calendrier.plagesUtilisateur[indexToUpdate].consigne = (int)(consigne.toFloat()*10);
    server.sendHeader("Location", String("/calendrier"), true);
    server.send ( 302, "text/plain", "");
}

//----------------------------------------------
//
//      handleEditPlageModele
//
//----------------------------------------------
void handleEditPlageModele(void) {
    Serial.println("---------------------------");
    Serial.println("    handleEditPlageModele");
    char ligne[100];
    indexToUpdate = server.arg("id").toInt();
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
    page += "   <title> EDITION D'UNE PLAGE MODELE</title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "   <h1> EDITION D'UNE PLAGE MODELE </h1>\n";
    page += "   <p> modification de la plage ";
    page +=     indexToUpdate;
    page +=     " => ";
    page +=     calendrier.plagesModeles[indexToUpdate].nomPlage;
    page += "   </p>\n";
    sprintf(ligne,"<form action='/updatePlageUser?id=%d'>", indexToUpdate);
    page +=         ligne;
    page += "       <div>";
    page += "           <table>";
    page += "               <thead>";
    page += "                   <tr>";
    page += "                       <th> heure debut </th>";
    page += "                       <th> heure fin </th>";
    page += "                       <th> consigne </th>";
    page += "                   </tr>";
    page += "               </thead>";
    page += "               <tbody>";
    page += "                   <tr>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureDeb' name='heureDeb' value='%02d:%02d'>",
            calendrier.plagesModeles[indexToUpdate].heureDebut, 
            calendrier.plagesModeles[indexToUpdate].minuteDebut);
    page +=                             ligne;
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureFin' name='heureFin' value='%02d:%02d'>",
            calendrier.plagesModeles[indexToUpdate].heureFin, 
            calendrier.plagesModeles[indexToUpdate].minuteFin);
    page +=                             ligne;
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='text' id='consigne' name='consigne' value=%s>",getTemperatureSring(calendrier.plagesModeles[indexToUpdate].consigne));
    page +=                             ligne;
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
//      handleEditPlageUser
//
//----------------------------------------------
void handleEditPlageUser(void) {
    Serial.println("---------------------------");
    Serial.println("    handleEditPlageUser");
    char ligne[100];
    indexToUpdate = server.arg("id").toInt();
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
    page += "   <title> EDITION D'UNE PLAGE USER</title>\n";
    page += "   <meta charset='UTF-8'/>\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "   <h1> EDITION D'UNE PLAGE USER </h1>\n";
    page += "   <p> modification de la plage ";
    page +=     indexToUpdate;
    page +=     " => ";
    page +=     calendrier.plagesModeles[indexToUpdate].nomPlage;
    page += "   </p>\n";
    sprintf(ligne,"<form action='/updatePlageUser?id=%d'>", indexToUpdate);
    page +=         ligne;
    page += "       <div>";
    page += "           <table>";
    page += "               <thead>";
    page += "                   <tr>";
    page += "                       <th> heure debut </th>";
    page += "                       <th> heure fin </th>";
    page += "                       <th> consigne </th>";
    page += "                   </tr>";
    page += "               </thead>";
    page += "               <tbody>";
    page += "                   <tr>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureDeb' name='heureDeb' value='%02d:%02d'>",
            calendrier.plagesModeles[indexToUpdate].heureDebut, 
            calendrier.plagesModeles[indexToUpdate].minuteDebut);
    page +=                             ligne;
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='time' id='heureFin' name='heureFin' value='%02d:%02d'>",
            calendrier.plagesModeles[indexToUpdate].heureFin, 
            calendrier.plagesModeles[indexToUpdate].minuteFin);
    page +=                             ligne;
    page += "                       </td>";
    page += "                       <td>";
    sprintf(ligne," <input type='text' id='consigne' name='consigne' value=%s>",getTemperatureSring(calendrier.plagesModeles[indexToUpdate].consigne));
    page +=                             ligne;
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
