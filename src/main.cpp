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

int defaultVelocity = 75; //Default velocity for the robot

controller::axis axisForwardBackward = Controller1.Axis2; //Axis used to drive forward and backward
controller::axis axisLeftRight = Controller1.Axis4; //Axis used to move left and right

controller::button incrVel = Controller1.ButtonUp; //Increases velocity by 1%
controller::button decrVel = Controller1.ButtonDown; //Decreases velocity by 1%
controller::button resetVel = Controller1.ButtonA; //Resets the velocity to the default
controller::button maxVel = Controller1.ButtonX; //Sets velocity to 100%
controller::button zeroVel = Controller1.ButtonB; //Sets velocity to 0% (stops motors)

//WIP
controller::button instaSpinRight = Controller1.ButtonR2; //Spins Robot to the right at 100% velocity
controller::button instaSpinLeft = Controller1.ButtonL2; //Spins Robot to the left at 100% velocity

/*
  Stops all motors
*/
void stopAll()
{
  RightMotor.stop();
  LeftMotor.stop();
}

/*
  Sets total velocity to a certain percent
*/
void setTotalVelocity(int perc)
{
  if (perc <= 0) stopAll();
  RightMotor.setVelocity(perc, percent);
  LeftMotor.setVelocity(perc, percent);
}

/*
  Returns default velocity
*/
int getDefaultVel() 
{
  return defaultVelocity;
}

/*
  Gets the current velocity of both motors
*/
int getCurrentVelocity()
{
  if (LeftMotor.isSpinning() && RightMotor.isSpinning()) 
  {
    double totalVel = LeftMotor.velocity(percent) + RightMotor.velocity(percent);
    double avgVel = totalVel / 2;
    return avgVel;
  } 
  else return 0;
}

void printVel()
{
  ostringstream stream1;

  stream1 << "Right Motor Vel: " << RightMotor.velocity(percent) << " . Left Motor Vel: " << LeftMotor.velocity(percent);

  Brain.Screen.print(RightMotor.velocity(percent));
  Brain.Screen.print(LeftMotor.velocity(percent));
}

/*
  Drives forward or backward
  @param vel Robot velocity. If vel > 0, robot will drive forward, if vel < 0 backward
*/
void drive(int vel)
{
  if(vel != 0) 
  {
    directionType dir = vel > 0 ? vex::forward : vex::reverse;
    RightMotor.spin(dir);
    LeftMotor.spin(dir);
  }
  else stopAll();
}

/*
  Spins the robot to the left
  @param instant If set to true, robot will set velocity to 100
  @param arc If set to true, robot will halve the left motor's velocity to make for a smoother turn
*/
void spinLeft(bool instant = false, bool arc = false) 
{
  if(instant) setTotalVelocity(100);
  else if(arc) LeftMotor.setVelocity(getCurrentVelocity() / 2, percent);

  LeftMotor.spin(vex::reverse);
  RightMotor.spin(vex::forward);

  if(instant || arc) setTotalVelocity(getDefaultVel());
}

/*
  Spins the robot to the right
  @param instant If set to true, robot will set velocity to 100
  @param arc If set to true, robot will halve the right motor's velocity to make for a smoother turn
*/
void spinRight(bool instant = false, bool arc = false) 
{
  if(instant) setTotalVelocity(100);
  else if(arc) LeftMotor.setVelocity(getCurrentVelocity() / 2, percent);

  LeftMotor.spin(vex::forward);
  RightMotor.spin(vex::reverse);

  if(instant || arc) setTotalVelocity(getDefaultVel());
}

/*
  Method to be run during autonomous period
*/
void autonomousControl()
{
  //NOT IMPLEMENTED
}

/*
  Method to be run when a driver is controlling the bot
*/
void manualControl()
{
  int forwardBackwardPos, leftRightPos;
  while(true)
  {
    //Velocity Modifiers
    if(incrVel.pressing()) setTotalVelocity(getCurrentVelocity() + 5);
    if(decrVel.pressing()) setTotalVelocity(getCurrentVelocity() - 5);
    if(resetVel.pressing()) setTotalVelocity(getDefaultVel());

    if(maxVel.pressing()) setTotalVelocity(100);
    if(zeroVel.pressing()) stopAll();

    //Instant Spin
    if (instaSpinRight.pressing()) 
    {
      spinRight(true);
      instaSpinRight.released(stopAll);
    }
    else if (instaSpinLeft.pressing()) 
    {
      spinLeft(true);
      instaSpinLeft.released(stopAll);
    }
  
    //Update the axes position values
    forwardBackwardPos = axisForwardBackward.position();
    leftRightPos = axisLeftRight.position();

    //Stop motors if neither axis in use
    if(forwardBackwardPos == 0 && leftRightPos == 0) stopAll();

    //Forwards and Backwards (Axis 2)
    if(forwardBackwardPos != 0) drive(forwardBackwardPos);

    //Left and Right (Axis 4)
    if(leftRightPos > 0 && forwardBackwardPos != 0) spinRight(false, true);
    else if(leftRightPos > 0) spinRight();
    else if(leftRightPos < 0 && forwardBackwardPos != 0) spinLeft(false, true);
    else if(leftRightPos < 0) spinLeft();
  }
}

int main()
{
  vexcodeInit();
  manualControl();
}
