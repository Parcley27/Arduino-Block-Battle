#include <Cytron_PS2_Shield.h>
#include <AltSoftSerial.h>
#include <ServoTimer2.h>

// Create a "PS2" object for the Cryton_PS2_Shield library
Cytron_PS2_Shield PS2;

// Create a "Serial2" object for the AltSoftSerial library
AltSoftSerial Serial2;

// Create a "ArmServo" object for each arm Motor with the ServoTimer2 library

ServoTimer2 hand;
ServoTimer2 wrist;
ServoTimer2 elbow;
ServoTimer2 shoulder;

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

// Arm motor pins
const int handPin = 10;
const int wristPin = 11;
const int elbowPin = 12;
const int shoulderPin = 13;

bool handOpen = false;
int wristPosition = 1850;
int elbowPosition = 750;
int shoulderPosition = 1450;

int lastChange;

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

  // Attatch arm motor pins to respective objects
  
  hand.attach(handPin);
  wrist.attach(wristPin);
  elbow.attach(elbowPin);
  shoulder.attach(shoulderPin);

  hand.write(1250);
  wrist.write(wristPosition);
  elbow.write(elbowPosition);
  shoulder.write(shoulderPosition);

  lastChange = millis();

}

void drive() {

  // Set joystick to the left one for now
  PS2.SetJoystick(LEFT_JOYSTICK); 
  
  // Refresh PS2 controller data
  PS2.UpdateData();

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

    //Serial.println("forwards");

  // Else if user wants to go forwards
  } else if (leftJoystickYPosition > 10) { //forwards
    analogWrite(rightSpeedPin, rightMotorSpeed);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, leftMotorSpeed);
    digitalWrite(leftDirectionPin, forwards);

    //Serial.println("backwards");

  // Else (In deadzone)
  } else {
    analogWrite(rightSpeedPin, 0);
    digitalWrite(rightDirectionPin, forwards);

    analogWrite(leftSpeedPin, 0);
    digitalWrite(leftDirectionPin, forwards);

    //Serial.println("dead zone");
  }
}

void moveHand() {
  PS2.UpdateData();

  unsigned long currentTime = millis();

  int crossButton = PS2.readButton(PS2_CROSS);

  // Toggle hand position
  if (crossButton == HIGH && currentTime > (lastChange + 250)) {
    if (handOpen == true) {
      hand.write(1150);
      handOpen = false;
      //Serial.println("closing hand");

    } else {
      hand.write(750);
      handOpen = true;
      //Serial.println("opening hand");
    }

    lastChange = currentTime;
  }
}

void moveWrist() {
  PS2.UpdateData();

  int wristUp = PS2.readButton(PS2_RIGHT_1); 
  int wristDown = PS2.readButton(PS2_RIGHT_2);

  // Move Wrist
  if (wristUp == HIGH && wristPosition >= 760) {
    wristPosition -= 10;
    //Serial.println(wristPosition);

  } else if (wristDown == HIGH && wristPosition <= 2240) {
    wristPosition += 10;
    //Serial.println(wristPosition);
  }

  wrist.write(wristPosition);

}

void moveElbow() {
  PS2.UpdateData();

  int elbowUp = PS2.readButton(PS2_LEFT_1);
  int elbowDown = PS2.readButton(PS2_LEFT_2);

  // Move Elbow
  if (elbowUp == HIGH && elbowPosition >= 760) {
    elbowPosition -= 10;
    //Serial.println(elbowPosition);

  } else if (elbowDown == HIGH && elbowPosition <= 2240) {
    elbowPosition += 10;
    ///Serial.println(elbowPosition);
  }

  elbow.write(elbowPosition);
  
}

void moveShoulder() {
  PS2.SetJoystick(RIGHT_JOYSTICK);

  PS2.UpdateData();

  // Move Shoulder 
  int xAxisRight = PS2.GetX_Axis_Raw();
  int shoulderMoveAmount = map(xAxisRight, 0, 255, 15, -15);

  shoulderPosition += shoulderMoveAmount;

  if (shoulderPosition > 2250) {
    shoulderPosition = 2250;

  } else if (shoulderPosition < 750) {
    shoulderPosition = 750;
  }

  shoulder.write(shoulderPosition);
  //Serial.println(shoulderPosition);
}

void homeArm() {
  PS2.UpdateData();

  int circleButton = PS2.readButton(PS2_CIRCLE);

  // Revert to home position
  if (circleButton == HIGH) {
    handOpen = true;
    hand.write(750);

    wristPosition = 1850;
    wrist.write(wristPosition);

    elbowPosition = 750;
    elbow.write(elbowPosition);

    shoulderPosition = 1450;
    shoulder.write(shoulderPosition);

  }
}

void loop() {
  drive();

  moveHand();
  moveWrist();
  moveElbow();
  moveShoulder();
  homeArm();
}
