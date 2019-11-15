/** @file main.c
 */

 #include <Arduino.h>

 #include <ESP8266WiFi.h>
 #include <ESP8266HTTPClient.h>
 #include <WiFiClientSecureBearSSL.h>
 #include "secrets.h"

 const char* ssid     = SECRET_SSID;
 const char* password = SECRET_PASSWORD;
 const char* url      = SECRET_WEBHOOK;


 String testMessage = "Discord test";
 
void setup()
{
  Serial.begin(115200);

  //Connect to a WiFi network

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(SECRET_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Setup HTTPS client and connect to host

  HTTPClient https;
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);

  // Check connection to server was successful
  if (https.begin(*client, url))
  {
    https.addHeader("Content-Type", "application/json");
    int httpsCode = https.POST("{\"content\":\"" + testMessage + "\"}");
  
    Serial.print("HTTPS Code: ");
    Serial.println(httpsCode);
  
    https.end(); 
  }
  else
  {
    Serial.println("[HTTPS] Unable to connect");
    return;
  }  
}

void loop()
{
  // put your main code here, to run repeatedly:

}
