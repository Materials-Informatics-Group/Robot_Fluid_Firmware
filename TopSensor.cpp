/*
=====================================================================
Takahashi Group Syringe Bot Firmware
TopSensor.cpp
Written by Micke Nicander Kuwahara
=====================================================================
*/

// Include files
//------------------------------------------
#include <Arduino.h>
#include "TopSensor.h"
//------------------------------------------

//------------------------------------------
// Constructor: TopSensor
//------------------------------------------
TopSensor::TopSensor(int limitPin) : topSensorPin(limitPin), topSensorState(false) {
  pinMode(topSensorPin, INPUT_PULLUP);
}
//------------------------------------------

//------------------------------------------
// Method: isTriggered
//------------------------------------------
bool TopSensor::isTriggered() {
  return (digitalRead(topSensorPin) == 1);
}
//------------------------------------------

//------------------------------------------
// Method: getState
//------------------------------------------
int TopSensor::getState() {
  return digitalRead(topSensorPin); 
}
//------------------------------------------
