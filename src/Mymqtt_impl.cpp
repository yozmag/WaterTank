#include "Arduino.h"
#include "Mymqtt.h"
#include "Wifi.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


Mymqtt::Mymqtt()
{
  this->setting.load();
  WiFiClient espClient;
  //PubSubClient client(espClient);
  this->client.setClient(espClient);
  int str_len = setting.getMqttserver().length() + 1;
  char char_array[str_len];
  setting.getMqttserver().toCharArray(char_array, str_len);
  //this->client.setServer(server, 1883);
  this->client.setServer((char*) setting.getMqttserver().c_str(), setting.getMqttport());
  this->client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
  this->publishTopic = "iot-2/evt/status/fmt/json";
  this->responseTopic = "iotdm-1/response";
  this->manageTopic = "iotdevice-1/mgmt/manage";
  this->updateTopic = "iotdm-1/device/update";
  this->rebootTopic = "iotdm-1/mgmt/initiate/device/reboot";

}
void Mymqtt::connect(){
 const char* ssid = this->setting.getSsid().c_str();
 const char* password = this->setting.getPassword().c_str();

 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 }
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 uint8_t mac[6];
 WiFi.macAddress(mac);
 this->client.connect((char*) this->buildClientName().c_str(),(char*)  this->setting.getMqttuser().c_str(), (char*) this->setting.getMqttpwd().c_str());
//  this->client.connect(clientStr, "use-token-auth", token))
 if(client.connected()) {
   this->publish(this->publishTopic, this->buildClientName());
   // ... and resubscribe
   this->subscribe(this->manageTopic);
   this->subscribe(this->responseTopic);
   this->subscribe(this->updateTopic);
   this->subscribe(this->rebootTopic);
 Serial.println("Success getting online...Begin transmit...");
 }
//const char* server = setting.getMqttserver().c_str();
//setting.getMqttserver().toCharArray(server[], 64);
//char server[64] = ".messaging.internetofthings.ibmcloud.com";
//char topic[] = "iot-2/evt/status/fmt/json";


}

void Mymqtt::reconnect(){
  // Loop until we're reconnected
  while (!this->client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (this->client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...

      this->publish(this->publishTopic, this->buildClientName());
      // ... and resubscribe
      this->subscribe(this->manageTopic);
      this->subscribe(this->responseTopic);
      this->subscribe(this->updateTopic);
      this->subscribe(this->rebootTopic);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
}
}
void Mymqtt::publish(const String &topic,const String &msg){

 this->client.publish( (char*) topic.c_str(), (char*) msg.c_str());


}

void Mymqtt::subscribe(const String &topic){
 this->client.subscribe((char*) topic.c_str());


}
bool Mymqtt::loop(){

  return this->client.loop();
}

void Mymqtt::callback(char* topic, byte* payload, unsigned int length){

  if (strcmp (this->responseTopic.c_str(), topic) == 0) {
     return; // just print of response for now
   }

   if (strcmp (this->manageTopic.c_str(), topic) == 0) {
     Serial.println("Manage...");

   }
   if (strcmp (this->rebootTopic.c_str(), topic) == 0) {
     Serial.println("Rebooting...");
     ESP.restart();
   }

   if (strcmp (this->updateTopic.c_str(), topic) == 0) {
     this->handleUpdate(payload);
   }
  }


void Mymqtt::handleUpdate(byte* payload) {
  int publishInterval = 30000;
 StaticJsonBuffer<300> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject((char*)payload);
 if (!root.success()) {
   Serial.println("handleUpdate: payload parse FAILED");
   return;
 }
 Serial.println("handleUpdate payload:"); root.prettyPrintTo(Serial); Serial.println();

 JsonObject& d = root["d"];
 JsonArray& fields = d["fields"];
 for (JsonArray::iterator it = fields.begin(); it != fields.end(); ++it) {
   JsonObject& field = *it;
   const char* fieldName = field["field"];
   if (strcmp (fieldName, "metadata") == 0) {
     JsonObject& fieldValue = field["value"];
     if (fieldValue.containsKey("publishInterval")) {
       publishInterval = fieldValue["publishInterval"];
       Serial.print("publishInterval:"); Serial.println(publishInterval);
     }
   }
 }
}
void Mymqtt::lowlevel(){
  const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();
  //root["sensor"] = "gpsxzvvzxvzxcvzxcvzvxzczxcvzcxvzcxvzxcvzxv";
  //root["time"] = 1351824120;

  JsonObject& data = root.createNestedObject("d");
  data["sensor"] = "level";
  data["value"] = "low";
  //data.add(48.75608);
  //data.add(2.302038);

  root.printTo(Serial);
  String output;
  root.printTo(output);

  this->publish(this->updateTopic, output);
}
void Mymqtt::highlevel(){
  const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();
  //root["sensor"] = "gpsxzvvzxvzxcvzxcvzvxzczxcvzcxvzcxvzxcvzxv";
  //root["time"] = 1351824120;

  JsonObject& data = root.createNestedObject("d");
  data["sensor"] = "level";
  data["value"] = "high";
  //data.add(48.75608);
  //data.add(2.302038);

  root.printTo(Serial);
  String output;
  root.printTo(output);

  this->publish(this->updateTopic, output);
}
void Mymqtt::botonpress(const boolean &actual_state){
  String state = actual_state?"high":"low";
  const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();
  //root["sensor"] = "gpsxzvvzxvzxcvzxcvzvxzczxcvzcxvzcxvzxcvzxv";
  //root["time"] = 1351824120;

  JsonObject& data = root.createNestedObject("d");
  data["sensor"] = "botton";
  data["value"] = state;
  //data.add(48.75608);
  //data.add(2.302038);

  root.printTo(Serial);
  String output;
  root.printTo(output);

  this->publish(this->updateTopic, output);
}


String Mymqtt::buildClientName(){
  String data = "";
  data+="d:";
  data+=this->setting.getIbmorgid();
  data+=":";
  data+=this->setting.getDevicetype();
  data+=":";
  data+=this->setting.getDeviceid();
  return data;
}
