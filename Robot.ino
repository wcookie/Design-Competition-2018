/*
 * The struct ontains our robot information like our current position, our state,
 * Our current block we are going for, our robot's block, and other viable things.
 * One thing we may add is whether we are an attack our defense robot.  
 * This (mostly) contains functions relating to driving, etc.
 */

#define CYLINDER_THRESHOLD 400 // If we are below this mark, we believe we are carrying a cylinder


 // Motors pins

 int leftPWM = 3; // Left pwm pin
 int leftDIR = 2; // Left digital pins
 int rightPWM = 6; // Right pwm pin
 int rightDIR = 7; //Right digital pin

 int teamSwitchD = 12; // Switch to tell us which team we're on //TBD
 int attackSwitchD = 13; // Switch to tell us if we're scoring or defending //TBD


goalType getTeam() {
  /*
   * Reads the switch, and returns the correct team.
   * HIGH == circle
   * LOW == Square
   */
  pinMode(teamSwitchD, INPUT);
  int switchVal = digitalRead(teamSwitchD);
  if (switchVal == HIGH) {
    return circle;
  } else {
    return square;
  }
}

attackState getAttackState() {
  /*
   * Reads the attack state switch, and returns whether we are scoring or defending
   * HIGH == Scoring
   * LOW == defending
   */
   pinMode(attackSwitchD, INPUT);
   int switchVal = digitalRead(attackSwitchD);
   if (switchVal == HIGH) {
    return scoring;
   } else {
    return defending;
   }
}

void setRobotPositionAndDirection(Robot& r) {
  /*
   * Takes in a robot and sets its position and heading by calling the vive sensors code
   */
   RawViveData rvd = readViveSensors();
   r.heading = rvd.heading;
   // this averages the x values from the two sensors.
   double rawXCenter = (rvd.v1LightPoint.x + rvd.v2LightPoint.x) / 2;
   double rawYCenter = (rvd.v1LightPoint.y + rvd.v2LightPoint.y) / 2;
   r.pos = physicalPointToVirtualPoint(LightPoint(rawXCenter, rawYCenter));
}

drivingState determineBlockHolding(Robot& r) {
  /*
   * Takes in a robot, and checks tripwire and current sensor to determine block fit
   * Right now just does exact calculation, might want to say if we were tripped last time 
   * and thought it was cylinder, but still tripped and not reading cylinder, that we may still
   * want to consider us as having a cylinder.
   */
   if (holdingBlock()) {
    // We are holding a block determine which one it is
    if (readCurrentSensor() < CYLINDER_THRESHOLD) {
      // We are holding a cylinder, then determine whether that's our goal or not.
      if (r.team == circle) {
        // we are aiming for a cylinder
        r.driving = holdingGoalBlock;
        return holdingGoalBlock;
      } else {
        r.driving = holdingEnemyBlock;
        return holdingEnemyBlock;
      }
    } else {
      // We have a cube
      if (r.team == square) {
        // We are aiming for a cube
        r.driving = holdingGoalBlock;
        return holdingGoalBlock;
      } else {
        r.driving = holdingEnemyBlock;
        return holdingEnemyBlock;
      }
    }
   } else {
    // We are not holding a block right now
    // If we weren't a second ago, do not change state
    // TODO:  add a function to check if we are near goal, if we are, then we can say we prob
    // dropped it off.  If not, then we don't want to change our state unless this happens for a while
    return other;
   }
   
}

void determineRobotState(Robot& r) {
  /*
   * This function takes in a robot, and determines what it's state should be.
   * State reminder: holding goal block, holding enemy block, moving towards block, orienting
   */
   // Our first priority is to see whether we are holding a block.  If we are, then we stop.
   drivingState ds = determineBlockHolding(r);
   if (ds != other) {
    // If we found that we are holding enemy block or holding our goal block, we leave.
    return; 
   }
   // Now we want to see if we are going towards a block
   // TODO (JCohner): Include the line of sight we have bc of our angle and position, and make sure
   // we are in the vicinity of pointing at our goal block.
   if (readingBlock(false)) {
    r.driving = movingTowardsBlock;
    return;
   }
   // Now if we don't see a block we should be orienting.  
   // TODO (JCohner): Add in our code that if we've been orienting for a bit (not tooo long)
   // And we don't see anything that we should move on to a new block
   r.driving = orienting; 
}

void motorSetup() {
  /*
   * Sets the motor pins to OUTPUT
   */
  pinMode(leftPWM, OUTPUT);
  pinMode(leftDIR, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(rightDIR, OUTPUT);
}

 void moveMotors(int leftSpeed, bool leftDirection, int rightSpeed, bool rightDirection) {
  /*
   * moves motors based on given left/right speed and direction
   * True is forwards for direction, False is backwards
   * speeds go between 0 and 255
   */
  analogWrite(leftPWM, leftSpeed);
  digitalWrite(leftDIR, leftDirection);
  analogWrite(rightPWM, rightSpeed);
  digitalWrite(rightDIR, rightDirection);
}
