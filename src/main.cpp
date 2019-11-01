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
#include <string>

using namespace vex;

/*
  Stops all motors
*/
void stopAll()
{
  RightMotor.stop();
  LeftMotor.stop();
}

/*
  Sets motor velocity to a certain percent
*/
void setTotalVelocity(int perc)
{
  if(perc <= 0) stopAll();
  else 
  {
    RightMotor.setVelocity(perc, percent);
    LeftMotor.setVelocity(perc, percent);
  }
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
  Spins robot to the left
*/
void spinLeft() 
{
  LeftMotor.spin(reverse);
  RightMotor.spin(forward);
}

/*
  Spins robot to the right
*/
void spinRight() 
{
  RightMotor.spin(reverse);
  LeftMotor.spin(forward);
}

/*
  Gets the current velocity, or the average of the motors' velocities
*/
int getCurrentAvgVelocity()
{
  return LeftMotor.isSpinning() && RightMotor.isSpinning() ? (LeftMotor.velocity(percent) + RightMotor.velocity(percent)) / 2 : 50;
}


int main()
{
  vexcodeInit();

  //Configure Axes here
  controller::axis bAxisFB = Controller1.Axis2;
  controller::axis bAxisLR = Controller1.Axis4;

  controller::button incrVel = Controller1.ButtonUp;
  controller::button decrVel = Controller1.ButtonDown;
  controller::button resetVel = Controller1.ButtonA;

  controller::button maxVel = Controller1.ButtonX;
  controller::button minVel = Controller1.ButtonB; //Used to be called Master Stop, same functionality

  controller::button bEnd = Controller1.ButtonL1;

  setTotalVelocity(50);
  int fbPos, lrPos;
  bool end = false;

  while(!end)
  {
    //Stop Robot from running in dire situations
    if(bEnd.pressing()) end = true;

    //Velocity Modifiers
    if(incrVel.pressing()) setTotalVelocity(getCurrentAvgVelocity() + 1);
    else if(decrVel.pressing()) setTotalVelocity(getCurrentAvgVelocity() - 1);

    if(resetVel.pressing()) setTotalVelocity(50);

    if(maxVel.pressing()) setTotalVelocity(100);
    if(minVel.pressing()) stopAll();

    //Update the axes position values
    fbPos = bAxisFB.position();
    lrPos = bAxisLR.position();

    //Turn off motors if neither axis is in use
    if(fbPos == 0 && lrPos == 0) stopAll();

    //Forwards and Backwards (Axis 2)
    if(fbPos > 0) drive(true);
    else if(fbPos < 0) drive(false);

    //Left and Right (Axis 4)
    if(lrPos > 0) spinRight();
    else if(lrPos < 0) spinLeft();
  }
  Brain.Screen.print("Robot has stopped");
}
