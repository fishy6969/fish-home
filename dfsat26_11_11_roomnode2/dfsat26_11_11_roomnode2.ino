#include <OneWire.h>
#include <DallasTemperature.h>
#include <JeeLib.h>
#include <avr/sleep.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4
Port ldr (1);

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
struct {
  byte light;
  float temp;
} payload;
  
void setup()
{

  sensors.begin();
  ldr.mode2(INPUT);
  ldr.digiWrite2(1);
  rf12_initialize(22, RF12_868MHZ, 212);
  rf12_sleep(0);
}


void loop()
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  payload.light = 255 - ldr.anaRead() / 4;
  payload.temp = (sensors.getTempCByIndex(0));
  rf12_sleep(-1);
  while (!rf12_canSend())
       rf12_recvDone();
  rf12_sendStart(0, &payload, sizeof payload, 2);
  rf12_sleep(0);
  
  for (byte i = 0 ; i < 3; ++i)
        Sleepy::loseSomeTime(60000);


  
}
