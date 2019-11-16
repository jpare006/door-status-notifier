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

 String testMessageOne = "green button has been pressed";
 String testMessageTwo = "red button has been pressed";

 int GreenBtn = D1;
 int RedBtn   = D2;

 int buttonPressed;
 
void setup()
{
  Serial.begin(115200);
  
  pinMode(GreenBtn, INPUT);
  pinMode(RedBtn, INPUT);
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
}

void loop()
{
  if(digitalRead(GreenBtn) == HIGH)
  {
    buttonPressed = 1;
  }
  else if(digitalRead(RedBtn) == HIGH)
  {
    buttonPressed = 2;
  }
  
  HTTPClient https;
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);

  // Check connection to server was successful
  if (https.begin(*client, url))
  {
    https.addHeader("Content-Type", "application/json");

    int httpsCode;
    switch(buttonPressed)
    {
      case 1:
        httpsCode = https.POST("{\"content\":\"" + testMessageOne + "\"}");
        Serial.print("HTTPS Code: ");
        Serial.println(httpsCode);
        delay(1000);
        break;
      case 2:
        httpsCode = https.POST("{\"content\":\"" + testMessageTwo + "\"}");
        Serial.print("HTTPS Code: ");
        Serial.println(httpsCode);
        delay(1000);
        break;
    }
    https.end(); 
  }
  else
  {
    Serial.println("[HTTPS] Unable to connect");
  }

  
  
  delay(100);

}
