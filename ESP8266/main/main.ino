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
      discord_send(data);
      https.end();
    }
  }
}

int discord_send(char option)
{
  String message;
  bool done = false;
  
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

  while(!done)
  {
    https.addHeader("Content-Type", "application/json");
    int httpsCode = https.POST("{\"content\":\"" + message + "\"}");

    if(httpsCode > 0)
    {
      done = true;
    }
    else
    {
      done = false;
    }
  }
}
