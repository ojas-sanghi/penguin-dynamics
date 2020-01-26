/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Saptarshi Mallick, Ojas Sanghi, Khoa Ho                   */
/*    Created:      Sat Jan 25 2020                                           */
/*    Description:  VexCode V5 Text Program for Penguin Dynamics              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 10           
// LeftIntake           motor         13              
// RightIntake          motor         20              
// ReverseStack         motor         16              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;

//<-------------------------Global Variables------------------------->

vex::competition Competition;

controller::axis axisDrive = Controller1.Axis2;
controller::axis axisTurn = Controller1.Axis4;

controller::button bIntake = Controller1.ButtonL2;
controller::button bOuttake = Controller1.ButtonR2;
controller::button bReverseStack = Controller1.ButtonUp;

bool inPressed = false;
bool outPressed = false;

//<--------------Basic Robot Functions and Stuff Code---------------->

/*
  Stops the drivetrain from moving
*/
void stopDrive()
{
  Drivetrain.setStopping(brake); //Options: brake, coast, hold
  Drivetrain.stop();
}

/*
  Sets the Driving Velocity to the parameter
  Limits the velocity to a range between a configured range so movements are maybe smoother
*/
void setDriveVel(int vel)
{
  int realVel = std::abs(vel);

  //Change the velocity to some value between the configured range
  int rangeMax = 90;
  int rangeMin = 10;

  realVel = realVel > rangeMax ? rangeMax : (realVel < rangeMin ? rangeMin : realVel);

  Drivetrain.setDriveVelocity(realVel, percent);
}

/*
  Sets the Turning Velocity to the parameter
  Limits the velocity to a range between a configured range so movements are maybe smoother
*/
void setTurnVel(int vel)
{
  int realVel = std::abs(vel);

  //Change the velocity to some value between the configured range
  int rangeMax = 90;
  int rangeMin = 10;

  realVel = realVel > rangeMax ? rangeMax : (realVel < rangeMin ? rangeMin : realVel);

  Drivetrain.setTurnVelocity(realVel, percent);
}

/*
  Drives (Forward and Backward, Axis 2)
*/
void drive(int axisPos)
{
  directionType driveDir;

  if(axisPos > 0) driveDir = vex::forward;
  else if(axisPos < 0) driveDir = vex::reverse;
  else return;

  Drivetrain.drive(driveDir);
}

/*
  Turns (Left and Right, Axis 4)
*/
void turn(int axisPos)
{
  turnType turnDir;

  if(axisPos > 0) turnDir = vex::right;
  else if(axisPos < 0) turnDir = vex::left;
  else return;
  
  Drivetrain.turn(turnDir);
}

/*
  Turns the intakes so that they spin blocks inwards
  Velocity is set in the main method
*/
void activateIntake()
{
  LeftIntake.spin(forward);
  RightIntake.spin(reverse);
}

/*
  Turns the intakes so that they spin blocks outwards
  Velocity is set in the main method
*/
void activateOuttake()
{
  LeftIntake.spin(reverse);
  RightIntake.spin(forward);
}

/*
  Following methods safely stop the Intakes
*/

//Stop intake gears (only directly called in autonomous)
void stopIntakes()
{
  LeftIntake.stop();
  RightIntake.stop();
}

//Stop intake gears, but check for the buttons as well
void stopIntakesManual()
{
  if(!inPressed && !outPressed) stopIntakes();
}

//Release callback for Intake
void deactivateIntake()
{
  inPressed = false;
  stopIntakesManual();
}

//Release callback for Outtake
void deactivateOuttake()
{
  outPressed = false;
  stopIntakesManual();
}

/*
  Turns on the reverse stack motor, so blocks move outward
  Velocity is set in the main method
*/
void activateReverseStack()
{
  ReverseStack.spin(forward);
}

/*
  Deactivates the reverse stack motor
*/
void deactivateReverseStack()
{
  ReverseStack.stop();
}

/*
  Stops all motors (Drivetrain and Intake)
*/
void stopAll()
{
  stopDrive();
  stopIntakes();
  deactivateReverseStack();
}


//<--------------Competition-Related Code---------------->

/*
  To be run during manual control period
*/
void manual()
{
  //Counts loop times
  int counter = 0;

  //Initializing axis position variables
  int driveAxisPos, turnAxisPos;

  //Setting default velocities
  int gearVel = 100;
  int drivetrainVel = 85;

  setDriveVel(drivetrainVel);
  setTurnVel(drivetrainVel);
  RightIntake.setVelocity(gearVel, percent);
  LeftIntake.setVelocity(gearVel, percent);
  ReverseStack.setVelocity(gearVel, percent);

  //Setting brake modes
  LeftIntake.setStopping(hold);
  RightIntake.setStopping(hold);

  //Real Controls
  while(true)
  {
    counter++;

    //Update axis positions
    driveAxisPos = axisDrive.position();
    turnAxisPos = axisTurn.position();

    //Check if neither driving-related axis is in use
    if(driveAxisPos == 0 && turnAxisPos == 0) stopDrive();

    //Drive
    if(driveAxisPos != 0) drive(driveAxisPos);

    //Turn
    if(turnAxisPos != 0) turn(turnAxisPos);

    //Intakes and Reverse Stack
    bIntake.pressed(activateIntake);
    bOuttake.pressed(activateOuttake);
    bReverseStack.pressed(activateReverseStack);

    bIntake.released(deactivateIntake);
    bOuttake.released(deactivateOuttake);
    bReverseStack.released(deactivateReverseStack);

    //Tick Time
    waitUntil(!Drivetrain.isMoving());

    setDriveVel(driveAxisPos);
    setTurnVel(turnAxisPos);
  }
}

/*
  To be run during autonomous control period
*/
void auton()
{
  setDriveVel(75);
  setTurnVel(100);

  //Turn on outtake
  activateOuttake();

  //Drop off one block and return
  Drivetrain.driveFor(forward, 24, inches);
  wait(1500, msec);
  Drivetrain.driveFor(reverse, 30, inches);

  //Reorient to face forwards
  Drivetrain.turnFor(270, degrees);

  /* Experimental Old Stuff (Don't we want to change strategy??)
    // Stop output motors
    stopAllIntakes();

    // Turn 180, take in block
    Drivetrain.turnFor(left, 160, degrees);
    turnIntake();
    Drivetrain.driveFor(forward, 30, inches);

    // Stop input motors
    stopAllIntakes();
  */
}

/*
  NOT FOR USE IN OFFICIAL COMPETITION
  This is for when we want to simulate a competition, for testing purposes
*/
void testAutonManual()
{
  Brain.Screen.print("Starting Autonomous");

  auton();

  Brain.Screen.clearScreen();
  Brain.Screen.print("Starting Manual");
  wait(500, msec);

  manual();
  
  Brain.Screen.clearScreen();
}

/*
  Quick Reference of all method names and what they do:

  stopDrive() – stop only the wheel motors
  stopIntakes() – stop intake gears w/o checking for the buttons
  stopIntakesManual() – stop intake gears by checking for the buttons
  stopAll() – stops all motors: wheels, intakes, reverse stack

  setDriveVel() – set drive velocity
  setTurnVel() – set turn velocity

  drive() – drive w/Axis 2
  turn() – turn w/Axis 4

  activateIntake() – turn on intake (blocks go in)
  deactivateIntake() – turn off intake

  activateOuttake() – turn on outtake (blocks go out)
  deactivateOuttake() – turn off outtake

  activateReverseStack() – turn on rev. stack (blocks go out the back)
  deactivateReverseStack() – turn off rev. stack

  manual() – Driver Control period
  auton() – Autonomous Control Period
  testAutonManual() – when we practice, this runs both auton then manual right after 

  Vars (no more parantheses need, ex. axisDrive() is now axisDrive):
  Competition – competition object

  axisDrive – driving axis
  axisTurn – turning axis

  bIntake – button for intake (blocks go in)
  bOuttake – button for outtake (blocks go out)
  bReverseStack – button for reverse stack (blocks go out back)
*/

int main() 
{
  //Initializing robot configuration, must be run first
  vexcodeInit();

  //Set Intake and Reverse Stack Motor Velocities
  int gearVel = 100;

  RightIntake.setVelocity(gearVel, percent);
  LeftIntake.setVelocity(gearVel, percent);
  ReverseStack.setVelocity(gearVel, percent);

  //Uncomment for specific testing purposes
  //testAutonManual();

  //Needed for Competition
  Competition.autonomous(auton);
  Competition.drivercontrol(manual);

  return 0;
}
