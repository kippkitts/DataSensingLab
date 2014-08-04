char GetComm(void)
{
  char header;
  char temp_id;
  char command;
  
  if(Serial.available() > 2)
  {
    header = Serial.read();  // read in the first byte of the 3 bytes and 
    
    // check to see if it is the ':'
    if(header == ':')
    {
      temp_id = Serial.read();      // grab the id
      command = Serial.read();  // grab the command
      
      if(temp_id == my_id)  // if the command is for me, act on it
      {
        digitalWrite(P2_1, HIGH);   // turn the ACT LED on to let us know the MSP430 is alive
        return command;    // pass the command up to the case structure
      }
      else if((command == 8) || (command == 9))
      {
        digitalWrite(P2_1, HIGH);   // turn the ACT LED on to let us know the MSP430 is alive
        return command;    // pass the command up to the case structure
      }
      else
      {
      digitalWrite(P2_1, LOW);   // turn the ACT LED on to let us know the MSP430 is alive
//      Serial.flush();  // Try clearing out any data that doesn't begin with the proper character
      }
    }
    else
    {
      digitalWrite(P2_1, LOW);   // turn the ACT LED on to let us know the MSP430 is alive
    }
  } 
  
  return 0;
}

