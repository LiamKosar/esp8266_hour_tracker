#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "connectToWifi.h"

void connectToWifi(std::string ssid, std::string password) {
  Serial.println("Connecting to wifi...");
  WiFi.begin(ssid.c_str(), password.c_str());

  while(WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(500);   
  }
  
  Serial.println("");
  Serial.println("Wifi Connected!");
  Serial.println("Ip Address: ");
  IPAddress ip =  WiFi.localIP();
  Serial.println(ip);
}