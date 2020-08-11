/*
 * Code by Athulkrishna.s
 * #404 Error
 * @Atkerror
 * Credits and doubts at Athulkrishna2711@gmail.com
 */



#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

char auth[] = "oHjs7aSQTJHZR4evyCMSRABAfohMSAi2";
char ssid[] = "hlo12";
char pass[] = "1231231234";
int accuracy = 1;       //higher the number the voltage will be stable lower values will fluctuate
int out = D6;           // the PWM pin the out is attached to
int Vmaxinput = 27.602;


int maxpwm = 900; // can be set via app
int pinValuea = 5 ; // can be set via app
int voltage = 0; // can be set via app
int power = 0; // can be set via app
int stable = 0;

BlynkTimer timer;
void tt() {
  Blynk.virtualWrite(V10, voltage);
  Blynk.virtualWrite(V11, power);
}
void ttt() {
  int sensorValue = analogRead(A0);
  voltage = sensorValue * (Vmaxinput / 1023.0);
  if (voltage > pinValuea) {
    if (voltage - pinValuea >= accuracy || stable == 1) {
      if (power > 0) {
        power = power - 4;
      }
    }
  }
  if (voltage < pinValuea) {
    if (pinValuea - voltage >= accuracy || stable == 1) {
      if (power < maxpwm) {
        power = power + 1;
      }
    }
  }
  analogWrite(out, power);
}
void setup()
{
  timer.setInterval(1, ttt);
  timer.setInterval(1000, tt);
  pinMode(out, OUTPUT);
  analogWriteFreq(100000);
  analogWrite(out, power);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch"; 
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  
}
BLYNK_WRITE(V1)
{
  pinValuea = param.asInt(); // voltageeee
}

BLYNK_WRITE(V3)
{
  stable = param.asInt(); // voltageeee
}
BLYNK_WRITE(V2)
{
  maxpwm = param.asInt();// assigning incoming value from pin V1 to a variable
}
void loop()
{
  timer.run();
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
    ArduinoOTA.handle();

  }
}
