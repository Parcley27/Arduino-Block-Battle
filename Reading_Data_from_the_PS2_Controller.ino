/*  This example code focuses on how to control a servo. This is designed to help you get started, but
 *  will require you to combine your own functions and control logic from the different examples.
 *
 * 
 *  The final project code relies on three separate libraries.
 *  
 *  The PS2 Shield library is design by the makers of the board, Cytron, to simplify the process
 *  of reading the data from the PS2 controller. It works for wireless or wired controllers.
 *  It allows you to read the joystick values or know if a button has been pressed. With this combination
 *  of libraries, you need to change the TX selector to D9 and RX to D8. This also means these pins are not
 *  available for other purposes.
 *  
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
 *  these for other items.
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

void setup()
{
  Serial.begin(9600);
  
  // Starts the software serial communication at a specific baud rate
  Serial2.begin(9600);

  // Connects the PS2 object to the desired serial communication
  PS2.AttachPS2Serial(&Serial2);

  delay(2000);

}

void loop()
{
  // Command to read/update incoming data from the controller
  PS2.UpdateData();
  
 /* All buttons on the controller can be read with a similar command. Below is the list of all buttons.
  *  The readButton command will return either a 0 or 1, depending on it being pressed.
  *  
  *   PS2_SELECT,  PS2_JOYSTICK_LEFT,  PS2_JOYSTICK_RIGHT,  PS2_START, PS2_UP,  PS2_RIGHT,  PS2_DOWN,
  *   PS2_LEFT, PS2_LEFT_2,  PS2_RIGHT_2,  PS2_LEFT_1,  PS2_RIGHT_1, PS2_TRIANGLE,  PS2_CIRCLE,
  *   PS2_CROSS,  PS2_SQUARE 
  */
    
  // Serial.println(PS2.readButton(PS2_CIRCLE));   // Comment this line out if you want to read the joystick
  
  /*
  if (PS2.readButton(PS2_CIRCLE) == 1) {
    Serial.println(1);
  } else if (PS2.readButton(PS2_CIRCLE) == 0) {
    Serial.println(0);
  }

  Serial.println(PS2.readButton(PS2))

  */

 /*  Reading the values from the two joysticks is a little different. Step one is to identify which joystick
  *  you want to read the value from. You can use both joysticks, but you will need to update the joystick
  *  selection and update the incoming data when switching between joysticks.
  *  
  *  Uncomment a joystick selection and the Serial.println command to see the joystick values
  */
  
    //PS2.SetJoystick(RIGHT_JOYSTICK);
    PS2.SetJoystick(LEFT_JOYSTICK);

    Serial.println(PS2.GetY_Axis_Raw());  //You can also read the X axis of the joystick by changing the letter

  

}