

void vis1() {

  //set x and y coordinates of menu shift button
  //display triangular button facing to the left

  //if in first menu
  smooth();

  //  for(int i=0; i<port_length; i++){
  //    dropdown_serial[i].display();
  //  }

  mic_volume();
  motion_detect();

  noSmooth();

  fill(120, 120, 250);
  triangle(width-50, height/2, width-20, 30+height/2, width-20, height/2-30);

  if (mousePressed && mouseX>width-50 && mouseX<width-20 && mouseY<height/2+30 && mouseY>height/2-30) {
    vis1_selected = false;
  }
  //draw triangle button
  //check if mousepressed
  //if so, set vis1_selected = false;
}

void vis2() {

  //draw triangular button facing to the right and located on left side
  //have select range option for charts
  temp_chart.display();
  humidity_chart.display();

  fill(120, 120, 250);
  triangle(50, height/2, 20, 30+height/2, 20, height/2-30);

  if (mousePressed && mouseX>20 && mouseX<50 && mouseY<height/2+30 && mouseY>height/2-30) {
    vis1_selected = true;
  }

  //draw triangle button
  //check if mousepressed
  //if so, set vis1_selected = true;
}
/*
void vis3(){
 // Serial Port dropdown menu
 
 port = new Serial(this, Serial.list()[0], 9600);
 
 
 }
 */
void mic_volume() {

  //shift to left, add table with current decibel value, max value, min value

    fill(0);
  textSize(15);
  text("Microphone Volume", 185, 120);

  fill(150);
  //  triangle(150, 170, 300, 140, 300, 200);
  triangle(175, 170, 325, 140, 325, 200);
  // rect(50, 170, MIC-500, 50);
  int val = 120;

  for (int i=0; i<MIC; i++) {
    stroke(50, 50+map(i, 0, 1023, 0, 200), 50);
    //    line(155+map(i, 0, 1023, 0, 150), 180-(0.2)*(55+map(i, 0, 1023, 0, 150))+2, 155+map(i, 0, 1023, 0, 150), (0.2)*(55+map(i, 0, 1023, 0, 150))+160-1); 
    line(180+map(i, 0, 1023, 0, 150), 180-(0.2)*(55+map(i, 0, 1023, 0, 150))+2, 180+map(i, 0, 1023, 0, 150), (0.2)*(55+map(i, 0, 1023, 0, 150))+160-1); 
    stroke(0);
  }
}

void motion_detect() {

  textSize(15);
  fill(0);
  //  text("Motion Detector",0.75*width-55,height/3-40);
  text("Motion Detector", width/2-50, 0.75*height-45);

  if (PIR==0) {
    fill(150);
  }
  if (PIR==1) {
    fill(0, 250, 0);
  }
  //  ellipse(0.75*width, height/3, 50, 50);
  rect(100, 0.75*height-25, width-200, 50);
}
//
//void temperature() {
//  
//  fill(0);
//  textSize(15);
//  text("Temperature",120,height/2+30);
//  text("40⁰C", 175, height/2+53);
//  text("20⁰C", 175, height/2+203);
//  
//  fill(150);
//  rect(150, height/2+50, 20, 150);
//  for (int i=0; i<TEMP*10-200; i++) {
//    stroke(2*i, 0, 150-2*i);
//    line(151, height/2+200-i, 169, height/2+200-i);
//    stroke(0);
//  }
//}

//void humidity() {
//  
//  textSize(15);
//  fill(0);  
//  text("Humidity",width/2+90,height/2+30);
//  text(HUM,width/2+95,height/2+125);
//  text("%",width/2+155,height/2+125);
//  
//  for (int i=0; i<HUM*100-4000; i++) {
//    stroke(0, 0, 250);
//    //point(0.75*width+random(-HUM,HUM), 0.67*height+random(-HUM,HUM));
//    point(width/2+50+random(0, 150), height/2+50+random(0, 150));
//    stroke(0);
//  }
//}



class Chart {

  int _width, _height, xpos, ypos, min_value, max_value;
  float[] data = new float[8];
  String chart_type;

  Chart(int chart_width, int chart_height, int x, int y, String chart_string) {
    _width = chart_width;
    _height = chart_height;
    xpos = x;
    ypos = y;
    chart_type = chart_string;
    for (int i=0; i<data.length; i++) {
      data[i] = 25.0;
    }

    if (chart_type.equals("Temperature")) {
      min_value = 5;
      max_value = 35;
    }
    if (chart_type.equals("Humidity")) {
      min_value = 0;
      max_value = 100;
    }
  } 

  void display() {
    //draw chart background
    fill(0);
    rect(xpos, ypos, _width, _height);
    
    fill(0);
    textSize(15);
    stroke(0);
    
    text(max_value, xpos-30, ypos);
    text(min_value, xpos-20, ypos+_height);
    
    if(chart_type.equals("Temperature")){
      text(chart_type+" ⁰C", xpos+140, ypos+_height+20);
    }else{
      text(chart_type+" %", xpos+140, ypos+_height+20);
    }

    //draw color gradient
    for (int i=0; i<150; i++) {
      if (chart_type.equals("Temperature")) {
        stroke(0.75*i, 0, 100-i);
      }
      if (chart_type.equals("Humidity")) {       
        stroke(200-0.75*i, 200-0.75*i, 255);
      }
      line(xpos, ypos+_height-i, xpos+_width, ypos+_height-i);
    }

    //fill negative space
    fill(0);
    stroke(0);
    beginShape();
    vertex(xpos, ypos);

    if (chart_type.equals("Temperature")) {
      for (int i=0; i<data.length; i++) {
        vertex(xpos+i*_width/(data.length-1), ypos+_height-map(data[i], min_value, max_value, 0, _height)); //5 to 35 celsius range
      }
    }
    if (chart_type.equals("Humidity")) {
      for (int i=0; i<data.length; i++) {
        vertex(xpos+i*_width/(data.length-1), ypos+_height-map(data[i], min_value, max_value, 0, _height)); //5 to 35 celsius range
      }
    }
    vertex(xpos+_width, ypos);
    endShape(CLOSE);

    textSize(15);
    stroke(0);

    if (chart_type.equals("Temperature")) {
      text(data[data.length-1], xpos+_width+3, ypos+_height-map(data[data.length-1], min_value, max_value, 0, _height));
    }
    if (chart_type.equals("Humidity")) {
      text(data[data.length-1], xpos+_width+3, ypos+_height-map(data[data.length-1], min_value, max_value, 0, _height));
    }

    //draw markers
    fill(200, 100, 0);
    for (int i=0; i<data.length; i++) {
      if (chart_type.equals("Temperature")) {
        ellipse(xpos+i*_width/(data.length-1), ypos+_height-map(data[i], min_value, max_value, 0, _height), 5, 5);
      }
      if (chart_type.equals("Humidity")) {
        ellipse(xpos+i*_width/(data.length-1), ypos+_height-map(data[i], min_value, max_value, 0, _height), 5, 5);
      }
    }
  }
}



/*
class DropDown{
 
 int xpos, ypos, _width, _height;
 String port;
 
 DropDown(int x, int y, int item_width, int item_height, String port_name){
 xpos = x;
 ypos = y;
 _width = item_width;
 _height = item_height;
 port = port_name;
 }
 
 void display(){
 fill(255);
 rect(xpos, ypos, _width, _height);
 textSize(20);
 fill(0);
 text(port,xpos+5,ypos+22);   
 }
 
 }
 */











