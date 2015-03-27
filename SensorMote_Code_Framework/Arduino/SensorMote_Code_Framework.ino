
/*__________________________________________________________\
|                                                           |
|       January 24 2014                                     |
|       Data Sensing Lab SensorMote                         |
|	Copyright kippkitts, LLC 2014	                    |
|			       	                            |
|	kippkitts, LLC                                      |
|	999 Main Street, Suite 705                          |
|       Pawtucket, RI USA			            |
|	Tel: 401-480-7972, Fax: 401-475-3574		    |
|							    |
|       Sensor Mote Test Code    	                    |
|                                                           |
|							    |
| kippkitts HAS MADE EVERY EFFORT TO INSURE THAT	    |
| THE CODE CONTAINED IN THIS RELEASE IS ERROR-FREE.	    |
| HOWEVER, kippkitts DOES NOT WARRANT THE                   |
| OPERATION OF THE CODE, ASSOCIATED FILES,		    |
| SCHEMATICS &/OR OTHER ENGINEERING MATERIAL.               |
|                                                           |
| THESE MATERIALS WERE PRODUCED FOR PROTOTYPE               |
| PURPOSES ONLY, AND ARE NOT INTENDED FOR                   |
| PRODUCTION. THEY ARE RELEASED SOLELY TO GIVE              |
| ASSISTANCE IN PREPARING                                   |
| THE PROJECT FOR FUTURE PRODUCTION.                        |
|                                                           |
| kippkitts ACCEPTS NO RESPONSIBILITY OR LIABILITY          |
| FOR THE USE OF THIS MATERIAL, OR ANY PRODUCTS             |
| THAT MAY BE BUILT WHICH UTILIZE IT.                       |
|                                                           |
|                                                           |
|		File name : SensorMote_Code_Framework.ino   |
|		Written by: 		K Bradford          |
|		Final edit date : 	24 JAN, 2014        |
|                                                           |
|                                                           |
|	1. This prototype code is not for release.          |
|                                                           |
|	2. This project is built for the Arduino Leonardo   |
|	                                                    |
|		NOT FOR PRODUCTION                          |
\__________________________________________________________*/


#include <Wire.h>
#include <EEPROM.h>


#define USE_BMP 1  // Comment this out to ignore the BMP085. Use this if it isn't responding for some reason
#define BMP085_ADDRESS 0x77  // I2C address of BMP085
#define CODE_VERSION 1.0


// ********** START DIGITAL PINS *************
//
// 0         RX (from XBee)
// 1         TX (to XBee)

// 2
// 3
// 4 RF-Detect-Shutdown
const int RF_shdwn = 4;

// 5  BMP085 XCLR
const int BMP085_XCLR = 5;

// 6  Pressure Matt or PIR input pin
const int butPin = 6;

// 7 BMP085 EOC
const int BMP085_EOC = 7;

// 8 LED (Power)
int powr_led = 8;

// 9 LED (Loop Activity)
int loop_led = 9;

// 10 LED (Motion Detected - aka Button Pushed)
int motn_led = 10;

// 11 RF Configuration Detection Pin
int rf_config = 11;

// 12 Gas Sensor Configuration Detection Pin
int gas_config = 12;


// ********** END DIGITAL PINS *************

// ********** START ANALOG PINS *************

// A0        Humidity Sensor (HTH-4030)
const int hih4030 = A0;

// A1        Light Sensor (TEMT6000)
const int temt6000 = A1;

// A2        Microphone
const int micPin = A2;

// A3        Gas Sensor
const int gasPin = A3;

// ********** END ANALOG PINS *************

// ADAFRUIT BMP085 Library
// Temperature & Humidity Sensor (BMP085) via I2C
// Uses I2C and the Wire Library
//
// NOTE: This is a 3.3V part
// NOTE: The SDA/SCL pins on the Uno and Leonardo are different.
// A4 & A5 (Arduino Uno)      
// 2 & 3 (Arduino Leonardo) 

// variables
short temperature;
long pressure;
const float p0 = 101325;             // Pressure at sea level (Pa)float altitude;
unsigned long samplingDelay = 4000; // default sampling delay in millis 
int micVal = 0;

int buttonState;             // the current reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

// SETUP ------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // to XBee
  
  // while the serial stream is not open, do nothing:
  while (!Serial) ;  // REMOVE THIS LINE OF CODE FOR STAND-ALONE USE
  Serial.println("Initializing...");
 
  // Toggle the LEDs for testing
  pinMode( powr_led, OUTPUT);
  pinMode( loop_led, OUTPUT);
  pinMode( motn_led, OUTPUT);
  digitalWrite(powr_led, HIGH);
  //digitalWrite(loop_led, HIGH);

/*
  // Use this code if you want to update sampling speed from EEPROM
  // Get the saved sampling delay from memory
  unsigned long eepromNumber = getNumber();
  
  // ignore a zero value
  if (eepromNumber > 0)
  {
    samplingDelay = eepromNumber;
  }
  Serial.print( "Sampling delay = " );
  Serial.println( samplingDelay );
*/
   
  if(USE_BMP)
  {
    bmp085Calibration();
  }
  
  delay(2000);
  
  Serial.print("DSL SensorMote version ");
  Serial.println(CODE_VERSION); 
  
  digitalWrite(loop_led, LOW);
  
}

// LOOP ------------------------------------------------------------------------------------------------------

void loop()
{
  
  getSample();
  
  // Button
  // read the state of the switch into a local variable:
  buttonState = digitalRead(butPin);
  
  digitalWrite(motn_led, buttonState);

}



// GET SAMPLE ------------------------------------------------------------------------------------------------------
void getSample(void) {
  digitalWrite(loop_led,HIGH);

  // Temperature, Pressure, Light, Humidity, Altitude

  temperature = bmp085GetTemperature(bmp085ReadUT());
  float temp = 0.1*(float)temperature;
  pressure = bmp085GetPressure(bmp085ReadUP()); 
  int light = analogRead(temt6000);
  int humid = analogRead(hih4030);
  float relative_humid = ((0.0004*temp + 0.149)*humid)-(0.0617*temp + 24.436);
  float altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));

  // Microphone
  micVal = getSound(); 
 
  // Gas sensor
  int gasValue =  analogRead(gasPin);
 
  // Output values to console
  Serial.print( "T = " );
  Serial.print( temp );
  Serial.print( "C, P = ");
  Serial.print( pressure );
  Serial.print( "Pa, H = " );
  Serial.print( relative_humid );
  Serial.print( "%, Light = " );
  Serial.print( light );
  Serial.print( ", A = " );
  Serial.print( altitude );
  Serial.print( ", Mic = " );
  Serial.print( micVal );
  Serial.print( ", Gas = " );
  Serial.print( gasValue );

  digitalWrite(loop_led, LOW);
  
}


// GET SOUND ------------------------------------------------------------------------------------------------------
//
// Does something sensible(ish) with the microphone input

int getSound(void)
{
  int sound = 0;
    
  sound = analogRead(micPin); // look at the voltage coming from the mic
  return sound;
}
