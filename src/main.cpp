/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\1301261566                                       */
/*    Created:      Thu Nov 21 2019                                           */
/*    Description:  Intake                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 10           
// LeftIntake           motor         13              
// RightIntake          motor         20              
// StackMotor           motor         16              
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

controller::button bIntake() {return Controller1.ButtonL2;}
controller::button bOutput() {return Controller1.ButtonR2;}

controller::button bStack() {return Controller1.ButtonUp;}

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

void turnIntake() {
  LeftIntake.spin(forward);
  RightIntake.spin(reverse);
}

void turnOutput() {
  LeftIntake.spin(reverse);
  RightIntake.spin(forward);
}

// Stopping methods for the intake motors

bool inPressed = false;
bool outPressed = false;

void appropriatelyStopIntake() {
  if (!inPressed && !outPressed) {
    LeftIntake.stop();
    RightIntake.stop();
  }
}

void releaseIn() {
  inPressed = false;
  appropriatelyStopIntake();
}

void releaseOut() {
  outPressed = false;
  appropriatelyStopIntake();
}

// Not preferred, only used in auton, when we know for a fact we want to stop all the motors
void stopAllIntakes() {
  LeftIntake.stop();
  RightIntake.stop();
}

// Movement methods for the stacking motor

void turnStackMotor() {
  StackMotor.spin(forward);
}

void stopStackMotor() {
  StackMotor.stop();
}

/*
  Method to be called during manual control period
*/
void manualControl()
{
  int driveAxisPos, turnAxisPos;

  setDriveVel(defaultVel());
  setTurnVel();

  LeftIntake.setStopping(hold);
  RightIntake.setStopping(hold);

  // Note: intake motors' velocities are set in main()
  StackMotor.setVelocity(100, percent);

  while(true)
  {
    //Stop!
    if(bEnd().pressing()) break;

    //Update axis positions
    driveAxisPos = axisDrive().position();
    turnAxisPos = axisTurn().position();

    //If neither axis is in use, don't do anything
    if(driveAxisPos == 0 && turnAxisPos == 0) stopAll();
    
    //Drive
    if(driveAxisPos != 0) drive(driveAxisPos);

    //Turn
    if(turnAxisPos != 0) turn(turnAxisPos);

    //Intake
    bIntake().pressed(turnIntake);
    bOutput().pressed(turnOutput);

    bIntake().released(releaseIn);
    bOutput().released(releaseOut);

    // Stacking motor
    bStack().pressed(turnStackMotor);

    bStack().released(stopStackMotor);

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
  setDriveVel(75);

  // Begin turning output motors
  turnOutput();

  // Drop off block
  Drivetrain.driveFor(forward, 24, inches);
  wait(2000, msec);
  Drivetrain.driveFor(reverse, 30, inches);

  // Stop output motors
  stopAllIntakes();

  // Turn 180, take in block
  Drivetrain.turnFor(left, 160, degrees);
  turnIntake();
  Drivetrain.driveFor(forward, 30, inches);

  // Stop input motors
  stopAllIntakes();
}

int main() 
{
  vexcodeInit();

  LeftIntake.setVelocity(100, percent);
  RightIntake.setVelocity(100, percent);

  //autonomousControl();
  //manualControl();
  
  Competition.autonomous(autonomousControl);
  Competition.drivercontrol(manualControl);
  return 0;
}

