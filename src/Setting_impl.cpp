#include "Setting.h"
#include "Arduino.h"


Setting::Setting(){
this->ibmcloud = ".messaging.internetofthings.ibmcloud.com";
this->mqtt_server = this->ibmorgid + this->ibmcloud;
this->ibmclientid = "d:"+  this->ibmorgid +":"+ this->device_type+ ":"+ this->device_id;
this->publishinterval = 60000;
}
String Setting::getMqttserver(){
  return this->mqtt_server;
}
int Setting::getMqttport(){
  return this->mqtt_port;
}
String Setting::getMqttuser(){
  return this->mqtt_user;
}
String Setting::getMqttpwd(){
  return this->mqtt_pwd;
}
String Setting::getIbmclientid(){
  return this->ibmclientid;
}
String Setting::getIbmorgid(){
  return this->ibmorgid;
}
String Setting::getDevicetype(){
  return this->device_type;
}
String Setting::getDeviceid(){
  return this->device_id;
}
String Setting::getSsid(){
  return this->ssid;
}
String Setting::getPassword(){
  return this->password;
}

void Setting::load(){

  File jsonFile;
  String jsonData = "";
  String filePath = "/MyFile.json";
  //if (SPIFFS.begin()) {
  Serial.print("reading " + filePath + "...");
  if (SPIFFS.exists(filePath)) {
      jsonFile = SPIFFS.open(filePath, "r");
      if (jsonFile) {
        Serial.println("OK");
        size_t size;
        size = jsonFile.size();
        std::unique_ptr<char[]> buf(new char[size]);

        jsonFile.readBytes(buf.get(), size);
        int type = 1;
        if (type == 0){
        //Array
          DynamicJsonBuffer jsonBufferArray;
          JsonArray& jsonArray = jsonBufferArray.parseArray(jsonFile);
          //JsonArray& jsonArray = jsonBufferArray.parseArray(buf.get());
          if (jsonArray.success()) {
            char buffer[size+1];
            jsonArray.printTo(buffer, sizeof(buffer));
            jsonData = String(buffer);
          }
        }else{
        //Object
          DynamicJsonBuffer jsonBuffer;
          JsonObject& json = jsonBuffer.parseObject(buf.get());
          if (json.success()) {
            json["mqtt_server"].printTo(this->mqtt_server);
            this->mqtt_port = json["mqtt_port"];
            json["mqtt_user"].printTo(this->mqtt_user);
            json["mqtt_pwd"].printTo(this->mqtt_pwd) ;
            json["ibmcloud"].printTo(this->ibmcloud);
            json["ibmorgid"].printTo(this->ibmorgid);
            json["ibmclientid"].printTo(this->ibmclientid) ;
            json["device_type"].printTo(this->device_type) ;
            json["device_id"].printTo(this->device_id);
            json["token"].printTo(this->token);
            json["ssid"].printTo(this->ssid);
            json["password"].printTo(this->password) ;
            this->publishinterval = json["publishinterval"];


            /*
            char buffer[size];
            json.printTo(buffer, sizeof(buffer));
            jsonData = String(buffer);*/
          }
        }
      }
      jsonFile.close();
    }

  //}

  Serial.print("Read " + jsonData + " fim");
}
void Setting::save(){
  DynamicJsonBuffer jsonBuffer;
  File jsonFile;
  String jsonData = "";
  String filePath = "/MyFile.json";
  JsonObject &root = jsonBuffer.createObject();

  root["mqtt_server"] = this->mqtt_server;
  root["mqtt_port"] = this->mqtt_port;
  root["mqtt_user"] = this->mqtt_user;
  root["mqtt_pwd"] = this->mqtt_pwd;
  root["ibmcloud"] = this->ibmcloud;
  root["ibmorgid"] = this->ibmorgid;
  root["ibmclientid"] = this->ibmclientid;
  root["device_type"] = this->device_type;
  root["device_id"] = this->device_id;
  root["token"] = this->token;
  root["ssid"] = this->ssid;
  root["password"] = this->password;
  root["publishinterval"] = this->publishinterval;
  int publishinterval = 0;
  //if (SPIFFS.begin()) {
  Serial.print("reading " + filePath + "...");
  if (SPIFFS.exists(filePath)) {
      jsonFile = SPIFFS.open(filePath, "w");
      if (root.printTo(jsonFile) == 0) {
          Serial.println(F("Failed to write to file"));
        }

        // Close the file (File's destructor doesn't close the file)
      jsonFile.close();

    }



}
