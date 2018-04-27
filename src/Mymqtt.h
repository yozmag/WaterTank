#ifndef Mymqtt_h
#define Mymqtt_h

#include "Arduino.h"
#include "Setting.h"
//#include "Wifi.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
//https://developer.ibm.com/recipes/tutorials/run-an-esp8266arduino-as-a-iot-foundation-managed-device/
class Mymqtt{
private:
  PubSubClient client;
  Setting setting;
  WiFiClient espClient;
  String buildClientName();
  String publishTopic;
  String responseTopic;
  String manageTopic;
  String updateTopic;
  String rebootTopic;

  void handleUpdate(byte* payload);


public:
  Mymqtt();
  void connect();
  void reconnect();
  void publish(const String &topic,const String &msg);
  void subscribe(const String &topic);
  bool loop();
  void callback(char* topic, byte* payload, unsigned int length);
  void lowlevel();
  void highlevel();
  void botonpress(const boolean &actual_state);





};

#endif
