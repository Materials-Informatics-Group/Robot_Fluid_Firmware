/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Syriunge.h
Written by Micke Nicander Kuwahara
=====================================================================
*/
#ifndef Syringe_h
#define Syringe_h

// Include files
//------------------------------------------
#include <Adafruit_PWMServoDriver.h>
#include "Pump.h"
#include "Valve.h"
//------------------------------------------


// Class Definition
//------------------------------------------
class Syringe {
  private:
    int syringeIndex;
    Pump pump;
    Valve valve1;
    Valve valve2;

  public:
    Syringe();
    Syringe(int index, int pumpDirPin, int pumpStepPin, int topSensorPin, Adafruit_PWMServoDriver* servoMaster);
    void startPumpMovement(float rotations);
    void setValveTargetAngle(int valveIndex, int angle);
    int updatePumpMovement();
    int updateValveMovement();
    bool isValveClosed();
    bool isPumpMoving();
    void stop();
    void reset();  
};
//------------------------------------------

#endif
