/*
=====================================================================
Takahashi Group Syringe Bot Firmware
TopSensor.h
Written by Micke Nicander Kuwahara
=====================================================================
*/
#ifndef TopSensor_h
#define TopSensor_h

// Class Definition
//------------------------------------------
class TopSensor {
  private:
    int topSensorPin;
    bool topSensorState;

  public:
    TopSensor(int limitPin);
    bool isTriggered();
    int getState();
};
//------------------------------------------

#endif
