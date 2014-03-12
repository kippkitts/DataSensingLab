DSL Sensor Mote Workshop Board
===========================================

<img src="https://raw.github.com/kippkitts/kippkitts.github.io/master/Images/DataSensingLab_workshop.JPG" width="300px" height="200px"/>

Arduino and Processing code, as well as PCB files, for the DataSensingLab workshops are located here. 

#Hardware

The DataSensingLab Sensor Mote Workshop Board is a shield for the Arduino Leonardo with an onboard electret microphone amplifier, temperature and humidity sensor, PIR motion detector, and XBee transceiver.  It transmits the data over USB or wirelessly over an XBee network.

The files provided here are upgraded versions of the board designed for the book "[Distributed Network Data](http://shop.oreilly.com/product/0636920028802.do)"

#Software

Two versions of the Arduino code are provided.  Version 2 simply transmits the data over USB and the XBee.  Version 3 interfaces with a data visualization Processing sketch, which also provides graphical controls for the XBee, onboard LEDs, and sampling rate.  
