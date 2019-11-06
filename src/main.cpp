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
#include <sstream>

using namespace vex;
using namespace std;

int defaultVelocity = 0;

void printVel();

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
  printVel();
  if(perc <= 0) stopAll();
  else 
  {
    RightMotor.setVelocity(perc, percent);
    LeftMotor.setVelocity(perc, percent);
  }
  printVel();
}

/*
  Drives robot forward or backward
*/
void drive(bool directionForward, int vel = 0)
{
  directionType dir;
  if(vel != 0) setTotalVelocity(vel);

  if(directionForward) dir = vex::forward;
  else dir = reverse;

  RightMotor.spin(dir);
  LeftMotor.spin(dir);
}

/*
  Default velocity
*/
int getDefaultVel() {return defaultVelocity;}
void setDefaultVel(int vel) {defaultVelocity = vel;}

/*
  Spins robot to the left
*/
void spinLeft(bool instant = false) 
{
  bool isInstant = false;
  if(instant)
  {
    setTotalVelocity(100);
    isInstant = true;
  }
  LeftMotor.spin(reverse);
  RightMotor.spin(vex::forward);
  if(isInstant) setTotalVelocity(getDefaultVel());
}

/*
  Spins robot to the right
*/
void spinRight(bool instant = false) 
{
  bool isInstant = false;
  if(instant)
  {
    setTotalVelocity(100);
    isInstant = true;
  }
  LeftMotor.spin(vex::forward);
  RightMotor.spin(reverse);
  if(isInstant) setTotalVelocity(getDefaultVel());
}

/*
  Gets the current velocity, or the average of the motors' velocities
*/
int getCurrentAvgVelocity()
{
  bool isOn = LeftMotor.isSpinning() && RightMotor.isSpinning();
  return isOn ? (LeftMotor.velocity(percent) + RightMotor.velocity(percent)) / 2 : 50;
}

void printVel()
{
  ostringstream stream1;

  stream1 << "Right Motor Vel: " << RightMotor.velocity(percent) << " . Left Motor Vel: " << LeftMotor.velocity(percent);

  Brain.Screen.print(RightMotor.velocity(percent));
  Brain.Screen.print(LeftMotor.velocity(percent));
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

  controller::button bInstaSpinR = Controller1.ButtonRight;
  controller::button bInstaSpinL = Controller1.ButtonLeft;

  setDefaultVel(75);
  int fbPos, lrPos;

  while(true)
  {
    //Velocity Modifiers
    if(incrVel.pressing()) setTotalVelocity(getCurrentAvgVelocity() + 1);
    if(decrVel.pressing()) setTotalVelocity(getCurrentAvgVelocity() - 1);
    if(resetVel.pressing()) setTotalVelocity(50);

    if(maxVel.pressing()) setTotalVelocity(100);
    if(minVel.pressing()) stopAll();

    //Instant Spin
    if(bInstaSpinR.pressing() || bInstaSpinL.pressing())
    {
      if(bInstaSpinR.pressing()) spinRight(true);
      else spinLeft(true);
    }
    
    //Axis Stuff
    //Update the axes position values
    fbPos = bAxisFB.position();
    lrPos = bAxisLR.position();

    //Forwards and Backwards (Axis 2)
    if(fbPos > 0) drive(true);
    else if(fbPos < 0) drive(false);

    //Left and Right (Axis 4)
    if(lrPos > 0) spinRight();
    else if(lrPos < 0) spinLeft();
  }
}
