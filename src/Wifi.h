#ifndef Wifi_h
#define Wifi_h
#include "Arduino.h"
#include <ESP8266WiFi.h>
class Wifi{
 private:
 String ssid = "<yourSSID>";
 String password = "<yourPassword>";
 WiFiClient wifiClient;
public:
 Wifi();


};


#endif
