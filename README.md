# IoT-devices GGreg20_V3 Arduino Driver Library

IoT-devices GGreg20_V3 - an ionizing radiation detector module driver library for Arduino

This Library allows to measure and get five-minute moving average (MA5) valule of ionizing radiation power in microsieverts per hour. 

## Library settings
- Measurement cycle is set to 60 seconds. And can be changed at header file via GGREG20_V3_MEASURE_TIME.
- SBM-20 Conversion Factor is set as CPM * 0.0054 = uSv/h. And can be changed at header file via GGREG20_V3_CONVFACTOR.
- Debounce timeout is set to 5 milliseconds. And can be changed at header file via GGREG20_V3_DEBOUNCE_TIME.

## Install
### Install via Library Manager
> Note. This installation method is under construction. Please use Manual Installation.
1. Go to the **Tools** -> **Manage Libraries** menu
2. Search for **GGreg20_V3** library
3. Click **install** button
### Manual install
1. Download from GitHub
2. Unzip archive and place its contents under /Arduino/libraries/
3. Restart your Arduino IDE software
4. Go to the **File** -> **Examples** -> **GGreg20_V3** under the _Examples from custom libraries_ menu section

## Usage

1. Include library under your sketch like that:
````Arduino
#include "GGreg20_V3.h"
````
and declare driver instantce to access its public methods:
````Arduino
GGreg20_V3 GGREG(interruptPin);
````
See full usage example sketch under /GGreg20_V3/examples/

## Public methods
### GGREG.begin(int pin);
int begin(int pin) - Library Init method. Runs once usualy from void setup() section. This method sets interrupt pin and restarts counting from the begining. 

### GGREG.read_cpm();
int read_cpm() - returns current GM-tube counts per minute (CPM) value.

### GGREG.read_ma5();
float read_ma5() - returns ionizing radiation power (uSv/h) five-minute moving average value.

### GGREG.read_time();
long read_time()  - returns current one-minute measure-loop count value in milliseconds [0-60000].

### GGREG.read_ma5_pointer();
byte read_ma5_pointer() - returns current MA5 array pointer value [0-4->0].

### GGREG.read_ma5_arr_elements();
int read_ma5_arr_elements() - returns current number of MA5 array elements are filled with measurement values [0-5].

### GGREG.keepAlive();
static void keepAlive() - Library Watchdog function that keeps one-minute time-cycle measurement more precise. Must be run from void loop() section.
