#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "secrets.h"

// ================= global variables =================
const char* url = SECRET_WEBHOOK;

String messageClosed = "Door is closed";
String messageOpen   = "Door is OPEN!";
ESP8266WiFiMulti WiFiMulti;
HTTPClient https;
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
char data;
int code;
// ================ end global variables ===============

int discord_send(char option);

void setup() {
  Serial.begin(9600);

  client->setFingerprint(fingerprint);

  //connect to wifi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SECRET_SSID, SECRET_PASSWORD);
}

void loop() {
  // as long as connected to wifi
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    if (Serial.available())
    {

      while(!https.begin(*client, url));
      
      data = Serial.read();

      do
      {
        code = discord_send(data);
      } while(code < 0); //if code is negative that means error occurred and must retry
      https.end();
    }
  }
}

int discord_send(char option)
{
  String message;
  
  if(option == 'o')
  {
    message = messageOpen;
  }
  else if(option == 'c')
  {
    message = messageClosed;
  }
  else
  {
    message = "Undefined";
  }

  https.addHeader("Content-Type", "application/json");
  int httpsCode = https.POST("{\"content\":\"" + message + "\"}");

  return httpsCode;
}
