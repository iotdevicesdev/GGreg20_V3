/* 
  Tutorial sketch on using GGreg20_V3 Arduino driver Library
  This example shows how to connect and use GGreg20_V3 ionizing radiation detector module with Arduino controllers

  Developed by IoT-devices LLC, Kyiv, Ukraine
  Library version 1.0.0 (Initial Release)

  Created at 2022-01-25
  Modified at 2022-01-25

  Full Online instructions: https://github.com/iotdevicesdev/GGreg20_V3

  Circuit signals
   - GGreg20_V3 3V3 (active-low) pulse output pin - 0.1uF ceramic capacitor in series - Arduino UNO GPIO2 5V input
   - GGreg20_V3 GND pin - Arduino board GND pin

*/
// install the GGreg20_V3 library via Library Manager
// if using the Arduino IDE, click here: http://librarymanager#GGreg20_V3
#include "GGreg20_V3.h"

byte interruptPin = 2; // Arduino pin witch will handle external interrupts from GGreg20_V3
volatile float radPower_uSVh = 0;
volatile int   radPower_CPM = 0;
volatile int   radMA5_Confidence = 0;

// Library Class driver instance 'GGREG' init
GGreg20_V3 GGREG(interruptPin);

// Standard Arduino Sketch void setup() block
void setup()
{
  // Driver initialization public method
  GGREG.begin(interruptPin);
}

// Standard Arduino Sketch void loop() block
void loop()
{
  // By default each pulse from GPIO refreshes driver's an one-minute watchdog time counter.
  // It is not necessary, but if you want to refresh watchdog state with a higher frequency, you may also call it from the void loop() here.
  // This makes 60-second measure cycle more accurate.
  GGREG.keepAlive();

  // Returns current one-minute cycle ionizing radiation power CPM value, uSV/hour
  // CPM - counts per minute
  radPower_CPM = GGREG.read_cpm();
  
  // Returns current ionizing radiation power MA5 value, uSV/hour
  // MA5 - five-minute moving average
  // MA5 value makes available after 1 minute of measurement:
  // 0.00 - impossible value in normal conditions (wait 1-minute, check sensor, wiring and power)
  // 0.10 - 0.30 - NORMAL level, uSv/h
  // 0.30 - 0.60 - WARNING level, uSv,h
  // 0.60 - > HAZARD level, uSv/h
  radPower_uSVh = GGREG.read_ma5();
  
  // Returns the state of five-minute moving average array of measurements
  // For the propose of this example, number of array calculated elements may be treated as confidence level
  // 0 - lowet, 5 - highest confidence level of measurement results from program start
  radMA5_Confidence = GGREG.read_ma5_arr_elements();
  
  // Test debug output to serial console of values calculated inside the GGreg20_V3 driver library
  Serial.println(GGREG.read_time());
  Serial.println(GGREG.read_ma5_pointer());
  Serial.println(radMA5_Confidence);
  Serial.println(radPower_CPM);
  Serial.println(radPower_uSVh);
  
  delay(1000);
}
