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

// Declare servo position variables and set them to starting positions
bool handOpen = false;
int wristPosition = 1850;
int elbowPosition = 750;
int shoulderPosition = 1450;

// Keeps track of the last time the hand closed
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

  // Move servos to starting positions
  hand.write(1250);
  wrist.write(wristPosition);
  elbow.write(elbowPosition);
  shoulder.write(shoulderPosition);

  lastChange = millis();

}

// Function for driving direction and speed
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

// Function for opening/closing robot arm's hand
void moveHand() {
  PS2.UpdateData();

  // Create a variable to hold the current time since the program started
  unsigned long currentTime = millis();

  // Store the value of the cross button on the controller
  int crossButton = PS2.readButton(PS2_CROSS);

  // Toggle hand position
  // If button is being pressed && it has been long enough since the last change
  if (crossButton == HIGH && currentTime > (lastChange + 250)) {
    // If hand is open, close it
    if (handOpen == true) {
      hand.write(1150);
      handOpen = false;
      //Serial.println("closing hand");

    // Else, open hand
    } else {
      hand.write(750);
      handOpen = true;
      //Serial.println("opening hand");
    }

    // Update lastChange to the currentTime
    lastChange = currentTime;
  }
}

// Function for moving robot arm's wrist
void moveWrist() {
  PS2.UpdateData();

  // Set wristUp/Down to the values of R1 and R2
  int wristUp = PS2.readButton(PS2_RIGHT_1); 
  int wristDown = PS2.readButton(PS2_RIGHT_2);

  // Move Wrist
  // If up button pressed
  if (wristUp == HIGH && wristPosition >= 760) {
    wristPosition -= 10;
    //Serial.println(wristPosition);

  // If down button pressed
  } else if (wristDown == HIGH && wristPosition <= 2240) {
    wristPosition += 10;
    //Serial.println(wristPosition);
  }

  // Move wrist servo
  wrist.write(wristPosition);

}

// Function for moving robot arm's elbow
void moveElbow() {
  PS2.UpdateData();

  // Set elbowUp/Down to the values of L1 and L2
  int elbowUp = PS2.readButton(PS2_LEFT_1);
  int elbowDown = PS2.readButton(PS2_LEFT_2);

  // Move Elbow
  // If up button pressed
  if (elbowUp == HIGH && elbowPosition >= 760) {
    elbowPosition -= 10;
    //Serial.println(elbowPosition);

  // If down button pressed
  } else if (elbowDown == HIGH && elbowPosition <= 2240) {
    elbowPosition += 10;
    ///Serial.println(elbowPosition);
  }

  // Move elbow servo
  elbow.write(elbowPosition);
  
}

// Function for moving robot arm's shoulder (base)
void moveShoulder() {

  // Set joystick to the right one for now
  PS2.SetJoystick(RIGHT_JOYSTICK);

  PS2.UpdateData();

  // Get xAxis value, and remap it to +/- 15
  int xAxisRight = PS2.GetX_Axis_Raw();
  int shoulderMoveAmount = map(xAxisRight, 0, 255, 15, -15);

  // Change shoulder position by inputted amount
  shoulderPosition += shoulderMoveAmount;

  // Constrain shoulderPosition to valid outputs
  constrain(shoulderPosition, 750, 2250);

  // Move shoulder (base)
  shoulder.write(shoulderPosition);
  //Serial.println(shoulderPosition);
}

void homeArm() {
  PS2.UpdateData();

  // Get value of the circle button
  int circleButton = PS2.readButton(PS2_CIRCLE);

  // Revert to home positions if the circle button is pressed
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
  // Drive
  drive();

  // Move Arm
  moveHand();
  moveWrist();
  moveElbow();
  moveShoulder();
  homeArm();

}
