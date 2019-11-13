/** @file main.c
 */

 #include <ESP8266WiFi.h>
 #include "secrets.h"

 const char* ssid     = SECRET_SSID;
 const char* password = SECRET_PASSWORD;
 
 
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
}

void loop()
{
  // put your main code here, to run repeatedly:

}
