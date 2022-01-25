/* 
  GGreg20_V3.cpp - GGreg20_V3 Arduino driver Library CPP code file
  GGreg20_V3 ionizing radiation detector module driver library for Arduino controllers

  Developed by IoT-devices LLC, Kyiv, Ukraine
  Library version 1.0.0 (Initial Release)

  Created at 2022-01-25
  Modified at 2022-01-25

  Full Online instructions: https://github.com/iotdevicesdev/GGreg20_V3

  License: Apache-2.0 License

*/

#include "Arduino.h"
#include "GGreg20_V3.h"

 static volatile int __pin;             // GPIO for an external interrupt
          static int cpm = 0;           // Counts per Minute variable
static unsigned long now = millis();    // Debounce comparison variable
static unsigned long meas_start = now;  // One minute timer comparison variable
          static int ma5_pointer;       // Moving average array pointer
        static float ma5_arr[5];        // Moving average array
        static float ma5_val;           // Main output value, five-minute moving average ionizing radiation power, uSV/h
          static int ma5_arr_elements;  // Moving average array number of elements are filled
         static byte i = 0;

// Class
GGreg20_V3::GGreg20_V3(int pin)
{
  _pin = pin;
  __pin = _pin;

}

// Init public method
int GGreg20_V3::begin(int pin){
  now = millis();
  meas_start = now;
  ma5_arr[0] = 888; // Filling array with impossible values of 888 (i.e. empty)
  ma5_arr[1] = 888;
  ma5_arr[2] = 888;
  ma5_arr[3] = 888;
  ma5_arr[4] = 888;
  ma5_arr[5] = 888;
  ma5_pointer = 0;
  ma5_val = 0;
  ma5_arr_elements = 0;

  Serial.begin(115200);
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), counter, LOW);
}

// More acurate timer control public method for void loop()
void GGreg20_V3::keepAlive(){
  counter();
}

// Interrupt Service Routine, private method
void GGreg20_V3::counter(){
  detachInterrupt(digitalPinToInterrupt(__pin));
  if (digitalRead(__pin) == LOW && (millis() - now) >= GGREG20_V3_DEBOUNCE_TIME) {
    cpm = cpm + 1;
    now = millis();
  }
  if (millis() - meas_start >= GGREG20_V3_MEASURE_TIME) {
    ma5_val = 0;
    ma5_arr[ma5_pointer] = cpm * GGREG20_V3_CONVFACTOR;
    i = 0;
    while(i < 5 && ma5_arr[i] != 888){
      ma5_arr_elements = (i + 1);
      i++;
    }
    i = 0;
    while(i < ma5_arr_elements && ma5_arr[i] != 888){
      ma5_val += ma5_arr[i];
      i++;
    }
    if(ma5_arr_elements != 0){ma5_val /= ma5_arr_elements;}
    if (ma5_pointer < 4){ma5_pointer++;} else {ma5_pointer = 0;}
    cpm = 0;
    meas_start = millis();
  }
  attachInterrupt(digitalPinToInterrupt(__pin), counter, LOW);
}

// Current CPM value fetch public method
int GGreg20_V3::read_cpm(){
  return cpm;
}

// Ionizing power MA5 value fetch public method
float GGreg20_V3::read_ma5(){
  return ma5_val;
}

// Current one-minute cycle timer value fetch public method
long GGreg20_V3::read_time(){
  return (millis() - meas_start);
}

// Current MA5-array pointer value fetch public method
byte GGreg20_V3::read_ma5_pointer(){
  return (ma5_pointer);
}

// Current number of MA5-array elements fullfilment value fetch public method
int GGreg20_V3::read_ma5_arr_elements(){
  return (ma5_arr_elements);
}
