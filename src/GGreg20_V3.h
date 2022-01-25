/* 
  GGreg20_V3.h - GGreg20_V3 Arduino driver Library Header file
  GGreg20_V3 ionizing radiation detector module driver library for Arduino controllers

  Developed by IoT-devices LLC, Kyiv, Ukraine
  Library version 1.0.0 (Initial Release)

  Created at 2022-01-25
  Modified at 2022-01-25

  Full Online instructions: https://github.com/iotdevicesdev/GGreg20_V3

  License: Apache-2.0 License

*/
#ifndef GGreg20_V3_h
#define GGreg20_V3_h

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#ifndef GGREG20_V3_DEBOUNCE_TIME
#define GGREG20_V3_DEBOUNCE_TIME 5
#endif
#ifndef GGREG20_V3_MEASURE_TIME
#define GGREG20_V3_MEASURE_TIME 60000
#endif
#ifndef GGREG20_V3_CONVFACTOR
#define GGREG20_V3_CONVFACTOR 0.0054
#endif

class GGreg20_V3
{
  public:
    GGreg20_V3(int pin);
    int begin(int pin);
    int read_cpm();
    float read_ma5();
    long read_time();
    byte read_ma5_pointer();
    int read_ma5_arr_elements();
    static void keepAlive();

  private:
    static void counter();
    int _pin;
};

#endif
