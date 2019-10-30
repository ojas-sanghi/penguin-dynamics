/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Saptarshi Mallick, Ojas Sanghi, Khoa Ho                   */
/*    Created:      Mon Oct 28 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftMotor            motor         10
// RightMotor           motor         1
// Controller1          controller
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <iostream>

using namespace vex;

/*
  Sets motor velocity to a certain percent
*/
void setTotalVelocity(int perc)
{
  RightMotor.setVelocity(perc, percent);
  LeftMotor.setVelocity(perc, percent);
}

/*
  Drives robot forward or backward
*/
void drive(bool directionForward, int vel = 0)
{
  directionType dir;
  if(vel != 0) setTotalVelocity(vel);

  if(directionForward) dir = forward;
  else dir = reverse;

  RightMotor.spin(dir);
  LeftMotor.spin(dir);
}

/*
  Stops all motors
*/
void stopAll()
{
  RightMotor.stop();
  LeftMotor.stop();
}

/* Deprecated
  //Turns robot left, in an arc (doesn't spin in place)
void turnLeft()
{
  LeftMotor.stop();
  RightMotor.spin(forward);
}

  //Turns robot right, in an arc (doesn't spin in place)
void turnRight()
{
  RightMotor.stop();
  LeftMotor.spin(forward);
}
*/

/*
  Spins robot to the left - Overloaded method sets velocity as well
*/
void spinLeft() 
{
  LeftMotor.spin(reverse);
  RightMotor.spin(forward);
}

void spinLeft(int vel)
{
  setTotalVelocity(vel);
  spinLeft();
}

/*
  Spins robot to the right - Overloaded method sets velocity as well
*/
void spinRight() 
{
  RightMotor.spin(reverse);
  LeftMotor.spin(forward);
}

void spinRight(int vel)
{
  setTotalVelocity(vel);
  spinRight();
}

bool isBetween(int num, int min, int max)
{
  return max > min ? num >= min && num <= max : num <= max && num >= max;
}

int main()
{
  vexcodeInit();

  //Configure Buttons and Axes Here

  controller::axis bAxisFB = Controller1.Axis2;
  controller::axis bAxisLR = Controller1.Axis4;

  controller::button masterStop = Controller1.ButtonX;

  while(true)
  {
    int fbPos = bAxisFB.position();
    int lrPos = bAxisLR.position();

    //Forwards and Backwards
    if(isBetween(fbPos, 1, 100))
    {
      drive(true, fbPos);
    }
    else if(isBetween(fbPos, -1, -100))
    {
      drive(false, fbPos * -1);
    }
    else if(fbPos == 0) stopAll();

    //Left and Right - CHECK TO SEE WHAT 100 ACTUALLY MEANS
    if(isBetween(lrPos, 1, 100))
    {
      spinLeft(lrPos);
    }
    else if(isBetween(lrPos, -1, -100))
    {
      spinRight(lrPos * -1);
    }
    else if(lrPos == 0) stopAll();

    // Master Stop Button
    if(masterStop.pressing()) stopAll();
  }
}
