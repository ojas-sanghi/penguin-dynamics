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
// LeftIntake           motor         13              
// RightIntake          motor         20              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

vex::competition Competition;

//Main vars
int defaultVel() {return 75;}
int robotVel(){return Drivetrain.velocity(percent);}

  //Axes
controller::axis axisDrive() {return Controller1.Axis2;}
controller::axis axisTurn() {return Controller1.Axis4;}
  //Stop program running
controller::button bEnd() {return Controller1.ButtonY;}

controller::button bIntakeForward() {return Controller1.ButtonA;}
controller::button bIntakeBackward() {return Controller1.ButtonB;}

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

void turnIntakeGears(bool outwards = false)
{
  if(!outwards)
  {
    LeftIntake.spin(forward);
    RightIntake.spin(reverse);
  }
  else
  {
    LeftIntake.spin(reverse);
    RightIntake.spin(forward);
  }
}

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

    if(bIntakeForward().pressing()) turnIntakeGears();
    else if(bIntakeBackward().pressing()) turnIntakeGears(true);

    //Update axis positions
    driveAxisPos = axisDrive().position();
    turnAxisPos = axisTurn().position();

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
  turnIntakeGears();
  Drivetrain.driveFor(24, inches);
  Drivetrain.turnFor(90, degrees);
}

int main() 
{
  vexcodeInit();
  Competition.drivercontrol(manualControl);
  Competition.autonomous(autonomousControl);
  return 0;
}
