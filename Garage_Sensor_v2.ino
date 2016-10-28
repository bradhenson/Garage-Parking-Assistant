 #include <Ultrasonic.h>
 #include <EEPROM.h>
 #define TRIGGER_PIN 5
 #define ECHO_PIN 6
 const int red = 4; // Red bank of LEDs
 const int yellow = 3; // Yellow bank of LEDs
 const int green = 2; // Green bank of LEDs
 const int calibration = 11;  // Calibration button
 int threshold = 0;
 int redcount = 0;
 int yellowcount = 0;
 int greencount = 0;
 int toclosecount = 0;
 int tofarcount = 0;
  
 Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
 //testing out github version control
 /******************************************************************************************
  * Setup Area
  ******************************************************************************************/
 void setup()
{
  Serial.begin(9600);
 // I don't set the ref pin and the analogread pin because 
 // Arduino automatically sets them
 pinMode(red, OUTPUT);
 pinMode(yellow, OUTPUT);
 pinMode(green, OUTPUT);
 pinMode(calibration, OUTPUT);
 digitalWrite(calibration, HIGH);
 
 // When the device gets reset, it needs to reset the total counts
 int redcount = 0;
 int yellowcount = 0;
 int greencount = 0;
 int toclosecount = 0;
 int tofarcount = 0;
 
 // Sets the threshold value equal to the value in EEPROM
    threshold = EEPROM.read(0); 
 
 for(int i = 0; i < 6; i++) // When the system first resets it should flash the leds
   {
     digitalWrite(red, HIGH);
     digitalWrite(yellow, HIGH);
     digitalWrite(green, HIGH);

     delay(100);

     digitalWrite(red, LOW);
     digitalWrite(yellow, LOW);
     digitalWrite(green, LOW);

     delay(100);
    }
}

 /******************************************************************************************
  * Run Loop Starts
  ******************************************************************************************/
 void loop()
{
  // Inlude the code for taking a sensor sample and converting it to a measurement
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  
  int currentDistance = (int)inMsec;
    
  /******************************************************************************************
  * Calibration Button
  ******************************************************************************************/
  // Execute the calibration function
  if (analogRead(5) > 800)
   {
    // Earse the current value in the EEPROM
    EEPROM.write(0, 0);
     
    // Write the current distance measurement to the first byte in EEPROM
    EEPROM.write(0, currentDistance);  
    
  // Send the program back to the setup function to start the system over,
  // the flashing at the end of setup will let you know that the calibration when okay
   setup();
   }
        
  /******************************************************************************************
  * Determine what zone the car is in
  ******************************************************************************************/  
  else 
   {              
     if (currentDistance > ( 125 + threshold) && tofarcount < 6000)
          {
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, LOW);
          tofarcount += 1;
          greencount = 0;
          yellowcount = 0;
          redcount = 0;
          toclosecount = 0;
          tofarcount = 0;
        }   
     
     if (currentDistance <= (124 + threshold) && currentDistance > (threshold + 40) && greencount < 1000)
          {
          digitalWrite(green, HIGH);
          digitalWrite(yellow, LOW);
          digitalWrite(red, LOW);
          greencount += 1;
          yellowcount = 0;
          redcount = 0;
          toclosecount = 0;
          tofarcount = 0;
        }
        if (currentDistance <= (39 + threshold) && currentDistance > (threshold + 4) && yellowcount < 1000)
          {
          digitalWrite(green, LOW);
          digitalWrite(yellow, HIGH);
          digitalWrite(red, LOW);  
          yellowcount += 1;
          greencount = 0;
          redcount = 0;
          toclosecount = 0;
          tofarcount = 0;    
          }
        if (currentDistance <= (threshold + 3) && currentDistance > (threshold - 3) && redcount < 1000)
        {
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, HIGH);
          redcount += 1;
          yellowcount = 0;
          greencount = 0;
          toclosecount = 0;
          tofarcount = 0;
        }
        if (currentDistance <= (threshold - 4) && toclosecount < 1000)
        {  
          digitalWrite(green, HIGH);
          digitalWrite(yellow, HIGH);
          digitalWrite(red, HIGH);
          toclosecount += 1; 
          greencount = 0;
          yellowcount = 0;
          redcount = 0;
          tofarcount = 0;         
        } 
   
     if (greencount == 1000){
       digitalWrite(green, LOW);
       digitalWrite(yellow, LOW);
       digitalWrite(red, LOW);
    }
    if (yellowcount == 1000){
       digitalWrite(green, LOW);
       digitalWrite(yellow, LOW);
       digitalWrite(red, LOW);
    }
    if (redcount == 1000){
       digitalWrite(green, LOW);
       digitalWrite(yellow, LOW);
       digitalWrite(red, LOW); 
    }
    if (toclosecount == 1000){
       digitalWrite(green, LOW);
       digitalWrite(yellow, LOW);
       digitalWrite(red, LOW);   
    }
   } 
  delay(50);
  
}
