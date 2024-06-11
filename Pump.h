/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Pump.h
Written by Micke Nicander Kuwahara
=====================================================================
*/
#ifndef Pump_h
#define Pump_h

// Include files
//------------------------------------------
#include "TopSensor.h"
//------------------------------------------

// Various Defines
//------------------------------------------
#define MOTOR_FULL_ROT_PULSES 200
#define MOTOR_DELAY 500

// Class Definition
//------------------------------------------
class Pump {
  private:
    int pumpIndex;
    int stepPin;
    int dirPin; 
    int level;
    float targetSteps;
    float stepsMoved;
    TopSensor topSensor;
    bool isMoving;
    int movementDir;
    int sensorTriggerDir;
    void moveOneStep();

  public:
    Pump(int index, int pumpDirPin, int pumpStepPin, int topSensorPin);
    void startMovement(float rotations);
    int update(bool isValveClosed);
    void stop();
    void reset();
    bool isPumpMoving();
};
//------------------------------------------

#endif
