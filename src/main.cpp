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

controller::axis axisForwardBackward() {return Controller1.Axis2;} //Axis used to drive forward and backward
controller::axis axisLeftRight() {return Controller1.Axis4;} //Axis used to move left and right

controller::button incrVel() {return Controller1.ButtonUp;} //Increases velocity by 5%
controller::button decrVel() {return Controller1.ButtonDown;} //Decreases velocity by 5%
controller::button resetVel() {return Controller1.ButtonA;} //Resets the velocity to the default
controller::button maxVel() {return Controller1.ButtonX;} //Sets velocity to 100%
controller::button zeroVel() {return Controller1.ButtonB;} //Sets velocity to 0% (stops motors)

//WIP
controller::button instaSpinRight() {return Controller1.ButtonR2;} //Spins Robot to the right at 100% velocity
controller::button instaSpinLeft() {return Controller1.ButtonL2;} //Spins Robot to the left at 100% velocity

controller::button stopProgram() {return Controller1.ButtonY;} //Breaks out of the while loop and stops the program

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
  // if (perc <= 0) stopAll();
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
  directionType dir = vel > 0 ? vex::forward : vex::reverse;
  if(vel != 0) 
  {
    setTotalVelocity(vel < 0 ? vel * -1 : vel);
    Brain.Screen.print(RightMotor.direction());
    Brain.Screen.print(LeftMotor.direction());
    RightMotor.spin(dir);
    LeftMotor.spin(dir);
    Brain.Screen.print(RightMotor.direction());
    Brain.Screen.print(LeftMotor.direction());
  }
  else stopAll();
}

/*
  Spins the robot to the left
  @param instant If set to true, robot will set velocity to 100
*/
void spinLeft(int vel = 100) 
{
  setTotalVelocity(100); 

  LeftMotor.spin(vex::reverse);
  RightMotor.spin(vex::forward);

  if(vel == 100) setTotalVelocity(getDefaultVel());
}

/*
  Spins the robot to the right
  @param instant If set to true, robot will set velocity to 100
*/
void spinRight(int vel = 100) 
{
  setTotalVelocity(100);

  LeftMotor.spin(vex::forward);
  RightMotor.spin(vex::reverse);

  if(vel == 100) setTotalVelocity(getDefaultVel());
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
    if(incrVel().pressing()) setTotalVelocity(getCurrentVelocity() + 5);
    if(decrVel().pressing()) setTotalVelocity(getCurrentVelocity() - 5);
    if(resetVel().pressing()) setTotalVelocity(getDefaultVel());

    if(maxVel().pressing()) setTotalVelocity(100);
    if(zeroVel().pressing()) stopAll();

    /*Instant Spin
    if (instaSpinRight().pressing()) 
    {
      spinRight(true);
      instaSpinRight().released(stopAll);
    }
    else if (instaSpinLeft().pressing()) 
    {
      spinLeft(true);
      instaSpinLeft().released(stopAll);
    }*/
  
    //Update the axes position values
    forwardBackwardPos = axisForwardBackward().position();
    leftRightPos = axisLeftRight().position();

    //Stop motors if neither axis in use
    if(forwardBackwardPos == 0 && leftRightPos == 0) stopAll();

    //Forwards and Backwards (Axis 2)
    if(forwardBackwardPos != 0) drive(forwardBackwardPos < 0 ? forwardBackwardPos * -1 : forwardBackwardPos);

    // Left and Right (Axis 4) - Left is axis pos < 0, right is axis pos > 0

    // Axis Position is positive and forwards/backwards axis is idle (== 0) -> Spin Right
    if(leftRightPos > 0) 
    {
      spinRight(leftRightPos);
    }
    // Axis Position is negative and forwards/backwards axis is idle (== 0) -> Spin Left
    else if(leftRightPos < 0) 
    {
      spinLeft(leftRightPos * -1);
    }

    //Stops the while(true) loop, in case something goes wrong and robot goes crazy
    if(stopProgram().pressing()) break;
  }
}

int main()
{
  vexcodeInit();
  manualControl();
  if(stopProgram().pressing()) return 0;
}
