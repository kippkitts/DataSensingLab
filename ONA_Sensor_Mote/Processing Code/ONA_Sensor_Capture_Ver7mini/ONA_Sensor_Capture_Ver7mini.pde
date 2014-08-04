/*__________________________________________________________\
|                                                           |
|       October 18 2013                                     |
|       Data Sensing Lab SensorMote                         |
|  Copyright kippkitts, LLC 2013                            |
|                                                           |
|  kippkitts, LLC                                           |
|  999 Main Street, Suite 705                               |
|  Pawtucket, RI USA                                        |
|  Tel: 401-400-0KIT, Fax: 401-475-3574                     |
|                                                           |
|       ONA Sensor Capture V7.0                             |
|                                                           |
|                                                           |
| kippkitts HAS MADE EVERY EFFORT TO INSURE THAT            |
| THE CODE CONTAINED IN THIS RELEASE IS ERROR-FREE.         |
| HOWEVER, kippkitts DOES NOT WARRANT THE                   |
| OPERATION OF THE CODE, ASSOCIATED FILES,                  |
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
|    File name : ONA_Sensor_Capture_Ver7mini.pde            |
|    Written by:     K Bradford                             |
|    Final edit date :   31 OCT, 2013                       |
|                                                           |
|                                                           |
|  1. This prototype code is not for release.               |
|                                                           |
|  2. This project is built for the kippkitts MSP430        |
|          Sensormote                                       |
|                                                           |
|    NOT FOR PRODUCTION                                     |
\__________________________________________________________*/

// Pull in the serial port interface. This requires pre-installation of the correct USB->serial drivers.
import processing.serial.*;
import java.io.BufferedWriter;
import java.io.FileWriter;

PrintWriter my_file;    // Create a stream to push the data to... This can be set to append data rather than overwrite.
PrintWriter motionFile;
PrintWriter motionFileArray;

Serial myPort;      // create the serial port variable

//volatile int motion_count = 0;
int motion_count = 0;

int lf = 10;    // Linefeed in ASCII
String myString = null;

String[] my_parse;

String s_day = null;
String s_mon = null;
String s_yrr = null;

String s_sec = null;
String s_min = null;
String s_hrr = null;

String myfilePath = "/Dropbox/Projects/ONA_Data/ona_data_live.csv"; 
String motionfilePath = "/Dropbox/Projects/ONA_Data/ona_data_motion.csv"; 
String motionfileArray = "/Dropbox/Projects/ONA_Data/ona_motion_array.csv"; 
// Obviously, change the path to adapt to your system

String my_data = null;
String motion_temp = null;
int motion_global = 0;  // use this global to grab the last recorded motion

int sensor_id = 1;
int index = 0;
int timeout = 0;

String header_string = "Mote,Temp,Mic,RF,AQ";

//String[] csv_string = new String[32];
String[] csv_string = new String[31];
String[] motion_string = new String[31];


void setup() {
  println(Serial.list());
//  myPort = new Serial(this, Serial.list()[15], 9600);
    myPort = new Serial(this, Serial.list()[0], 9600);

  motion_count = 0;
  
  delay(1000);
}

void draw () {              

  int timeout;
    delay(100); 

    csv_string[0] = header_string;
    sensor_id = 1;
    while(sensor_id < 31)
    { 
      println("Sending " + sensor_id);
      myPort.write(':');
      myPort.write((char)sensor_id);
      myPort.write(2);

      delay(20);

      timeout = 6;
      while((myPort.available() == 0) && (timeout > 0))
      {
        timeout--;
        delay(50);
        myPort.write(':');
        myPort.write((char)sensor_id);
        myPort.write(2);

      }

      if (myPort.available() > 0)
      {
        while (myPort.available() > 0)
        {
          myString = myPort.readStringUntil(lf);
          if (myString != null)
          {
            csv_string[sensor_id] = processString(sensor_id, myString);
            motion_string[sensor_id] = String.valueOf(motion_global);
            print(csv_string[sensor_id]);
            println("," + motion_count);
          }
        }
      }
      else
      {
        //csv_string[sensor_id] = nullString(sensor_id);  // Overwrite stale values with null
        csv_string[sensor_id] = csv_string[sensor_id];    // Keep stale values
      }     
      delay(1000);    // pause before grabbing data
      sensor_id++;
    }

// Write the data to files:
    my_data = join(csv_string, "\n");  // combine all the data
    my_file = createWriter(myfilePath);
    my_file.println(my_data);
    my_file.flush(); 
    my_file.close();

    motionFile = createWriter(motionfilePath);
    motionFile.println("Motion_Count: ");
    motionFile.println(String.valueOf(motion_count));
    motionFile.flush(); 
    motionFile.close();

    motion_temp = join(motion_string, ",");
    motionFile = createWriter(motionfileArray);
    motionFile.print("Motion_Array: ");
    motionFile.println(motion_temp);
    motionFile.flush(); 
    motionFile.close();

    motion_count = 0;  // Clear the motion count after it's written to a file

}

void keyPressed() {
//  my_file.flush(); 
//  my_file.close();
  exit(); 
}

// this function takes the incoming data string, parses it, and returns it as a CSV string
String processString(int the_id, String theString) {
  int raw_temp = 0;
  int motion_temp;
  String string_temp = null;

  String[] sensor_values = new String[5];
  
// Parse the incoming string to extract the data
  my_parse = splitTokens(theString, "\t  =");

//  print("The ID = ");
//  println(the_id);
  sensor_values[0] = String.valueOf(the_id);  // put the ID value into the array as a string
//  print("Now the ID = ");
//  println(sensor_values[0]);

  raw_temp = int(my_parse[1]);  // get the current temperature raw value string and convert to an int
  sensor_values[1] = str_convert_temp(raw_temp);  //move temperature post conversion into the array
  sensor_values[2] = my_parse[3];  // get the mic value
  sensor_values[3] = my_parse[5];  // get the rf value
  sensor_values[4] = my_parse[7];  // get the aq value
  
  string_temp = trim(my_parse[9]);  // get rid of
  
  motion_temp = int(string_temp);
  motion_count = motion_count + motion_temp;
  motion_global = motion_temp;
   
  return join(sensor_values, ",");
}

String nullString(int mote_id)
{
  int[] numbers = new int[5]; 
  numbers[0] = mote_id; 
  numbers[1] = 0;   // no temp
  numbers[2] = 0;   // no mic
  numbers[3] = 0;   // no rf
  numbers[4] = 0;   // no aq
  // just ignore the motion. it is zero anyway.
  return join(nf(numbers, 0), ","); 
}

String get_time_string()
{
  String[] timestamp = new String[6];

  timestamp[0] = String.valueOf(year());
  timestamp[1] = String.valueOf(month());
  timestamp[2] = String.valueOf(day());
  timestamp[3] = String.valueOf(hour());
  timestamp[4] = String.valueOf(minute());
  timestamp[5] = String.valueOf(second());
  
  return join(timestamp, ",");
}

void appendToFile(String filePath, String data)
{
  PrintWriter pw = null;
  try 
  {
    pw = new PrintWriter(new BufferedWriter(new FileWriter(filePath, true))); // true means: "append"
    pw.println(data); 
  } 
  catch (IOException e)
  {
    // Report problem or handle it
    e.printStackTrace();
  }

  finally
  {
    if (pw != null)
    {
      pw.close();
    }
  } 
}

// Use this function to get a Float
float convert_temp(int temp_sense)
{
  return float(nf(((((3.3 * float(temp_sense))/1024) - 0.6)*100), 2,2));
}

// Use this function to get a String
String str_convert_temp(int temp_sense)
{
  return nf(((((3.0  * float(temp_sense))/1024) - 0.6)*100), 2,2);
}

