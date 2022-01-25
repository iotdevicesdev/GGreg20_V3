# IoT-devices GGreg20_V3 Arduino Driver Library

IoT-devices GGreg20_V3 - an ionizing radiation detector module driver library for Arduino

This Library allows to measure and get five-minute moving average (MA5) valule of ionizing radiation power in microsieverts per hour using GGreg20_V3 detector module manufactured by IoT-devices LLC. 

## Library constants and their settings
- Measurement cycle is set to 60 seconds. And can be changed at header file via GGREG20_V3_MEASURE_TIME.
- SBM-20 Conversion Factor is set as CPM * 0.0054 = uSv/h. And can be changed at header file via GGREG20_V3_CONVFACTOR.
- Debounce timeout is set to 5 milliseconds. And can be changed at header file via GGREG20_V3_DEBOUNCE_TIME.

## Library Install
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

## Library Usage

1. Include library under your sketch like that:
````C++
#include "GGreg20_V3.h"
````
2. Choose and set a right GPIO to manage incoming pulses
````C++
byte interruptPin = 2;
````
3. Declare driver instantce to access its public methods:
````C++
GGreg20_V3 GGREG(interruptPin);
````
See full usage example sketch under /GGreg20_V3/examples/ or simple code here:
````C++
#include "GGreg20_V3.h"

byte interruptPin = 2;
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
````

## Library public methods
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

## Hardware Setup
The GGreg20_V3 output supports direct connection to the Arduino and we recommend powering both modules of the same power supply and turning them on at the same time.
However, if you plan to control the on / off of the GGreg20_V3 sensor regardless of the main controller being switched on, the following information may be useful to you.

>For example, there may be a situation where the GGreg20_V3 detector has a separate power supply and the GGreg20_V3 is not switched on at the same time as the controller, but by a separate command from the controller or a physical switch manually.
In this case, processes occur between the devices, which with a difference of 3V3 at the output and 5V at the input can lead to the establishment of an indeterminate state on the controller port. Which in turn stops interrupt processing and, as a consequence, stops measuring the number of pulses coming from the sensor.

In this case, it is necessary to match the logic voltage levels between these two components.

We recommend that you do this:

**It is necessary** to connect a **0.1uF ceramic capacitor in series** in the signal circuit (wire between the pulse output of the sensor and the input GPIO of the controller). 

The figure shows a diagram and a photo of how the capacitor is connected. The presence of a capacitor makes it impossible to block the input of the controller and the circuit will work stably.

FIGURE

Note that this solution takes into account the complex internal processes that occur between the connected electronic components, which can not be described in a limited amount of this document and was chosen by experts as optimal to solve this particular problem.
