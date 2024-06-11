/*
=====================================================================
Takahashi Group Syringe Bot Firmware
Valve.h
Written by Micke Nicander Kuwahara
=====================================================================
*/
#ifndef Valve_h
#define Valve_h

// Include files
//------------------------------------------
#include <Adafruit_PWMServoDriver.h>
//------------------------------------------

// Various Defines
//------------------------------------------
#define SERVOMIN  180 // 'minimum' pulse length (out of 4096)
#define SERVOMAX  560 // 'maximum' pulse length (out of 4096)

// Class Definition
//------------------------------------------
class Valve {
  private:
    int valveIndex;
    int currentAngle;
    int targetAngle;
    bool moving;
    Adafruit_PWMServoDriver* theServoMaster;
    int angleToPulse(int whatAngle);

  public:
    Valve(int index, Adafruit_PWMServoDriver* servoMaster);
    int getAngle();
    void setTargetAngle(int newAngle);
    int update(int whichSyrV);
    bool isMoving();
    void stop();
    void reset();
    void quickOpen();
};
//------------------------------------------

#endif
