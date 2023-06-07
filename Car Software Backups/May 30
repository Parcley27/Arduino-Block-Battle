#include <ServoTimer2.h>
#include <Cytron_PS2_Shield.h>
#include <AltSoftSerial.h>

Cytron_PS2_Shield PS2;  // Create an object called "PS2" for referencing the PS2 Shield library
AltSoftSerial Serial2; // Create an object called "Serial2" for the AltSoftSerial library

const int rightSpeedPin = 5;     //Digital pin 5 controls the speed of motor 1
const int rightDirectionPin = 4;     //Digital pin 4 controls the direction of motor 1

const int leftSpeedPin = 6;     //Digital pin 6 controls the speed of motor 2
const int leftDirectionPin = 7;     //Digital pin 7 controls the direction of motor 2

const int forwards = HIGH;
const int backwards = LOW;

//sets turn strength
const int turnStrength = 50;

void setup() {
  //Start hard serial comm. at baud
  Serial.begin(9600);

  // Starts the software serial communication at a specific baud rate
  Serial2.begin(9600);
  
  // Connects the PS2 object to the desired serial communication
  PS2.AttachPS2Serial(&Serial2);

  // Declare output or input for any used pins
  pinMode(rightSpeedPin, OUTPUT); 
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightDirectionPin, OUTPUT);
  pinMode(leftDirectionPin, OUTPUT);
  
  // Delay to pause the program to ensure the user is ready to provide input on the controller
  delay(2000);

}

void loop() {

  PS2.SetJoystick(LEFT_JOYSTICK); 
  PS2.UpdateData();

  int yAxisLeft = PS2.GetY_Axis_Raw();
  int xAxisLeft = PS2.GetX_Axis_Raw();

  int motorSpeed = map(yAxisLeft, 0, 255, -100, 100);
  int turnSpeed = map(xAxisLeft, 0, 255, -100, 100);  

  int leftJoystickYPosition = map(yAxisLeft, 0, 255, -100, 100);
  
  turnSpeed = turnSpeed * turnStrength / 100;

  int leftMotorSpeed = motorSpeed - turnSpeed;
  int rightMotorSpeed = motorSpeed + turnSpeed;

  //when is less than 0, sets to 0 so motors don't turn
  //abs makes ""MotorSpeed positive regardless

  leftMotorSpeed = constrain(abs(leftMotorSpeed), 0, 175);
  rightMotorSpeed = constrain(abs(rightMotorSpeed), 0, 175);

  //Serial.println(rightMotorSpeed);
  //Serial.println(motorSpeed);
  //Serial.println(leftJoystickYPosition);

  if (leftJoystickYPosition < -10) { //backwards
    analogWrite(rightSpeedPin, rightMotorSpeed);
    digitalWrite(rightDirectionPin, backwards);

    analogWrite(leftSpeedPin, leftMotorSpeed);
    digitalWrite(leftDirectionPin, backwards);

    Serial.println("forwards");

  } else if (leftJoystickYPosition > 10) { //forwards
    analogWrite(rightSpeedPin, rightMotorSpeed);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, leftMotorSpeed);
    digitalWrite(leftDirectionPin, forwards);

    Serial.println("backwards");
  } else {
    analogWrite(rightSpeedPin, 0);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, 0);
    digitalWrite(leftDirectionPin, forwards);
  }
}
