
#include <Wire.h>
#include "Adafruit_BMP085.h"
 
Adafruit_BMP085 bmp;
 

// Google I/O Prototype Mote
// Breadboard Protoype Code
// Last Updated: 15-April-2013 04:00 EST

#define BMP085_ADDRESS 0x77  // I2C address of BMP085
#define CODE_VERSION 1.3
#define SILENT_VALUE 380  // Starting neutral microphone value (self-correcting)


// ********** START DIGITAL PINS *************
//
// 0         RX (from XBee)
// 1         TX (to XBee)
// 2
// 3
// 4 RF-Detect-Shutdown
const int RF_shdn = 4;
// 5  BMP085 XCLR
const int BMP085_XCLR = 5;
// 6         Button (Pressure Mat)
const int butPin = 6;
// 7 BMP085 EOC
const int BMP085_EOC = 7;
// 8         LED (Power)
int powr_led = 8;
// 9         LED (Loop Activity)
int loop_led = 9;
// 10        LED (Motion Detected - aka Button Pushed)
int motn_led = 10;
// 11       RF Configuration Detector
int rf_config = 11;
// 12       Gas Sensor Configuration Detector
int gas_config = 12;
// ********** END DIGITAL PINS *************


// ********** START ANALOG PINS *************
// A0        Humidity Sensor (HTH-4030)
const int hih4030 = A0;
// A1        Light Sensor (TEMT6000)
const int temt6000 = A1;
// A2        Microphone (Adafruit Board)
const int micPin = A2;
// A3        Gas Sensor
const int gasPin = A3;
// ********** END ANALOG PINS *************


// variables
int light;
int humid;
int gasValue;
int button;
int temp;
int micVal = 0;


// SETUP ------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // to XBee
  
  // while the serial stream is not open, do nothing:
//  while (!Serial) ;
  Serial.println("Initializing...");
 
  pinMode(RF_shdn, INPUT); // TESTING ONLY. SHOULD BE OUTPUT
  pinMode(BMP085_XCLR, OUTPUT); //
  pinMode(BMP085_EOC, INPUT); // End of conversion signal
  pinMode(butPin, INPUT);  // SET PULL-UP FOR CONFIG TESTING
  pinMode(rf_config, INPUT);  // SET PULL-UP FOR CONFIG TESTING
  pinMode(gas_config, INPUT);  // SET PULL-UP FOR CONFIG TESTING
 
  pinMode( powr_led, OUTPUT);
  pinMode( loop_led, OUTPUT);
  pinMode( motn_led, OUTPUT);

  digitalWrite(powr_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(powr_led, LOW);   // LED CHECK
  digitalWrite(loop_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(loop_led, LOW);   // LED CHECK
  digitalWrite(motn_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(motn_led, LOW);   // LED CHECK

  digitalWrite(powr_led, HIGH);  // LED OPERATIONAL
  digitalWrite(loop_led, HIGH);  // LED OPERATIONAL

  digitalWrite(BMP085_XCLR, HIGH);  // Make sure BMP085 is on
   
  Serial.print("SensorMote (Google I/O) v");
  Serial.println(CODE_VERSION);
  
  bmp.begin(); 
  
  digitalWrite(loop_led, LOW);
  
}

// LOOP ------------------------------------------------------------------------------------------------------
void loop() {
  
  // Button
  // read the state of the switch into a local variable:
  button = digitalRead(butPin);
  digitalWrite(motn_led, button);
 
  light = analogRead(temt6000);
  humid = analogRead(hih4030);
  gasValue =  analogRead(gasPin);
  micVal = getSound(); 

 
  // Output
  Serial.print("Button = ");
  Serial.print( button );
  Serial.print( ", Humidity = " );
  Serial.print( humid );
  Serial.print( "(raw), Light = " );
  Serial.print( light );
  Serial.print( ", Mic = " );
  Serial.print( micVal );
  Serial.print( ", Gas or RF = " );
  Serial.print( gasValue );

  Serial.print(", Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.print(" *C, ");
 
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");


  digitalWrite(loop_led, HIGH);

  Serial1.print("Hello XBee!!");

  Serial1.print("Button = ");
  Serial1.print( button );
  Serial1.print( ", Humidity = " );
  Serial1.print( humid );
  Serial1.print( "(raw), Light = " );
  Serial1.print( light );
  Serial1.print( ", Mic = " );
  Serial1.print( micVal );
  Serial1.print( ", Gas or RF = " );
  Serial1.print( gasValue );

  Serial1.print(", Temperature = ");
  Serial1.print(bmp.readTemperature());
  Serial1.print(" *C, ");
 
  Serial1.print("Pressure = ");
  Serial1.print(bmp.readPressure());
  Serial1.println(" Pa");

  digitalWrite(loop_led, LOW);

  
}

// GET SOUND ------------------------------------------------------------------------------------------------------
//
// Does something sensible(ish) with the microphone input

int getSound() {
  static int average = SILENT_VALUE; // stores the neutral position for the mic
  static int avgEnvelope = 0; // stores the average sound pressure level
  int avgSmoothing = 10; // larger values give more smoothing for the average
  int envSmoothing = 2; // larger values give more smoothing for the envelope
  int numSamples=1000; //how many samples to take
  int envelope=0; //stores the mean sound from many samples
  for (int i=0; i<numSamples; i++) {
    int sound=analogRead(micPin); // look at the voltage coming from the mic
    int sampleEnvelope = abs(sound - average); // the distance from this reading to the average
    envelope = (sampleEnvelope+envelope)/2;
    avgEnvelope = (envSmoothing * avgEnvelope + sampleEnvelope) / (envSmoothing + 1);
    //Serial.println(avgEnvelope);
    average = (avgSmoothing * average + sound) / (avgSmoothing + 1); //create a new average
  }
  return envelope;
}
