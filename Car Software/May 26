#include <ServoTimer2.h>
#include <Cytron_PS2_Shield.h>
#include <AltSoftSerial.h>

Cytron_PS2_Shield PS2;  // Create an object called "PS2" for referencing the PS2 Shield library
AltSoftSerial Serial2; // Create an object called "Serial2" for the AltSoftSerial library

/*

E1 = rightSpeed
E2 = leftSpeed
M1 = rightDirection
M2 = leftDirection

256/2 = 128
1 - 120, 136 = 256

*/

const int rightSpeed = 5;     //Digital pin 5 controls the speed of motor 1
const int rightDirection = 4;     //Digital pin 4 controls the direction of motor 1

const int leftSpeed = 6;     //Digital pin 6 controls the speed of motor 2
const int leftDirection = 7;     //Digital pin 7 controls the direction of motor 2


void setup() {
  //Start hard serial comm. at baud
  Serial.begin(9600);

  // Starts the software serial communication at a specific baud rate
  Serial2.begin(9600);
  
  // Connects the PS2 object to the desired serial communication
  PS2.AttachPS2Serial(&Serial2);

  // Declare output or input for any used pins
  pinMode(rightSpeed, OUTPUT); 
  pinMode(leftSpeed, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  
  // Delay to pause the program to ensure the user is ready to provide input on the controller
  delay(2000);

}

void loop() {
  int yAxisLeft;

  PS2.SetJoystick(LEFT_JOYSTICK); 
  
  PS2.UpdateData();

  yAxisLeft = PS2.GetY_Axis_Raw();

  // Check the value of the joystick against the ranges for forward/backward
  if (yAxisLeft > 136) { 
    
  int leftMap;
  
  leftMap = map(yAxisLeft, 136, 256, 0, 175);  // Map the value from the joystick to the range of values for AnalogWrite
  
  analogWrite(rightSpeed, leftMap);      
  digitalWrite(rightDirection, HIGH);      //This sets the direction of motor 1. You can set it to either HIGH or LOW
  //analogWrite(leftSpeed, leftMap);      
  //digitalWrite(leftDirection, HIGH); 

  }

  else if (yAxisLeft < 120) {  
    int leftMap;

    //map reverse because motors are backwards
    leftMap = map(yAxisLeft, 1, 120, 175, 0);
  
    analogWrite(rightSpeed, leftMap);
    digitalWrite(rightDirection, LOW);
    analogWrite(leftSpeed, leftMap);      
    digitalWrite(leftSpeed, LOW); 

  }


  else {
    analogWrite (rightSpeed, 0); // Motor velocity set to zero otherwise
    analogWrite (leftSpeed, 0);
  }
}
