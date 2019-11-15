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
// LeftMotor            motor         1               
// RightMotor           motor         2               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <iostream>
#include <sstream>

using namespace vex;
using namespace std;

int defaultVelocity = 0;

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
  if (perc <= 0) {
    stopAll();  
  }
  RightMotor.setVelocity(perc, percent);
  LeftMotor.setVelocity(perc, percent);
}

/*
  Drives robot forward or backward
*/
void drive(int vel = 0)
{
  directionType dir = vex::forward;

  if(vel > 0) dir = vex::forward;
  else if(vel < 0) dir = vex::reverse;
  else stopAll();

  RightMotor.spin(dir);
  LeftMotor.spin(dir);
}

/*
  Default velocity getters and setters
*/
int getDefaultVel() {
  return defaultVelocity;
}
void setDefaultVel(int vel) {
  defaultVelocity = vel;
}

/*
  Spins robot to the left
*/
void spinLeft(bool instant = false) 
{
  if (instant)
  {
    setTotalVelocity(100);
  }
  LeftMotor.spin(reverse);
  RightMotor.spin(vex::forward);
  if (instant) {
    setTotalVelocity(getDefaultVel());
  } 
}

/*
  Spins robot to the right
*/
void spinRight(bool instant = false) 
{
  if(instant)
  {
    setTotalVelocity(100);
  }
  LeftMotor.spin(vex::forward);
  RightMotor.spin(reverse);
  if(instant) {
    setTotalVelocity(getDefaultVel());
  }
}

/*
  Gets the current velocity, or the average of the motors' velocities
*/
int getCurrentVelocity()
{
  bool isOn = LeftMotor.isSpinning() && RightMotor.isSpinning();
  if (isOn) {
    double totalVel = LeftMotor.velocity(percent) + RightMotor.velocity(percent);
    double avgVel = totalVel / 2;
    return avgVel;
  } else {
    return 0;
  }
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
  controller::axis axisForwardBackward = Controller1.Axis2;
  controller::axis axisLeftRight = Controller1.Axis4;

  controller::button incrVel = Controller1.ButtonUp;
  controller::button decrVel = Controller1.ButtonDown;
  controller::button resetVel = Controller1.ButtonA;

  controller::button maxVel = Controller1.ButtonX;
  controller::button zeroVel = Controller1.ButtonB; //Used to be called Master Stop, same functionality

  controller::button instaSpinRight = Controller1.ButtonRight;
  controller::button instaSpinLeft = Controller1.ButtonLeft;

  setDefaultVel(75);
  int forwardBackwardPos, leftRightPos;

  while(true)
  {
    //Velocity Modifiers
    if(incrVel.pressing()) setTotalVelocity(getCurrentVelocity() + 1);
    if(decrVel.pressing()) setTotalVelocity(getCurrentVelocity() - 1);
    if(resetVel.pressing()) setTotalVelocity(50);

    if(maxVel.pressing()) setTotalVelocity(100);
    if(zeroVel.pressing()) stopAll();

    //Instant Spin
    if (instaSpinRight.pressing()) {
      spinRight(true);
    } else if (instaSpinLeft.pressing()) {
      spinLeft(true);
    }
    
    //Axis Stuff
    //Update the axes position values
    forwardBackwardPos = axisForwardBackward.position();
    leftRightPos = axisLeftRight.position();

    //Stop motors if neither axis in use
    if(forwardBackwardPos == 0 && leftRightPos == 0) stopAll();

    //Forwards and Backwards (Axis 2)
    if(forwardBackwardPos != 0) drive(forwardBackwardPos);

    //Left and Right (Axis 4)
    if(leftRightPos > 0) spinRight();
    else if(leftRightPos < 0) spinLeft();
  }
}
