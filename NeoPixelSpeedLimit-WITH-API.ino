#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(14, 15, NEO_GRB + NEO_KHZ800);

int speedLimitPossibilities[] = {25, 35, 45};
String streetNames[] = {"S. Main St", "E. Stadium St", "Jackson Rd"};
int speedLimit;
long currentSpeed;
long speedIndex;
const char* ssid = "MGuest";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  WiFi.begin(ssid);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  randomSeed(analogRead(0));
  leds.begin();
  leds.show();
}

void loop() 
{
  int index;
  speedIndex = random(0, 3);
  speedLimit = speedLimitPossibilities[speedIndex];
  Serial.println("");
  Serial.println(recognizeStreetName());
  Serial.println(recognizeSpeedLimit());
  Serial.println("***************************");
  currentSpeed = random((speedLimit - 3), (speedLimit + 8));
  for(index = 0; index < 14; index++)
  {
    leds.setPixelColor(index, 0, 255, 0);
    leds.setBrightness(10);
  }
  leds.show();
  
  speedStatus(currentSpeed);
  for(index = 0; index < 11;  index++)
  {
    while(currentSpeed < (speedLimit + 12))
    {
      accelerate();
      speedStatus(currentSpeed);
    }
  }
  for(index = 11; index > -1; index--)
  {
    decelerate();
    speedStatus(currentSpeed);
  }
}

String recognizeStreetName()
{
  String streetName = " ";
  
  if(speedLimit == 25)
  {
    streetName = "CURRENT STREET: " + streetNames[0];
  }
  else if(speedLimit == 35)
  {
    streetName = "CURRENT STREET: " + streetNames[1];
  }
  else if(speedLimit == 45)
  {
    streetName = "CURRENT STREET: " + streetNames[2];
  }
  else
  {
    streetName = "CURRENT STREET: UNKNOWN";
  }
  return streetName;
}

String recognizeSpeedLimit()
{
  String speedRecognized = " ";
  
  if(speedLimit == 25)
  {
    speedRecognized = "SPEED LIMIT: 25 MPH";
  }
  else if(speedLimit == 35)
  {
    speedRecognized = "SPEED LIMIT: 35 MPH";
  }
  else if(speedLimit == 45)
  {
    speedRecognized = "SPEED LIMIT: 45 MPH";
  }
  else
  {
    speedRecognized = "SPEED LIMIT: UNKNOWN";
  }
  return speedRecognized;
}


void setBlinkingRed()
{ 
 int index;
 int loopIndex;
 for(loopIndex = 0; loopIndex < 3; loopIndex++)
 {
   for(index = 0; index < 14; index++)
   {
    leds.setPixelColor(index, 255, 0, 0);
    leds.show();
   }
   delay(250);
   for(index = 0; index < 14; index++)
   {
    leds.setPixelColor(index, 0, 0, 0);
    leds.show();
   }
  delay(250);
   for(index = 0; index < 14; index++)
   {
    leds.setPixelColor(index, 255, 0, 0);
    leds.show();
   }
  leds.setBrightness(10);
   }
}

void setIncrementingGreen()
{
  while(leds.getPixelColor(0) != 59392)
  {
    int index;
    for(index = 0; index < 14; index++)
    {
      leds.setPixelColor(index, 0, 255, 0);
      leds.show();
      delay(100);
    }
    leds.setBrightness(10);
  } 
}

void setIncrementingYellow()
{
  while(leds.getPixelColor(0) != 9139200)
  {
    int index;
    for(index = 0; index < 14; index++)
    {
      leds.setPixelColor(index, 150, 125, 0);
      leds.show();
      delay(100);
    }
    leds.setBrightness(10);
  }
}

void setIncrementingRed()
{
  while(leds.getPixelColor(0) != 15204352)
  {
    int index;
    for(index = 0; index < 14; index++)
    {
      leds.setPixelColor(index, 255, 0, 0);
      leds.show();
      delay(100);
    }
    leds.setBrightness(10);
  }
}

int accelerate()
{
  int speedComparison;
  String speedMessage = "CURRENT SPEED: ";
  String speedMessage2 = " ";
  currentSpeed += 1;
  speedComparison = currentSpeed - speedLimit;
  delay(1000);
  Serial.print(speedMessage);
  Serial.print(currentSpeed);
  Serial.print("\t");
  Serial.print(speedComparison);
  if(speedComparison < 0)
  {
    speedMessage2 = " MPH under the speed limit";
    Serial.println(speedMessage2);
  }
  else
  {
    speedMessage2 = " MPH over the speed limit";
    Serial.println(speedMessage2);
  }
  return currentSpeed;
}

int decelerate()
{
  int speedComparison;
  String speedMessage = "CURRENT SPEED: ";
  String speedMessage2 = " ";
  currentSpeed -= 1;
  speedComparison = currentSpeed - speedLimit;
  delay(1000);
  Serial.print(speedMessage);
  Serial.print(currentSpeed);
  Serial.print("\t");
  Serial.print(speedComparison);
  if(speedComparison < 0)
  {
    speedMessage2 = " MPH under the speed limit";
    Serial.println(speedMessage2);
  }
  else
  {
    speedMessage2 = " MPH over the speed limit";
    Serial.println(speedMessage2);
  }
  return currentSpeed;
}

void speedStatus(int currentSpeed)
{
  if(currentSpeed <= (speedLimit + 3))
  {
    setIncrementingGreen();
  }
  else if(currentSpeed <= (speedLimit + 6) && (currentSpeed >= (speedLimit + 4)))
  {
    setIncrementingYellow();
  }
 else if(currentSpeed <= (speedLimit + 10) && (currentSpeed >= (speedLimit + 7)))
 {
    setIncrementingRed();
 }
 else
 {
    setBlinkingRed();
 }
}

