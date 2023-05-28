/*  This example code focuses on how to control a servo. This is designed to help you get started, but
 *  will require you to combine your own functions and control logic from the different examples.
 *
 * 
 *  The final project code relies on three separate libraries.
 * 
 *  The PS2 Shield library is design by the makers of the board, Cytron, to simplify the process
 *  of reading the data from the PS2 controller. It works for wireless or wired controllers.
 *  It allows you to read the joystick values or know if a button has been pressed.
 *  
 *  The ServoTimer2 library is very similar to the standard Servo library. However its key difference
 *  is how it uses a different part of the Arduino Uno hardware to control the servo. The output is
 *  the same, but this servo library does not cause issues when used with the PS2 Shield. 
 *  Specifically, Arduino Uno's have three hardware timers. These timers are internal counters
 *  that are used for specific functions. Timer0 is used by the millis function to know how long
 *  the program has been running. The standard Servo library uses Timer1, but this caused issues
 *  with the PS2 Shield, so this library solved that problem since it uses Timer2.
 *  
 *  An Arduino communicates with your computer using a communication protocol known as Serial.
 *  The PS2 Shield is design to communicate using the same system, but an Arduino UNO only has
 *  one set of hardware for this purpose. Thankfully, you can using software to make the Arduino
 *  "think" it has more ways to communicate using the Serial protocol. AltSoftSerial is one of many
 *  libraries is designed to do this.
 *  
 *  On the PS2 Shield, you can select which pins you use for the serial communication between the 
 *  Arduino board and the shield. Pay attention to which pins are selected to ensure you do not use
 *  these for other items.a
 *  
 *  The DF Robot Romeo BLE board functions just like an Arduino Uno but has some things built in.
 *  Specifically it has a motor controller and some buttons built in. For this project, it is important
 *  to know that the motor controller is connected to pins 4, 5, 6, and 7 so these cannot be used for
 *  other items.
 */

#include <ServoTimer2.h>
#include <Cytron_PS2_Shield.h>
#include <AltSoftSerial.h>

Cytron_PS2_Shield PS2;  // Create an object called "PS2" for referencing the PS2 Shield library
AltSoftSerial Serial2; // Create an object called "Serial2" for the AltSoftSerial library

const int E1 = 5;     //Digital pin 5 controls the speed of motor 1
const int E2 = 6;     //Digital pin 6 controls the speed of motor 2
const int M1 = 4;     //Digital pin 4 controls the direction of motor 1
const int M2 = 7;     //Digital pin 7 controls the direction of motor 2


void setup()
{
   // Starts the software serial communication at a specific baud rate
  Serial2.begin(9600);
  
  // Connects the PS2 object to the desired serial communication
  PS2.AttachPS2Serial(&Serial2);

  // Declare output or input for any used pins
  pinMode(E1, OUTPUT); 
  pinMode(E2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  // Delay to pause the program to ensure the user is ready to provide input on the controller
  delay(2000);

}

void loop()
{
  int yAxisRight;

  
  PS2.SetJoystick(RIGHT_JOYSTICK); 

  
  PS2.UpdateData();

  
  yAxisRight = PS2.GetY_Axis_Raw();

  // Check the value of the joystick against the ranges for forward/backward
  if (yAxisRight >145){ 
    
  int rightMap;
  
  rightMap = map(yAxisRight, 140, 254, 0, 175);  // Map the value from the joystick to the range of values for AnalogWrite
  
  analogWrite(E1, rightMap);      
  digitalWrite(M1, HIGH);      //This sets the direction of motor 1. You can set it to either HIGH or LOW
  analogWrite(E2, rightMap);      
  digitalWrite(M2, HIGH); 
    }
  else if (yAxisRight < 110){  
  
  int rightMap;
  rightMap = map(yAxisRight, 1, 120, 255, 0);
  
  analogWrite(E1, rightMap);
  digitalWrite(M1, LOW);
  analogWrite(E2, rightMap);      
  digitalWrite(E2, LOW); 
  }


  else {
   analogWrite (E1, 0); // Motor velocity set to zero otherwise
   analogWrite (E2, 0);
  }
  
  
}