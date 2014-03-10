
void Settings() {
  PortMenu();
  DataRate();
  XbeeControl();
  LEDs();
  fill(80, 130, 230);
  textSize(60);
  String Logo = "DataSensingLab";
  float LogoSize = textWidth(Logo);
  text("DataSensingLab",width/2-LogoSize/2, height-70);
  //change led behavior
  //direct control of led's
  //different ways of responding to sensors
  //change comm rate (delay)
  //enable/disable xbee
}

void XbeeControl() {
  // smooth();
  // fill(0,0,200);
  // ellipse(100, 0.75*height, 75, 75);
  // fill(0);
  // ellipse(300, 0.75*height, 75, 75);
  // noFill();
  // for(int i=0; i<50; i++){
  //   stroke(0,0,250-4.25*i);
  //   ellipse(100,0.75*height,1.5*i,1.5*i);
  //   stroke(0,0,4.25*i);
  //   ellipse(300,0.75*height, 1.5*i, 1.5*i);
  // }
   

//  if (mousePressed && mouseX>=xpos && mouseX<=xpos+_width && mouseY>=ypos && mouseY<=ypos+_height) {
//    xbee_state = !xbee_state;
//    
//  }
  
  int xbee_switch;
  String xbee_state_str;
  if (xbee_state) {
    fill(0, 255, 0);
    xbee_switch = xbee_width/2;
    xbee_state_str = "ON";
  }
  else {
    fill(255, 0, 0);
    xbee_switch = -10;
    xbee_state_str = "OFF";
  }

  rect(xbee_x, xbee_y, xbee_width, xbee_height);
  fill(150);
  rect(xbee_x+xbee_switch, xbee_y+1, xbee_width/2+10, xbee_height-2,20);
  
  fill(0);
  text("Xbee "+xbee_state_str, xbee_x, xbee_y-12);



  /*
 if(!keyPressed){
   smooth();
   fill(0,0,200);
   ellipse(135, 0.68*height, 100, 100);
   noFill();
   for(int i=0; i<100; i++){
   stroke(0,0,250-2*i);
   ellipse(135,0.68*height,i,i);
   }
   }else{
   
   smooth();
   fill(0);
   stroke(0,0,250);
   ellipse(135, 0.68*height, 100, 100);
   noFill();
   for(int i=0; i<100; i++){
   stroke(0,0,2*i);
   ellipse(135,0.68*height, i, i);
   }
   }
   */
}

void PortMenu() {
  fill(255);
  rect(dropdown_x, dropdown_y, dropdown_w, dropdown_h);
  textSize(dropdown_serial[0].text_size);
  fill(0);
  text("Select Serial Port", dropdown_x+5, dropdown_y-dropdown_serial[0].text_size);
  text(selected_port, dropdown_x+5, dropdown_y+dropdown_serial[0].text_size+2);
  if (!display_serial) {
    for (int i=0; i<port_length; i++) {
      dropdown_serial[i].display();
    }
  }
}

void DataRate() {
  fill(255);
  rect(rate_x, dropdown_y, dropdown_w, dropdown_h);
  textSize(dropdown_rate[0].text_size);
  fill(0);
  text("Select Comm Rate (ms)", rate_x, dropdown_y-dropdown_rate[0].text_size);
  text(selected_rate, rate_x+5, dropdown_y+dropdown_rate[0].text_size+2);
  if (!display_rate) {
    for (int i=0; i<5; i++) {
      dropdown_rate[i].display();
    }
  }
}

void LEDs(){
 fill(0);
 text("Status LED", LED1.x-30, LED1.y-30); 
 text("Power LED", LED2.x-30, LED2.y-30); 
 LED1.display();
 LED2.display();
}

class LED {
  
  boolean on;
  int x, y, _width, _height;
  
  LED(boolean state, int xpos, int ypos){
    on = state;
    x = xpos;
    y = ypos;
    _width = 50;
    _height = 50;
  }
  
  void display(){
    fill(75+int(on)*180,75,75);
    smooth();
    ellipse(x, y, _width, _height);
  } 
  
}

class DropDown {

  int xpos, ypos, _width, _height, id;
  String item;
  float text_size;

  DropDown(int x, int y, int item_width, int item_height, float textsize, String item_name, int id_num) {
    xpos = x;
    ypos = y;
    _width = item_width;
    _height = item_height;
    item = item_name;
    text_size = textsize;  
    id = id_num;
  }

  void display() {
    if (mouseX>xpos && mouseX<xpos+_width && mouseY>ypos && mouseY<ypos+_height) {
      fill(100);
      if (mousePressed) {        
        if (id<port_length) {
          display_serial = !display_serial;
          if (selected_port != item) {
            selected_port = item;
            port_id = id;
            initialize();
          }
        }
        else {          
          selected_rate = id; 
          display_rate = !display_rate;
        }
      }
    }
    else {
      fill(255);
    }
    rect(xpos, ypos, _width, _height);
    textSize(text_size);
    fill(0);
    text(item, xpos+5, ypos+text_size+2);
  }

  void initialize() {
    for (int i=0; i<temp_chart.data.length; i++) {
      temp_chart.data[i] = 25.0; 
      humidity_chart.data[i] = 25.0;
    }
  }
}


