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
  Overloaded method sets a custom velocity before starting the motors
*/
void drive(bool directionForward)
{
  directionType dir;

  if(directionForward) dir = forward;
  else dir = reverse;

  RightMotor.spin(dir);
  LeftMotor.spin(dir);
}

void drive(bool forward, int vel)
{
  setTotalVelocity(vel);
  drive(forward);
}

/*
  Stops all motors
*/
void stopAll()
{
  RightMotor.stop();
  LeftMotor.stop();
}

/*
  Turns robot left, in an arc (doesn't spin in place)
*/
void turnLeft()
{
  LeftMotor.stop();
  RightMotor.spin(forward);
}

/*
  Turns robot right, in an arc (doesn't spin in place)
*/
void turnRight()
{
  RightMotor.stop();
  LeftMotor.spin(forward);
}

/*
  Spins robot to the left
*/
void spinLeft() {
  LeftMotor.spin(reverse);
  RightMotor.spin(forward);
}

/*
  Spins robot to the right
*/
void spinRight() {
  RightMotor.spin(reverse);
  LeftMotor.spin(forward);
}


int main()
{
  vexcodeInit();

  //Configure Buttons Here - eventually should be converted to using the axes buttons
  controller::button bFront1 = Controller1.ButtonR2;
  controller::button bFront2 = Controller1.ButtonL2;

  controller::button bBack1 = Controller1.ButtonR1;
  controller::button bBack2 = Controller1.ButtonL1;

  controller::button bLeft = Controller1.ButtonY;
  controller::button bRight = Controller1.ButtonA;

  controller::button bLeftSpin = Controller1.ButtonLeft;
  controller::button bRightSpin = Controller1.ButtonRight;

  controller::button masterStop = Controller1.ButtonX;

  //Robot settings before running
  setTotalVelocity(50);

  while(true)
  {
    //Forward
    if(bFront1.pressing() && bFront2.pressing()) drive(true);
    else
    {
      bFront1.released(stopAll);
      bFront2.released(stopAll);
    }

    //Reverse
    if(bBack1.pressing() && bBack2.pressing()) drive(false);
    else
    {
      bBack1.released(stopAll);
      bBack2.released(stopAll);
    }

    //Right
    if(bRight.pressing()) turnRight();
    else bRight.released(stopAll);

    //Left
    if(bLeft.pressing()) turnLeft();
    else bLeft.released(stopAll);

    //Right Spin
    if (bRightSpin.pressing()) spinRight();
    else bRightSpin.released(stopAll);

    //Right Spin
    if (bLeftSpin.pressing()) spinLeft();
    else bLeftSpin.released(stopAll);

    // Master Stop Button
    if(masterStop.pressing()) stopAll();
  }
}
