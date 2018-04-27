#ifndef Setting_h
#define Setting_h

#include <FS.h>    //this needs to be first, or it all crashes and burns...
#include "Arduino.h"

 #include <ArduinoJson.h>

class Setting{
private:
String mqtt_server ="";
int mqtt_port = 1883;
String mqtt_user = "";
String mqtt_pwd = "";
String ibmcloud = "";
String ibmorgid = "";
String ibmclientid = "";
String device_type = "ESP8266";
String device_id = "Test1";
String token = "<yourToken>";
String ssid = "<yourSSID>";
String password = "<yourPassword>";
int publishinterval;


public:
 Setting();
 void load();
 void save();
 void setMqttserver(String &server);
 void setMqttport(int &port);
 void setMqttuser(String &user);
 void setMqttpwd(String &pwd);
 void setIbmcloud(String &ibmcloud);
 void setIbmorgid(String &ibmorgid);

 String getMqttserver();
 int getMqttport();
 String getMqttuser();
 String getMqttpwd();
 String getIbmclientid();
 String getIbmorgid();
 String getDevicetype();
 String getDeviceid();
 String getSsid();
 String getPassword();

};
#endif
