/* A little LED night light that comes on when the sun goes down. 
Turns off at bedtime using DS1307 RTC. Light sensor used is TSL2561 */


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <FastLED.h>
#include "RTClib.h"


#define NUM_LEDS 4
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

RTC_Millis rtc;

void configureSensor(void)
{
  //auto gain for the light sensor
  tsl.enableAutoRange(true);            
  // set the sensor grain
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      
}



void setup() {
  Serial.begin(9600);
 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // Initialise the sensor 
  if(!tsl.begin())
  {
    // There was a problem detecting the TSL2561 ... check your connections 
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  
  // We're ready to go! 
  Serial.println("good to go");
}

void loop() {
    // put your main code here, to run repeatedly:
    //TO DO refactor for finite state machine

  DateTime now = rtc.now();

  //read the light
  sensors_event_t event;
  tsl.getEvent(&event);
 
  // Turn on the lights
  if (event.light >= 40.00 && now.hour() >= 22) 
  {
    Serial.println("Lights On");
    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Blue;
    leds[3] = CRGB::Blue;
    FastLED.show();
    
  }

 else if (event.light <= 40.00 || now.hour() >= 7){
    Serial.println("Lights Off");
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    FastLED.show();
    
 } 

}
