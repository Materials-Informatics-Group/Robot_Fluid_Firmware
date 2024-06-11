/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Syriunge.cpp
Written by Micke Nicander Kuwahara
=====================================================================
*/

// Include files
//------------------------------------------
#include "Syringe.h"
//------------------------------------------

//------------------------------------------
// Constructor: Syringe (Empty) 
//------------------------------------------
Syringe::Syringe() : syringeIndex(0), pump(0, 0, 0, 0), valve1(0, nullptr), valve2(1, nullptr) {
}
//------------------------------------------

//------------------------------------------
// Constructor: Syringe (Preset) 
//------------------------------------------
Syringe::Syringe(int index, int pumpDirPin, int pumpStepPin, int topSensorPin, Adafruit_PWMServoDriver* servoMaster)
  : syringeIndex(index), pump(index, pumpDirPin, pumpStepPin, topSensorPin),
    valve1(index * 2, servoMaster), valve2(index * 2 + 1, servoMaster) {
}
//------------------------------------------

//------------------------------------------
// Method: start Pump Movement
//------------------------------------------
void Syringe::startPumpMovement(float rotations) {
  pump.startMovement(rotations);
}
//------------------------------------------

//------------------------------------------
// Method: set Valve Target Angle
//------------------------------------------
void Syringe::setValveTargetAngle(int valveIndex, int angle) {
  if (valveIndex == 1) {
    valve1.setTargetAngle(angle);
  } else if (valveIndex == 2) {
    valve2.setTargetAngle(angle);
  }
}
//------------------------------------------

//------------------------------------------
// Method: update Pump Movement
//------------------------------------------
int Syringe::updatePumpMovement() {
  return pump.update(isValveClosed());
}
//------------------------------------------

//------------------------------------------
// Method: update Valve Movement
//------------------------------------------
int Syringe::updateValveMovement() {
  int reply = valve1.update(1);
  reply += valve2.update(2);

  return reply;
}
//------------------------------------------

//------------------------------------------
// Method: get Valve Close State
//------------------------------------------
bool Syringe::isValveClosed() {
  return (valve1.getAngle() < 5 || (valve1.getAngle() < 160 && valve2.getAngle() < 5));
}
//------------------------------------------


//------------------------------------------
// Method: is Pump Moving
//------------------------------------------
bool Syringe::isPumpMoving() {
  return pump.isPumpMoving();
}
//------------------------------------------


//------------------------------------------
// Method: stop
//------------------------------------------
void Syringe::stop() {
  pump.stop();
  valve1.stop();
  valve2.stop();
}
//------------------------------------------

//------------------------------------------
// Method: reset
//------------------------------------------
void Syringe::reset() {  
  valve1.quickOpen();
  valve2.quickOpen();
  delay(1000);
  pump.reset();
  valve1.reset();
  valve2.reset();  
}
//------------------------------------------