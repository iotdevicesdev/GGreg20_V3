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

/**************************************************************************/
/*!
    @brief  GGreg20_V3 Driver Library Class
    @param  pin A pin connected to the controller GPIO to handle external interrupts
*/
/**************************************************************************/
GGreg20_V3::GGreg20_V3(int pin)
{
  _pin = pin;
  __pin = _pin;

}


/**************************************************************************/
/*!
    @brief  Initial library setup public method
    @param  pin A pin connected to the controller GPIO to handle external interrupts
*/
/**************************************************************************/
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


/**************************************************************************/
/*!
    @brief  More acurate timer control public method for void loop()
*/
/**************************************************************************/
void GGreg20_V3::keepAlive(){
  counter();
}


/**************************************************************************/
/*!
    @brief  Interrupt Service Routine, calculates all values, private method
*/
/**************************************************************************/
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


/**************************************************************************/
/*!
    @brief  Returns current Counts per Minute (CPM) value, public method
    @return cpm Instant Counts per Minute (CPM) value
*/
/**************************************************************************/
int GGreg20_V3::read_cpm(){
  return cpm;
}


/**************************************************************************/
/*!
    @brief  Returns ionizing radiation power MA5 value, public method
    @return ma5_val Cummulative moving avegage five-minute value
*/
/**************************************************************************/
float GGreg20_V3::read_ma5(){
  return ma5_val;
}


/**************************************************************************/
/*!
    @brief  Returns current one-minute cycle timer value, public method
    @return One-minute time counter value in milliseconds
*/
/**************************************************************************/
long GGreg20_V3::read_time(){
  return (millis() - meas_start);
}


/**************************************************************************/
/*!
    @brief  Returns current MA5-array pointer value, public method
    @return ma5_pointer Current MA5-array pointer value [0-4]
*/
/**************************************************************************/
byte GGreg20_V3::read_ma5_pointer(){
  return (ma5_pointer);
}


/**************************************************************************/
/*!
    @brief  Returns current number of MA5-array elements fullfilment value, public method
    @return ma5_arr_elements Current number of MA5-elements in the array value [0-5]
*/
/**************************************************************************/
int GGreg20_V3::read_ma5_arr_elements(){
  return (ma5_arr_elements);
}
