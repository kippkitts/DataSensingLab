/*------------------------------------------------------\
|	Copyleft kippkitts LLC, 2013    		|
|       http://creativecommons.org/licenses/by-sa/3.0/  |
|       Attribution-ShareAlike 3.0 Unported             |
|							|
|	kippkitts, LLC.			          	|
|	999 Main Street STE 705                         |
|       Pawtucket, RI USA                		|
|	Tel: 401-400-0KIT, Fax: 401-475-3574		|
|							|
| Arduino code: DSL Workshop Shield      		|
|							|
| kippkitts, LLC HAS MADE EVERY EFFORT TO INSURE THAT	|
| THE CODE CONTAINED IN THIS RELEASE IS ERROR-FREE.	|
| HOWEVER, kippkitts DOES NOT WARRANT THE		|
| OPERATION OF THE CODE, ASSOCIATED FILES,      	|
| SCHEMATICS &/OR OTHER ENGINEERING MATERIAL.		|
|							|
| THESE MATERIALS WERE PRODUCED FOR PROTOTYPE		|
| PURPOSES ONLY, AND ARE NOT INTENDED FOR		|
| PRODUCTION. THEY ARE RELEASED SOLELY TO GIVE A	|
| DEVELOPER PRELIMINARY ASSISTANCE IN            	|
| THEIR PROJECT.                			|
|							|
| THIS CODE IS PROVIDED "AS IS".                        |
| kippkitts ACCEPTS NO RESPONSIBILITY OR LIABILITY	|
| FOR THE USE OF THIS MATERIAL, OR ANY PRODUCTS	        |
| THAT MAY BE BUILT WHICH UTILIZE IT.			|
|							|
|							|
|	File name : 	DSL_Workshop_Shield_ver2.ino	|
|	Written by: 	Kipp Bradford, Veton Saliu      |
|	Final edit date : 	28 Feb 2014      	|
|							|
|							|
|							|
|Notes:							|
|							|
|							|
|		NOT FOR PRODUCTION			|
|							|
\_______________________________________________________*/

// YOU MUST INSTALL THE ADAFRUIT DHT 22 LIBRARY
// You can find the library here:
// https://github.com/adafruit/DHT-sensor-library

// I've included the library like this:
#include "DHT.h"

// I define what pin I've connected the Temp/Humidity sensor to
#define DHTPIN 2

// Uncomment the proper DHTxx device to get the correct settings:
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Here, define the pins that we'll be using with this program
// Pins
//
// 0         RX (from XBee)
// 1         TX (to XBee)

// 2         DHT22 data pin
// 3         Button (or Motion Detector) 
// 8         LED (Power)
// 9         LED (Motion Detected - aka Button Pushed)
// 13        LED (System Activity)

// A0        Microphone (Adafruit Board)

// Now assign names to the pins for clear coding:
const int PIR_Pin = 3;
const int power_led = 8;
const int motion_led = 9;
const int status_led = 13;
const int micPin = A0;

boolean status_led_state = false;
boolean power_led_state = true;
int led_timer = 0;

int millis_old = 0;

int Comm_rate = 1000;
boolean Xbee_on = false;

// Declare the variables for this program:
int buttonState;   // the current reading from the input pin
int micValue;      // the current reading from the microphone

float h = 0;  // current humidity reading
float t = 0;  // current temperature reading
//float h_old = 0;

// We need to create a DHT device so we can get our data
DHT dht(DHTPIN, DHTTYPE);


// SETUP ------------------------------------------------------------------------------------------------------
void setup() 
{
  // "Serial" sends to your computer via USB
  Serial.begin(9600);
  // "Serial1" sends to your computer via the XBee
  Serial1.begin(9600);  // to XBee
  
//  Serial.print("Sensor Workshop V2.0");
  
  // Let's set the direction of the I/O lines
  pinMode( power_led, OUTPUT);
  pinMode( status_led, OUTPUT);
  pinMode( motion_led, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode( micPin, INPUT);
  pinMode( PIR_Pin, INPUT);
  
  
  digitalWrite(PIR_Pin, HIGH);  // Turn on the pull-up if you are just using a pushbutton
  
  // Lets test some LEDs!
//  Serial.println("Checking LEDs...");
  
  digitalWrite(power_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(power_led, LOW);   // LED CHECK
  digitalWrite(status_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(status_led, LOW);   // LED CHECK
  digitalWrite(motion_led, HIGH);  // LED CHECK
  delay(1000);
  digitalWrite(motion_led, LOW);   // LED CHECK

  digitalWrite(power_led, HIGH);  // Let's leave this on for operation
  digitalWrite(status_led, HIGH);  // Let's leave this on for operation

  delay(2000);  // This little delay let's you see the light(s)

}

// LOOP ------------------------------------------------------------------------------------------------------
void loop()
{
  
  if(Serial.available()>0 || Serial1.available()>0){
   int control_val = int(Serial.read());
   //Set communication rate
   if(control_val>=48 && control_val<=52){
    Comm_rate = (control_val-48)*500; 
   }
   //Turn Xbee comm on or off
   if(control_val==53 || control_val==54){
    Xbee_on = control_val-5-48; 
   }
   
   switch (control_val) {//Turn LED on or off
    case 7:
      digitalWrite(status_led, false);
      break;
   
    case 8:
      digitalWrite(status_led, true);
      break;
   
    case 9:
      digitalWrite(power_led, false);
      break;
   
    case 10:
      digitalWrite(power_led, true);
      break; 
   }
  }
  
  if(millis()-millis_old>=Comm_rate){//Set rate
    millis_old = millis();

  // read the state of the switch into a variable:
  buttonState = digitalRead(PIR_Pin);
  // now write that output to the LED
  digitalWrite(motion_led, buttonState); 


  // Read the mic value
  micValue = abs(analogRead(micPin)); 

  // Send the status of the motion detector to the USB
  Serial.print(buttonState, BIN);
  Serial.print(",");
  // Send the status of the mic detector to the USB
  Serial.print(micValue);
  Serial.print(",");
  
  if(Xbee_on){//If Xbee enabled
  // Send the status of the motion detector to the XBee
  Serial1.print(buttonState, BIN);
  Serial1.println(",");
  // Send the status of the mic detector to the XBee
  Serial1.print(micValue);
  Serial1.println(",");
  }


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h))
  {
//    Serial.println("Failed to read from DHT");
//    Serial1.println("Failed to read from DHT");
  } 
  else 
  {
    Serial.print(h);
    Serial.print(",");
    Serial.print(t);

    if(Xbee_on){
    Serial1.print(h); 
    Serial1.print(",");
    Serial1.print(t);
    }
  }
  
  Serial.println();
  Serial1.println();  
  }   
}  

