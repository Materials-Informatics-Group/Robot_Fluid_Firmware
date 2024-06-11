/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Pump.cpp
Written by Micke Nicander Kuwahara
=====================================================================
*/

// Include files
//------------------------------------------
#include "Pump.h"
#include <Arduino.h>
//------------------------------------------

//------------------------------------------
// Constructor: Pump 
//------------------------------------------
Pump::Pump(int index, int pumpDirPin, int pumpStepPin, int topSensorPin)
  : pumpIndex(index), dirPin(pumpDirPin), stepPin(pumpStepPin), topSensor(topSensorPin),
    isMoving(false), stepsMoved(0), targetSteps(0), movementDir(1) {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
//------------------------------------------

//------------------------------------------
// Method: startMovement 
//------------------------------------------
void Pump::startMovement(float rotations) {
  targetSteps = rotations * MOTOR_FULL_ROT_PULSES;
  stepsMoved = 0;
  isMoving = true;
  movementDir = (rotations > 0) ? HIGH : LOW;
  digitalWrite(dirPin, movementDir);
}
//------------------------------------------

//------------------------------------------
// Method: update
//------------------------------------------
int Pump::update(bool isValveClosed) {
  int reply = -1;
  if (isMoving) {        
    if (topSensor.isTriggered() && movementDir == sensorTriggerDir) {
      reply = 1;
      stop();
    }
    else if(isValveClosed){
      stop();
      reply = 3;
    }
    else{
      if (stepsMoved < abs(targetSteps)) {
        moveOneStep();
        stepsMoved++;
      }
      else{
        stop();
        reply = 2;
      }
      if(sensorTriggerDir != movementDir && stepsMoved > (MOTOR_FULL_ROT_PULSES*2)){ sensorTriggerDir = movementDir; }
    }
  }
  return reply;
}
//------------------------------------------

//------------------------------------------
// Method: moveOneStep
//------------------------------------------
void Pump::moveOneStep() {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(MOTOR_DELAY); 
  digitalWrite(stepPin, LOW);
  delayMicroseconds(MOTOR_DELAY); 
}
//------------------------------------------


//------------------------------------------
// Method: is Pump Moving
//------------------------------------------
bool Pump::isPumpMoving() {
  return isMoving;
}
//------------------------------------------



//------------------------------------------
// Method: stop
//------------------------------------------
void Pump::stop() {
  isMoving = false;
}
//------------------------------------------

//------------------------------------------
// Method: reset
//------------------------------------------
void Pump::reset() {
  // startMovement(-2);
  startMovement(50);
}
//------------------------------------------
