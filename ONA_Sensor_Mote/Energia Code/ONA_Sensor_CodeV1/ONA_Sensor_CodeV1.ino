
/*__________________________________________________________\
|                                                           |
|       October 18 2013                                     |
|       Data Sensing Lab SensorMote                         |
|	Copyright kippkitts, LLC 2013	                    |
|			       	                            |
|	kippkitts, LLC                                      |
|	999 Main Street, Suite 705                          |
|       Pawtucket, RI USA			            |
|	Tel: 401-480-7972, Fax: 401-475-3574		    |
|							    |
|       ONA Sensor Mote Code V1.0    	                    |
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
|		File name : ONA_Sensor_CodeV1.ino           |
|		Written by: 		K Bradford          |
|		Final edit date : 	31 OCT, 2013        |
|                                                           |
|                                                           |
|	1. This prototype code is not for release.          |
|                                                           |
|	2. This project is built for the kippkitts MSP430   |
|          Sensormote                                       |
|	                                                    |
|		NOT FOR PRODUCTION                          |
\__________________________________________________________*/

// this doesn't work as a # define. it gives errors in the GetComm line.
char my_id = 14;

int mic_value;
int rf_value;
int aq_value;
int therm_value;
int pir_val;

volatile int motion_count;

char my_command;

void motion(void);  // This is the Interrupt Service Routine (ISR) for the motion detector
void Sample_No_AQ(void);
void Sample_AQ(void);
void AQ_on(void);
void AQ_off(void);
void Sleep(void);
char GetComm(void);


  
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(P2_1, OUTPUT);     // ACT
  pinMode(P2_2, OUTPUT);     // MOT
  pinMode(P2_3, OUTPUT);     // AQ On Indicator
  pinMode(P2_5, INPUT);
//  pinMode(P1_3, INPUT_PULLUP);  // PIR for Motion
  pinMode(P1_3, INPUT);  // PIR for Motion
  pinMode(P2_0, OUTPUT);

  digitalWrite(P2_0, HIGH);   // turn off the RF detector
//  digitalWrite(P2_3, HIGH);  // turn on the AQ sensor
  digitalWrite(P2_3, LOW);  // turn off the AQ sensor

  Serial.begin(9600);
//  Serial.print("\n\n\n*** MSP430 Sensor Lab \n"); 
  delay(1000);
  Serial.flush();

  motion_count = 0;
  attachInterrupt(P1_3, motion, FALLING);
//  attachInterrupt(P1_3, mot_off, RISING); // seems we can't have both active at the same time...

  digitalWrite(P2_1, HIGH);   // turn the ACT LED on to let us know the MSP430 is alive
}

// the loop routine runs over and over again forever:
void loop() {

  my_command = GetComm();
  switch (my_command) {
    case 0:
      //digitalWrite(P2_1, LOW);   // turn the ACT LED on to let us know the MSP430 is alive
      break;            // do nothing if command isn't for me, or it's an invalid command
    case 1:
      Sample_No_AQ();   // Sample sensors with AQ off when var equals 1
      break;
    case 2:
      Sample_AQ();      // Sample sensors with AQ on whenn var equals 2
      break;
    case 8:
      AQ_on();         // Sample sensors with AQ off when var equals 1
      break;
    case 9:
      AQ_off();        // Sample sensors with AQ on whenn var equals 2
      break;

    default: 
      ;                // if nothing else matches, do the default
      // default is optional
  }
  
}

void Sleep(void)
{
  // turn off the radio and sensors for 10 seconds.
//  digitalWrite(P2_0, LOW);   // turn off the RF detector
//  delay(5000);
//  delay(5000);
//  digitalWrite(P2_0, HIGH);   // turn off the RF detector
  delay(100);  // this is a dummy command... fix sleep later...

}

void motion(void)
{
  motion_count++;
  digitalWrite(P2_2, HIGH);    // turn the LED on (HIGH is the voltage level)
  attachInterrupt(P1_3, mot_off, RISING); // seems we can't have both active at the same time... so attache the rising edge int to turn off LED

}

void mot_off(void)
{
  digitalWrite(P2_2, LOW);    // turn the LED on (HIGH is the voltage level)
  attachInterrupt(P1_3, motion, FALLING);  // reattach the falling edge interrupt to detect motion
}
