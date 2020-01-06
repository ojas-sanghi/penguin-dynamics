/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\1301261566                                       */
/*    Created:      Thu Nov 21 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 10           
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//Main vars
int defaultVel() {return 75;}
int robotVel(){return Drivetrain.velocity(percent);}

  //Axes
controller::axis axisDrive() {return Controller1.Axis2;}
controller::axis axisTurn() {return Controller1.Axis4;}
  //Buttons for increasing and decreasing the robot velocity 
controller::button bIncrVel() {return Controller1.ButtonUp;}
controller::button bDecrVel() {return Controller1.ButtonDown;}
  //Buttons for either setting the robot to velocity 0 (stop), velocity 100 (max), or velocity = default (default)
controller::button bMaxVel() {return Controller1.ButtonX;}
controller::button bZeroVel() {return Controller1.ButtonB;}
controller::button bDefaultVel() {return Controller1.ButtonA;}
  //Stop program running
controller::button bEnd() {return Controller1.ButtonY;}

/*
  Stops all motors
  @param instant Optional parameter to change the method of braking (hold means the wheels stop instantly, brake is the wheels slow down gradually (normal behavior))
*/
void stopAll(bool instant = false)
{
   Drivetrain.setStopping(instant ? hold : brake);
  Drivetrain.stop();
}

/*
  CURRENTLY UNUSED
  Limits the velocity from 10 to 90
*/
int limitedVel(int vel)
{
  return vel <= 10 ? 20 : (vel >= 90 ? 90 : vel);
}

/*
  Sets the drive velocity to the parameter
*/
void setDriveVel(int vel = 0)
{
  int absVel = vel < 0 ? vel * -1 : vel;
  
  if(absVel == 0) return;
  
  Drivetrain.setDriveVelocity(absVel, percent);
}

/*
  Sets the turn velocity to the parameter
*/
void setTurnVel(int vel = 100)
{
  int absVel = vel < 0 ? vel * -1 : vel;

  if(absVel == 0) return;

  Drivetrain.setTurnVelocity(absVel, percent);
}

/*
  Drives forward or backward
*/
void drive(int vel = 0)
{
  directionType driveDir;

  if(vel == 0) return;
  else if(vel > 0) driveDir = vex::forward;
  else driveDir = vex::reverse;

  Drivetrain.drive(driveDir);
}

/*
  Turns left or right
*/
void turn(int vel = 0)
{
  turnType turnDir;

  if(vel == 0) return;
  else if (vel > 0) turnDir = vex::right;
  else turnDir = vex::left;

  Drivetrain.turn(turnDir);
}

bool incr() {return bIncrVel().pressing();}
bool decr() {return bDecrVel().pressing();}
bool max() {return bMaxVel().pressing();}
bool zero() {return bZeroVel().pressing();}

/*
  Method to be called during manual control period
*/
void manualControl()
{
  int driveAxisPos, turnAxisPos;

  setDriveVel(defaultVel());
  setTurnVel();

  while(true)
  {
    //Stop!
    if(bEnd().pressing()) break;

    //Update axis positions
    driveAxisPos = axisDrive().position();
    turnAxisPos = axisTurn().position();

    //Velocity Modifying Buttons - if any are clicked the while loop will skip driving and stuff
    if(incr()) driveAxisPos += 5;
    if(decr()) driveAxisPos -= 5;
    if(max()) driveAxisPos = 100;
    if(zero()) stopAll();

    if(incr() || decr() || max() || zero())
    {
      Controller1.rumble(rumbleShort);
      continue;
    }

    //If neither axis is in use, don't do anything
    if(driveAxisPos == 0 && turnAxisPos == 0) stopAll();

    //Drive
    if(driveAxisPos != 0) drive(driveAxisPos);

    //Turn
    if(turnAxisPos != 0) turn(turnAxisPos);

    //Tick Time
    waitUntil(!Drivetrain.isMoving());

    setDriveVel(driveAxisPos);
    setTurnVel(turnAxisPos);
  }
}

/*
  Method to be called during autonomous period
*/
void autonomousControl()
{

}

int main() 
{
  vexcodeInit();
  manualControl();
  /* Whenever ready
  Competition.drivercontrol(manualControl); 
  Competition.autonomous(autonomousControl);
  */

  manualControl();
  return 0;
}
