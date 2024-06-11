/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Valve.cpp
Written by Micke Nicander Kuwahara
=====================================================================
*/

// Include files
//------------------------------------------
#include "Valve.h"
//------------------------------------------

//------------------------------------------
// Constructor: Valve
//------------------------------------------
Valve::Valve(int index, Adafruit_PWMServoDriver* servoMaster)
  : valveIndex(index), currentAngle(0), targetAngle(0), moving(false), theServoMaster(servoMaster) { }
//------------------------------------------

//------------------------------------------
// Method: get Angle
//------------------------------------------
int Valve::getAngle() {
  return currentAngle;
}
//------------------------------------------

//------------------------------------------
// Method: set Target Angle
//------------------------------------------
void Valve::setTargetAngle(int newAngle) {
  if (newAngle >= 0 && newAngle <= 180) {
    targetAngle = newAngle;
    moving = true;
  }
}
//------------------------------------------

//------------------------------------------
// Method: update
//------------------------------------------
int Valve::update(int whichSyrV) {  
  int reply = 0;
  if (moving) {
    if (currentAngle < targetAngle) {
      currentAngle++;
    } else if (currentAngle > targetAngle) {
      currentAngle--;
    }
    
    theServoMaster->setPWM(valveIndex, 0, angleToPulse(currentAngle));

    if (currentAngle == targetAngle) {
      moving = false;
      reply = whichSyrV;
    }
  }
  return reply;
}
//------------------------------------------

//------------------------------------------
// Method: isMoving
//------------------------------------------
bool Valve::isMoving() {
  return moving;
}
//------------------------------------------

//------------------------------------------
// Method: angle To Pulse
//------------------------------------------
int Valve::angleToPulse(int whatAngle) {
  int pulse = map(whatAngle, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}
//------------------------------------------


//------------------------------------------
// Method: stop
//------------------------------------------
void Valve::stop() {
  moving = false;
}
//------------------------------------------


//------------------------------------------
// Method: reset
//------------------------------------------
void Valve::reset() {
  setTargetAngle(0);
}
//------------------------------------------


//------------------------------------------
// Method: reset
//------------------------------------------
void Valve::quickOpen() {
  theServoMaster->setPWM(valveIndex, 0, angleToPulse(90));
  currentAngle = 90;
}
//------------------------------------------

