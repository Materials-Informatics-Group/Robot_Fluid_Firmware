/*
=====================================================================
Takahashi Group Syringe Bot Firmware
TakahashiGroup_SyringeBotFirmware_variant1.ino
Written by Micke Nicander Kuwahara
=====================================================================
*/

//------------------------------------------
// Including Libs
//------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Syringe.h"
//------------------------------------------

//------------------------------------------
// Defines
//------------------------------------------
#define SERIAL_TIMEOUT 10
#define SERIAL_BAUDRATE 115200
#define NO_OF_SYRINGES 4
#define STEPPER_ENABLE_PIN 8
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SPIN_PER_ML_10 2.9
#define SPIN_PER_ML_60 0.75
//------------------------------------------

//------------------------------------------
// Global Variables
//------------------------------------------
int stepPins[] = {2, 3, 4, 12};
int dirPins[] = {5, 6, 7, 13};
int limitPins[] = {9, 10, 11, 22};
bool isConnected = false;
int resetPumpModeActive = 0;
int initPumpModeActive = -1;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Syringe syringes[NO_OF_SYRINGES];
//------------------------------------------


//------------------------------------------------------------------------------------
// Function: Send Message
// Halt all motor and servo movements
//------------------------------------------------------------------------------------
void sendMsg(String firstChar, String followMsg){
  Serial.print(firstChar);
  Serial.println(followMsg);
  delay(10);
}
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Function: Stop All
// Halt all motor and servo movements
//------------------------------------------------------------------------------------
void stopAll(){
  for(int i = 0; i < NO_OF_SYRINGES; i++) {
    syringes[i].stop();
  }
}
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Function: Reset All
// Move all moving parts to their reset positions
//------------------------------------------------------------------------------------
void resetAll(){
  for(int i = 0; i < NO_OF_SYRINGES; i++) {
    syringes[i].reset();
  }
}
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Function: Setup
// Prepare and setup the devices
//------------------------------------------------------------------------------------
void setup() {
  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial.begin(SERIAL_BAUDRATE);

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ); 

  for(int i = 0; i < NO_OF_SYRINGES; i++) {
    syringes[i] = Syringe(i, dirPins[i], stepPins[i], limitPins[i], &pwm);
  }

  pinMode(STEPPER_ENABLE_PIN, OUTPUT);
  digitalWrite(STEPPER_ENABLE_PIN, LOW);
}
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Function: Loop
// All the action in each cycle
//------------------------------------------------------------------------------------
void loop() {
  // Move all parts that need to be moved and communicate their status if needed
  for (int i = 0; i < NO_OF_SYRINGES; i++) {
    int pumpReply = syringes[i].updatePumpMovement();
    if (pumpReply > -1) {
      if (pumpReply == 1) { //Top
        sendMsg("F", String(57) + String(i));
      } else if (pumpReply == 2) { //requested target
        sendMsg("F", String(55) + String(i));
      } else if (pumpReply == 3) { //emergrncy stop
        sendMsg("F", String(53) + String(i));
      }
      if(resetPumpModeActive > 0){
        resetPumpModeActive -= 1;
      }   
      if(initPumpModeActive >= 0){
        initPumpModeActive = -1;
      }           
    }

    if(resetPumpModeActive <= 0 && initPumpModeActive < 0){
      int valveReply = syringes[i].updateValveMovement(); 
      if (valveReply != 0) {
        sendMsg("F", String(66) + String(i) + String(valveReply));
      } 
    }      
  }

  // Check for any Commands from the GUI 
  if(Serial.available()){
    String data = Serial.readString();
    char command = data.charAt(0);
    int code = -1;
    
    switch(command) {
      // Start the moving of the pump for specified Syringe
      case 'P':  
        if(isConnected){
          int syringeIndex = data.charAt(1) - '0';
          if(!syringes[syringeIndex].isValveClosed()){                  
            float rotations = data.substring(2).toFloat();
            syringes[syringeIndex].startPumpMovement(rotations);
          }
          else{
            sendMsg("F", String(53) + String(syringeIndex));
          }
        }
        break;
      
      // Start the moving of a specified servo for specified Syringe
      case 'V':
        if(isConnected){
          int syringeIndex = data.charAt(1) - '0';
          if(!syringes[syringeIndex].isPumpMoving()){            
            int valveIndex = data.charAt(2) - '0';
            int angle = data.substring(3).toInt();
            syringes[syringeIndex].setValveTargetAngle(valveIndex, angle);
          }
          else{
            sendMsg("F", String(63) + String(syringeIndex));
          }          
        }
        break;
      
      // Reset all parts
      case 'I':  
        if(isConnected){          
          int syringeIndex = data.charAt(1) - '0';
          initPumpModeActive = syringeIndex;
          syringes[syringeIndex].reset();
        }
        break;
      
      // Reset all parts
      case 'R':  
        if(isConnected){
          resetPumpModeActive = NO_OF_SYRINGES;
          resetAll();
        }
        break;
      
      // Stop all moving parts
      case 'S':  
        stopAll();
        sendMsg("F", String(99));
        break;
      
      // First time Connect and initiate Serial Communication with PC App
      case 'A': 
        code = (data.substring(1)).toInt();
        if(code == 77){
          isConnected = true;
          sendMsg("B", String(22));
        }
        break;
      

      // Virtual Disconnect Serial Communication with PC App
      case 'D': 
        code = (data.substring(1)).toInt();
        if(code == 77){
          isConnected = false;
          sendMsg("B", String(44));
        }
        break;
      
      // Second or more time to Connect Serial Communication with PC App
      case 'C': 
        code = (data.substring(1)).toInt();
        if(code == 77){
          isConnected = true;
          sendMsg("B", String(33));
        }
        break;
            
      // Some unknow command
      // default:
      //     sendMsg("F", String(0));
      //     break;
    }
  }
}
