#include <ServoTimer2.h>
#include <Cytron_PS2_Shield.h>
#include <AltSoftSerial.h>

// Create a "PS2" object for the Cryton_PS2_Shield library
Cytron_PS2_Shield PS2;

// Create a "Serial2" object for the AltSoftSerial library
AltSoftSerial Serial2;

// Define Left and Right Motor Speed Control pins
const int leftSpeedPin = 6;
const int rightSpeedPin = 5;

// Define Left and Right Motor Direction Control pins
const int rightDirectionPin = 4;
const int leftDirectionPin = 7;

// Use forwards and backwards instead of high and low
const int forwards = HIGH;
const int backwards = LOW;

// Define turn strength controller
const int turnStrength = 50;

void setup() {
  // Start hardware serial communication at given baud rate
  Serial.begin(9600);

  // Starts software serial communication at given baud rate
  Serial2.begin(9600);
  
  // Connect PS2 object to the desired serial communication
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

  // Refresh PS2 controller data
  PS2.UpdateData();

  // ---------- DRIVE ----------

  // Set joystick to the left one for now
  PS2.SetJoystick(LEFT_JOYSTICK); 

  // Get Left Joystick X and Y location
  int yAxisLeft = PS2.GetY_Axis_Raw();
  int xAxisLeft = PS2.GetX_Axis_Raw();

  // Remap to within new ranges
  int motorSpeed = map(yAxisLeft, 0, 255, -200, 200);
  int turnSpeed = map(xAxisLeft, 0, 255, -100, 100);  

  int leftJoystickYPosition = map(yAxisLeft, 0, 255, -100, 100);
  
  // Motor speed affector algorithm
  turnSpeed = turnSpeed * turnStrength / 100;

  int leftMotorSpeed = motorSpeed - turnSpeed;
  int rightMotorSpeed = motorSpeed + turnSpeed;

  // When is less than 0, sets to 0 so motors don't turn
  // Abs makes ""MotorSpeed positive regardless

  leftMotorSpeed = constrain(abs(leftMotorSpeed), 0, 255);
  rightMotorSpeed = constrain(abs(rightMotorSpeed), 0, 255);

  //Serial.println(rightMotorSpeed);
  //Serial.println(motorSpeed);
  //Serial.println(leftJoystickYPosition);

  // If user wants to go backwards
  if (leftJoystickYPosition < -10) {
    analogWrite(rightSpeedPin, rightMotorSpeed);
    digitalWrite(rightDirectionPin, backwards);

    analogWrite(leftSpeedPin, leftMotorSpeed);
    digitalWrite(leftDirectionPin, backwards);

    Serial.println("forwards");

  // Else if user wants to go forwards
  } else if (leftJoystickYPosition > 10) { //forwards
    analogWrite(rightSpeedPin, rightMotorSpeed);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, leftMotorSpeed);
    digitalWrite(leftDirectionPin, forwards);

    Serial.println("backwards");

  // Else (In deadzone)
  } else {
    analogWrite(rightSpeedPin, 0);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, 0);
    digitalWrite(leftDirectionPin, forwards);
  }

  // ---------- ARM -----------


}
