//----------------------------------------------
//
//      wifiTools.hpp
//
//----------------------------------------------
#include <Arduino.h>

#ifndef __WIFITOOLS__
#define __WIFITOOLS__

    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>

    extern ESP8266WebServer server;

    extern void initWifi(char *ssid, char *pwd);
    extern void refreshWifi(void);
    extern char *getIpAdresse(void);
    
#endif
