/***********************************************************************************
Author: Bradford Henson
Date: 11/3/2016
Version: 0.0.0.3
License: USE AT YOUR OWN RISK

Description: 
------------
TODO

Hardware connections are as followed:
-------------------------------------
TODO

Additional hardware:
--------------------
TODO

*********************************************************************************/
 
 #include <Ultrasonic.h>
 #include <EEPROM.h>
 
 #define TRIGGER_PIN 5
 #define ECHO_PIN 6
 #define RED 7
 #define YELLOW 8
 #define GREEN 9
 #define CAL 3 
 #define TOFAR 10
 #define TOCLOSE 11

 int threshold = 0;
 int currentDistance;
 int redTimeout = 0;
 int yellowTimeout = 0;
 int greenTimeout = 0;
 int tocloseTimeout = 0;
 float cmMsec, inMsec;
 long microsec;

 Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

/*******************************************************************************************************
* 
* 
*                                           ARDUINO SETUP SECTION 
*                                
* 
********************************************************************************************************/
 void setup()
{
 pinMode(RED, OUTPUT);
 pinMode(YELLOW, OUTPUT);
 pinMode(GREEN, OUTPUT);
 pinMode(CAL, INPUT_PULLUP);

 attachInterrupt(1, calibrationButton, FALLING); // Setup the external interrupt for pin 3
 
 // Sets the threshold value equal to the value in EEPROM
 threshold = EEPROM.read(0); 

 flashLeds(); // Flash all LEDs three times
}

/*******************************************************************************************************
* 
* 
*                                           ARDUINO LOOP SECTION
*                                
* 
********************************************************************************************************/
 void loop()
{
  // Inlude the code for taking a sensor sample and converting it to a measurement
  microsec = ultrasonic.timing();
  
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  
  currentDistance = (int)inMsec;
           
  //Determine what zone the car is in               
  if (currentDistance > ( 125 + threshold)) // If a car isn't present, turn off the LEDs
  {
   digitalWrite(GREEN, LOW);
   digitalWrite(YELLOW, LOW);
   digitalWrite(RED, LOW);
  }      
  
  // The car has been picked up by the sensor, but can keep driving into the garage
  else if (currentDistance <= (124 + threshold) && currentDistance > (threshold + 40))
  {
   currentZone(GREEN);
  }
  
  //The car should start slowing down, it is about where it should be 
  else if (currentDistance <= (39 + threshold) && currentDistance > (threshold + 4))
  {
   currentZone(YELLOW);   
  }
  
  //The car is right where it is programmed to be + or - 3
  else if (currentDistance <= (threshold + 3) && currentDistance > (threshold - 3))
  {
   currentZone(RED);
  }
  
  //The car went to far, let the driver know to backup a little
  else if (currentDistance <= (threshold - 4))
  {  
   currentZone(TOCLOSE);
  }   

  delay(50);
  
}
/*******************************************************************************************************
* 
* 
*                                         FUNCTION SECTION
*                                
* 
********************************************************************************************************/
/*******************************************************************************************************
* Function Name: flashLeds()
* 
* Author: Bradford Henson
*  
* Description: TODO
*******************************************************************************************************/
 void flashLeds()
 {
   for(int i = 0; i < 6; i++) // When the system first resets it should flash the leds
   {
     digitalWrite(RED, HIGH);
     digitalWrite(YELLOW, HIGH);
     digitalWrite(GREEN, HIGH);

     delay(100);

     digitalWrite(RED, LOW);
     digitalWrite(YELLOW, LOW);
     digitalWrite(GREEN, LOW);

     delay(100);
    }
 }
/*******************************************************************************************************
* Function Name: calibrationButton()
* 
* Author: Bradford Henson
* 
* Description: TODO
*******************************************************************************************************/
 void calibrationButton()
 {
   // Earse the current value in the EEPROM
   EEPROM.write(0, 0);
     
   // Write the current distance measurement to the first byte in EEPROM
   EEPROM.write(0, currentDistance);  

   // Sets the threshold value equal to the value in EEPROM
   threshold = EEPROM.read(0); 
   
   flashLeds();
 }
 /*******************************************************************************************************
* Function Name: timeOut()
* 
* Author: Bradford Henson
* 
* Description: TODO
*******************************************************************************************************/
 int currentZone(int zone)
 { 
  switch (zone)
  {
    case 7:
     greenTimeout = 0;
     yellowTimeout = 0;
     redTimeout += 1;
     tocloseTimeout = 0;
     if (redTimeout < 1000)
     {
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
     }
     else
     {
      digitalWrite(RED, LOW);
     }
     break;
    case 8:
     greenTimeout = 0;
     yellowTimeout += 1;
     redTimeout = 0;
     tocloseTimeout = 0;
     if (yellowTimeout < 1000)
     {
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, LOW);
     }
     else
     {
      digitalWrite(YELLOW, LOW);
     }
     break;
    case 9:
     greenTimeout += 1;
     yellowTimeout = 0;
     redTimeout = 0;
     tocloseTimeout = 0;
     if (greenTimeout < 1000)
     {
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
     }
     else
     {
      digitalWrite(GREEN, LOW);
     }
     break;
    case 11:
     greenTimeout = 0;
     yellowTimeout = 0;
     redTimeout = 0;
     tocloseTimeout += 1;
     if (tocloseTimeout < 1000)
     {
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, HIGH);
     }
     else
     {
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
     }
     break;
  }   
 }
