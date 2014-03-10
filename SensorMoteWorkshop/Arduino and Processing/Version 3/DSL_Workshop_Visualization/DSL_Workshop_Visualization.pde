import processing.serial.*;

Serial[] SerialPort;
//Serial SerialPort;
Tab[] tab = new Tab[2];
Chart temp_chart, humidity_chart;
DropDown[] dropdown_serial; 
DropDown[] dropdown_rate;
LED LED1, LED2;
//String[] PortList = new String[15];
int index = 0;
boolean pressed = false;
int last_time = 0;
boolean vis1_selected = true;
boolean vis2_selected = false;

int MIC, PIR;
float TEMP, HUM;

String selected_port = "";
boolean display_serial = false;
int dropdown_x = 50;
int dropdown_y = 130;
int dropdown_w = 130;
int dropdown_h = 20;
int port_length;
int port_id = 100;
boolean Serial_set = true;

boolean xbee_state = false;
boolean xbee_state_last = true;

int selected_rate = 1000;
int selected_rate_last = 500;
int rate_x = 200;
boolean display_rate = false;

int xbee_x = 385;
float xbee_y = 130;
int xbee_width = 50;
int xbee_height = 20;

void setup() {
  size(500, 500);
  background(150);
  //store list of serial ports in array
  //  port = new Serial(this, Serial.list()[0], 9600);
  //  port.bufferUntil('\n');
  port_length = Serial.list().length;

  if (port_length==0) {
    //run error message sketch
    exit();
  }

  SerialPort = new Serial[port_length];  
  dropdown_serial = new DropDown[port_length];
  dropdown_rate = new DropDown[5];
  //if only one serial port available, 
  //First menu should be Settings Menu, second menu should be Read or Visualization
  //Settings menu should have Serial Port dropdown list, communication rate, led control, enable/disable xbee 

  for (int i=0; i<port_length; i++) {
    SerialPort[i] = new Serial(this, Serial.list()[i], 9600);
    SerialPort[i].bufferUntil('\n');
    dropdown_serial[i] = new DropDown(dropdown_x, dropdown_y+(i+1)*dropdown_h, dropdown_w, dropdown_h, 0.67*dropdown_h, Serial.list()[i], i);
  }

  for (int i=0; i<5; i++) {
    int rate = i*500+500;
    dropdown_rate[i] = new DropDown(rate_x, dropdown_y+(i+1)*dropdown_h, dropdown_w, dropdown_h, 0.67*dropdown_h, rate+"", rate);
  }

  tab[0] = new Tab(0, height/10, 200, true);
  tab[1] = new Tab(width/2, height/10, 250, false);

  temp_chart = new Chart(350, 150, width/2-350/2, height/10+20, "Temperature");
  humidity_chart = new Chart(350, 150, width/2-350/2, height/2+50, "Humidity");

  LED1 = new LED(true, 410, 220);
  LED2 = new LED(true, 410, 305);
}

void draw() {

  background(100, 150, 250);
  tab[0].display();
  tab[1].display();
  if (tab[1].selected) {
    if (vis1_selected) {
      vis1();
    }
    else {
      vis2();
    }
  }
  else {
    Settings();
  }

  //  ReadSerial();




  //  if(pressed){
  //  //if(temp_chart.xpos > width/2-temp_chart._width/2){
  //  temp_chart.xpos = width-index;
  //  
  //  index = index + 10;
  //  last_time = millis();
  //  //}
  //  //}
  //temp_chart.display();
  //  }

  if (port_id!=100 && (selected_rate!=selected_rate_last || xbee_state!=xbee_state_last)) {
    int selected_rate_adj = selected_rate/500;
    int xbee_state_adj = int(xbee_state)+5;
    SerialPort[port_id].write(selected_rate_adj);
    SerialPort[port_id].write(xbee_state_adj);
    selected_rate_last = selected_rate;
    xbee_state_last = xbee_state;
  }
}

//void ReadSerial() {
//  if (port_id!=100) {
//    if (SerialPort[port_id].available()>0) {
//      //      println(SerialPort[port_id].read());
//    }
//  }
//}


void serialEvent(Serial port) {
  if (port_id!=100 && port.equals(SerialPort[port_id])) {   
    String message = port.readStringUntil('\n');
    String[] data = split(trim(message), ","); 

    //  if((selected_rate!=selected_rate_last || xbee_state!=xbee_state_last)){
    //    int selected_rate_adj = selected_rate/500;
    //    int xbee_state_adj = int(xbee_state)+5;
    //    port.write(selected_rate_adj);
    //    port.write(xbee_state_adj);
    //    selected_rate_last = selected_rate;
    //    xbee_state_last = xbee_state;
    //  }

    if (data.length>3) {
      PIR = int(data[0]);
      MIC = int(data[1]);

      float HUM_old = HUM;
      HUM = float(data[2]);
      float TEMP_old = TEMP;
      TEMP = float(data[3]);      

      //Update temp data array only if incoming value has changed
      if (TEMP!=TEMP_old) {
        for (int i=0; i<temp_chart.data.length-1; i++) {
          temp_chart.data[i] = temp_chart.data[i+1];
        }
        temp_chart.data[temp_chart.data.length-1] = TEMP;
      }

      if (HUM!=HUM_old) {
        for (int i=0; i<humidity_chart.data.length-1; i++) {
          humidity_chart.data[i] = humidity_chart.data[i+1];
        }
        humidity_chart.data[humidity_chart.data.length-1] = HUM;
      }   

      //    println("serialEvent");
      //    println(PIR+" "+MIC+" "+HUM+" "+TEMP);
    }

    //  int integ = readBytes(2);
    //  int integ2 = readBytes(2);
    //  int integ3 = readBytes(4);
    //  int integ4 = readBytes(4);
    //  println(integ+"   "+integ2+"   "+integ3+"    "+integ4);
    // int integ = port.read();
    // int i0 = port.read();
    // int i1 = port.read();
    // int i2 = port.read();
    // int integ2 = i0<<i1<<i2;
    // String rest = port.readStringUntil('\n');

    // println(integ+"   "+integ2+"  "+rest);
  }
}



void mousePressed() {
  if (mouseX<width/2 && mouseY<tab[0]._height) {
    tab[0].selected = true;
    tab[1].selected = false;
  } 
  if (mouseX>width/2 && mouseY<tab[0]._height) {
    tab[0].selected = false;
    tab[1].selected = true;
  }
  if (tab[0].selected) {
    if (mouseX>=dropdown_x && mouseX<=dropdown_x+dropdown_w && mouseY<=dropdown_y+dropdown_h && mouseY>=dropdown_y) {
      display_serial = !display_serial;
      println("display_serial");
    }
    if (mouseX>=rate_x && mouseX<=rate_x+dropdown_w && mouseY<=dropdown_y+dropdown_h && mouseY>=dropdown_y) {
      display_rate = !display_rate;
      println("display_rate");
    }
    if (mouseX>=LED1.x-LED1._width/2 && mouseX<=LED1.x+LED1._width/2 && mouseY>=LED1.y-LED1._height/2 && mouseY<=LED1.y+LED1._height/2) {
      LED1.on = !LED1.on;
      if(port_id!=100){
        SerialPort[port_id].write(int(LED1.on)+7);        
      }
    }
    if (mouseX>=LED2.x-LED2._width/2 && mouseX<=LED2.x+LED2._width/2 && mouseY>=LED2.y-LED2._height/2 && mouseY<=LED2.y+LED2._height/2) {
      LED2.on = !LED2.on;
      if(port_id!=100){
        SerialPort[port_id].write(int(LED2.on)+9);
      }
    }
  }
  if (mouseX>=xbee_x && mouseX<=xbee_x+xbee_width && mouseY>=xbee_y && mouseY<=xbee_y+xbee_height) {
    xbee_state = !xbee_state;
    println("xbee");
  }
}

class Tab {

  int xpos, _height, _fill;
  boolean selected;
  String title;

  Tab(int x, int h, int tab_hue, boolean bool) {
    xpos = x;
    _height = h;
    _fill = tab_hue;
    selected = bool;
    if (xpos==0) {
      title = "Settings";
    }
    else {
      title = "Visualization";
    }
  }

  void display() {

    if (selected) {
      _fill = 250;
    }
    else {
      _fill = 200;
    }
    fill(100, 150, _fill);
    noStroke();
    rect(xpos, 0, width/2, _height);
    stroke(0);
    line(0, 0, 0, height);
    line(0, 0, width, 0);
    line(width-1, 0, width-1, height);
    line(width/2, 0, width/2, _height-1);
    line(0, height-1, width, height-1);


    if (xpos==0 && selected==false) {
      line(0, _height-1, width/2, _height-1);
    }
    if (xpos==width/2 && selected==false) {
      line(width/2, _height-1, width, _height-1);
    }

    fill(0);
    textSize(20);
    text(title, (2*xpos+width/2)/2-textWidth(title)/2, 30);
  }
}

