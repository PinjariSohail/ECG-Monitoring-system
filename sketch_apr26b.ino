#include <WiFi.h>
#include "MAX30100_PulseOximeter.h"
#include "ThingSpeak.h"

WiFiClient client;

#define SENSOR  A0
//PulseOximeter pox;

const char* WIFI_NAME = "vivo 1919";
const char* WIFI_PASSWORD = "123456789";
const int myChannelNumber =2524681 ;
const char* myApiKey = "SBA8BKGARCHXFQEY";
const char* server = "api.thingspeak.com";

void onBeatDetected()
{
  Serial.println("Beat...!");
}

void setup()
{
  Serial.begin(115200);
  pinMode(SENSOR,INPUT);
  //Serial.println("initialization pulse oximeter...");

  /*if(!pox.begin())
  {
    Serial.println("failed")
  }
  else
  {
    Serial.println("Success");
  }
  */
  WiFi.begin(WIFI_NAME,WIFI_PASSWORD);
  while (WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.println("WIFI not connected");
  }
  Serial.println("WIFI connected");
  Serial.println("local IP: "+String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  int sensor = analogRead(SENSOR);
  ThingSpeak.setField(1,sensor);

  Serial.println(sensor);
  int x = ThingSpeak.writeFields(myChannelNumber,myApiKey);

  //int rangelimit = 5000;


  if(x == 50)
  {
    Serial.println("Data pushed successfull");
  }
  else
  {
    Serial.println("puss error" + String(x));
  }
  Serial.println(".");
  delay(500);
}