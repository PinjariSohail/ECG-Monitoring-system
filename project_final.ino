#include <Wire.h>

#include <CircularBuffer.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>
#include <MAX30100.h>
#include "MAX30100_PulseOximeter.h"

#define SENSOR A0

#define REPORTING_PERIOD_MS 1000

#define bufferLen 64
int16_t sBuffer[bufferLen];

char payload[100];
char topic[150];
char str_sensor[10];

unsigned long lastConnectionTime = 0;
const unsigned long postingInyerval = 1L*1000L;

void callback(char*topic, byte*payload,unsigned int length)
{
  char p[length=1];
  memcpy(p,payload,length);
  Serial.write(payload,length);
  Serial.print(topic);
}

float HeartRate,SpO2;

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat !");
}

void setup()
{
  Serial.begin(115200);
  pox.begin();
  pinMode(SENSOR,INPUT);

  if(!pox.begin())
  {
    Serial.println(" ");
    Serial.println("failed to connect SpO2");
  }
  else
  {
    Serial.println("connected to SpO2");
    pox.setOnBeatDetectedCallback(onBeatDetected);
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  }
}

void loop()
{
  pox.update();

  float sensor = analogRead(SENSOR);
  Serial.println(sensor);
  

  HeartRate = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  if(millis()-tsLastReport >REPORTING_PERIOD_MS)
  {
    Serial.println(" ");
    Serial.print("BPM: ");
    Serial.println(HeartRate*7);
    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");
    Serial.println(" ");

    tsLastReport = millis();
  }

  int rangelimit = 5000;
  Serial.print(rangelimit);
  //Serial.print(" ");
  //Serial.print(rangelimit);
  Serial.print(" ");

  if(SENSOR == ESP_OK)
  {
    int16_t sample_read = 0/8;
    if(sample_read >0)
    {
      float mean = 0;
      for(int16_t i=0;i<sample_read;i++)
      {
        mean +=(sBuffer[i]); 
      }
      mean/=sample_read;
      Serial.println(mean);
    }
  }
  delay(200);
}