//----------------------------------------------
//
//      wifiTools.hpp
//
//----------------------------------------------
#include <Arduino.h>

#ifndef __WIFITOOLS__
#define __WIFITOOLS__

#ifdef lolin_s2_mini
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    extern ESP8266WebServer server;
#elif wemos_d1_mini32 
    #include <WiFi.h>
    #include <WebServer.h>
    extern WebServer server;
#elif adafruit_feather_m0 
    #include <WiFi.h>
    #include <WebServer.h>
    extern WebServer server;
#else 
    #error Unsupported board selection.
#endif

    extern bool getChauffageOnOff(void);
    extern bool initWifi(void);
    extern void handleWebRequete(void);
    extern String piedDePage;
    extern String enteteDePage;
    extern String webPageStyle;
    extern String listeDonneesChauffage(void);
    extern char *getIpAddress(void);
    extern char *getWifiSsid(void);
    extern void setWifiParameters(char *ssid, char *pwd, char *mode);
    extern bool isWifiConnected(void);
    extern bool isApMode(void);
    extern int getRSSI(void);
    
#endif
