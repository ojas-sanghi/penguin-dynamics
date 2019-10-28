/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\1301261566                                       */
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

void setTotalVelocity(int perc)
{
  RightMotor.setVelocity(perc, percent);
  LeftMotor.setVelocity(perc, percent);
}

/*
Drives robot forward or backward
*/
void drive(bool forward) 
{
  directionType dir;

  if(forward) dir = vex::forward;
  else dir = reverse;

  RightMotor.spin(dir);
  LeftMotor.spin(dir);
}

void stopAll()
{
  RightMotor.stop();
  LeftMotor.stop();
}

void turnLeft()
{
  LeftMotor.stop();
  RightMotor.spin(forward);
}

void turnRight()
{
  RightMotor.stop();
  LeftMotor.spin(forward);
}

int main()
{
  vexcodeInit();

  while(true)
  {

    //Forward
    if(Controller1.ButtonR2.pressing() && Controller1.ButtonL2.pressing()) 
    {
      drive(true);
    } else {
      Controller1.ButtonR2.released(stopAll);
      Controller1.ButtonL2.released(stopAll);
    }

    //Reverse
    if(Controller1.ButtonR1.pressing() && Controller1.ButtonL1.pressing()) 
    {
      drive(false);
    } else { Controller1.ButtonR1.released(stopAll);
      Controller1.ButtonL1.released(stopAll);
    }    

    //Right
    if(Controller1.ButtonA.pressing()) 
    {
      turnRight();
    } else { 
      Controller1.ButtonA.released(stopAll);
    }

    //Left
    if(Controller1.ButtonY.pressing()) 
    {
      turnLeft();
    } else { 
      Controller1.ButtonY.released(stopAll);
    }


    // ALL STOP
    if (Controller1.ButtonX.pressing()) stopAll();
  }
}