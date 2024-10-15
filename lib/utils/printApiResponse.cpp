#include <ArduinoJson.h>
#include <WifiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


#include "printApiResponse.h"

WiFiClientSecure client;

String url = "https://esp-next-testing.vercel.app/api/test";

// Printing a generic api response
void printApiResponse() {
  if (WiFi.status() == WL_CONNECTED) {
    client.setInsecure();
    HTTPClient http;
    http.begin(client, url); 
    http.setTimeout(1500);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();

      // Parsing JSON
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        const char* value = doc["message"].as<const char*>();
        Serial.println(value);
      } else {
        Serial.print("JSON Parsing Error: ");
        Serial.println(error.c_str());
      }
    } 
    else {
      Serial.print("GET Request failed, error: ");
      Serial.println(http.errorToString(httpCode));
    }
    http.end();
  }
  else {
    Serial.println("WiFi not connected");
  }
}

// Posting
void postToApi() {
  if (WiFi.status() == WL_CONNECTED) {
    client.setInsecure();
    HTTPClient http;
    http.begin(client, url); 
    http.addHeader("Content-Type", "application/json");

    // Create JSON object
    JsonDocument doc;
    doc["message"] = "success";

    String requestBody;
    serializeJson(doc, requestBody);

    // Specifying content length
    int contentLength = requestBody.length();
    http.addHeader("Content-Length", String(contentLength));

    Serial.println(requestBody);


    // Send post request
    int httpCode = http.POST(requestBody);

    // Errors will be negative
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("POST Response:");
      Serial.println(payload);

      // Parsing JSON
      JsonDocument responseDoc;
      DeserializationError error = deserializeJson(responseDoc, payload);

      if (!error) {
        const char* responseValue = responseDoc["message"].as<const char*>();
        Serial.print("Parsed Response: ");
        Serial.println(responseValue);
      } 
      else {
        Serial.print("JSON Parsing Error: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("POST Request failed, error: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
  } 
  else {
    Serial.println("WiFi not connected");
  }
}
