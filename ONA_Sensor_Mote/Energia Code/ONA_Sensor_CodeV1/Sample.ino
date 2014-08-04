
//*****************************************************
//*****************************************************
void Sample_No_AQ(void)
{
  digitalWrite(P2_1, LOW);   // turn the ACT LED off to let us know the MSP430 is sampling

  therm_value = analogRead(A6); // 
  Serial.print("TEMP = ");
  Serial.print(therm_value);
  Serial.print("\t");

  delay(1);

  mic_value = analogRead(A7); // 
  Serial.print("MIC = ");
  Serial.print(mic_value);
  Serial.print("\t");

  delay(1);
  
  rf_value = analogRead(A5); // 
  Serial.print("RF = ");
  Serial.print(rf_value);
  Serial.print("\t");

  delay(1);

  aq_value = analogRead(A4); // 
  Serial.print("AQ = ");
  Serial.print("OFF");
  Serial.print("\t");
  
  delay(1);
/*  pir_val = digitalRead(P1_3);
  if (pir_val)
  {
    digitalWrite(P2_2, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial.print("\t Motion Is Detected!! \t");
  }
  else
  {
    digitalWrite(P2_2, LOW);    // turn the LED off by making the voltage LOW
  }
*/ 
  Serial.print("MOT = ");
  Serial.print(motion_count);
  Serial.print("\n");

//  pir_val = digitalRead(P1_3);
//  if (pir_val)
//  {
    digitalWrite(P2_2, LOW);   // turn the LED off by making the voltage LOW
//  }

  delay(100);
  digitalWrite(P2_1, HIGH);   // turn the ACT LED on to let us know the MSP430 is alive

  motion_count = 0;  // clear the motion counter
  //Sleep();

}

//*****************************************************
//*****************************************************
void Sample_AQ(void)
{
  digitalWrite(P2_1, LOW);   // turn the ACT LED off to let us know the MSP430 is sampling

  therm_value = analogRead(A6); // 
  Serial.print("TEMP = ");
  Serial.print(therm_value);
  Serial.print("\t");

  delay(1);

  mic_value = analogRead(A7); // 
  Serial.print("MIC = ");
  Serial.print(mic_value);
  Serial.print("\t");

  delay(1);
  
  rf_value = analogRead(A5); // 
  Serial.print("RF = ");
  Serial.print(rf_value);
  Serial.print("\t");

  delay(1);

  aq_value = analogRead(A4); // 
  Serial.print("AQ = ");
  Serial.print(aq_value);
  Serial.print("\t");
  
  delay(1);
/*  pir_val = digitalRead(P1_3);
  if (pir_val)
  {
    digitalWrite(P2_2, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial.print("\t Motion Is Detected!! \t");
  }
  else
  {
    digitalWrite(P2_2, LOW);    // turn the LED off by making the voltage LOW
  }
*/ 
  Serial.print("MOT =");
  Serial.print(motion_count);
  Serial.print("\n");

//  pir_val = digitalRead(P1_3);
//  if (pir_val)
//  {
    digitalWrite(P2_2, LOW);   // turn the LED off by making the voltage LOW
//  }

  delay(100);
  digitalWrite(P2_1, HIGH);   // turn the ACT LED on to let us know the MSP430 is alive

  motion_count = 0;  // clear the motion counter
  //Sleep();

}

//*****************************************************
//*****************************************************
void AQ_on(void)
{
  digitalWrite(P2_3, HIGH);  // turn on the AQ sensor
}


//*****************************************************
//*****************************************************
void AQ_off(void)
{
  digitalWrite(P2_3, LOW);  // turn off the AQ sensor
}

