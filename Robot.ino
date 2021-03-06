/*
 * The struct ontains our robot information like our current position, our state,
 * Our current block we are going for, our robot's block, and other viable things.
 * One thing we may add is whether we are an attack our defense robot.  
 * This (mostly) contains functions relating to driving, etc.
 */
#define CYLINDER_THRESHOLD 400 // If we are below this mark, we believe we are carrying a cylinder

// How far away we want to be from our desired point
#define DISTANCE_THRESHOLD 5

// Angle difference
#define FAST_ANGLE_DIFFERENCE .15
#define SLOW_ANGLE_DIFFERENCE .02

// Speeds
#define STANDARD_SPEED 100
#define SMALLER_SPEED 60
#define FAST_TURNING_SPEED 60
#define SLOW_TURNING_SPEED 35
#define CUBE_SPEED 110
#define CYLINDER_SPEED 150

long long lastSeenBlock = 0; // Used to help us determine if we are seeing a block


 // Motors pins

 int leftPWM = 3; // Left pwm pin
 int leftDIR = 2; // Left digital pins
 int rightPWM = 6; // Right pwm pin
 int rightDIR = 7; //Right digital pin

 int teamSwitchD = 12; // Switch to tell us which team we're on //TBD
 int attackSwitchD = 13; // Switch to tell us if we're scoring or defending //TBD


void lastSeenSetup() {
  /*
   * Sets the last time we saw a block to be millis
   */
   lastSeenBlock = millis();
}

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
   r.inOrOut = robotEllipseState(r);
}

blockType whatAreWeHolding(Robot r) {
  /*
   * Determines whether we are holding a cylinder or a cube.
   * Should only be called if we are holding something
   * First drives for 400 milliseconds to normalize for current speed stuff.
   * Then says if we are reading a value smaller then CYLINDER_THRESHOLD, 
   * return cylinder
   * otherwise return cube
   */
   moveMotors(STANDARD_SPEED, true, STANDARD_SPEED, true);
   delay(400);
   if (readCurrentSensor() < CYLINDER_THRESHOLD) {
    return cylinder;
   } else {
    return cube;
   }
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

void dropOffBlock(Robot& r) {
  /*
   * For when we are holding our goal block.
   * Goes straight to the goal, checks for when we are by the center of the goal, 
   * then stops, backs up, determines new block, switches mode to orienting
   */
   printDebugging(r);
   Serial.println("WE ARE IN DROP OFF BLOCK");
   bool inGoal = false;
   if (r.team == circle) {
    inGoal = inCircleGoal(r);
   } else {
    inGoal = inSquareGoal(r);
   }
   if (inGoal) {
    // What we're doing if we are in the goal
    // Back up for 2 seconds
    backUpRoutine(2000);
    // Turn motors off
    turnMotorsOff();
    // Determine our next block
    r.desiredBlock = determineBestBlock(r);
    // Set mode to orienting
    r.driving = orienting;
    // Finish
    return;
   }
   // If we aren't in the goal, drive straight.
   // TODO: Adjust our driving angle if it's off.
   // TODO: Make sure we are still carrying the block.
   if (r.team == circle) {
    moveMotors(CYLINDER_SPEED, HIGH, CYLINDER_SPEED, HIGH);
   } else {
    moveMotors(CUBE_SPEED, HIGH, CUBE_SPEED, HIGH);
   }
}

void discardEnemyBlock(Robot& r) {
  /*
   * Discards Enemy block
   * Basically moves forward for 2 seconds and backwards for 2
   * We will move slowly so for a cylinder we will go at standard speed 
   * and for a cube go at smaller speed
   * Determine new block;
   * Set our state after to orienting
   * TODO(wcookie): Set block state from our array of blocks
   */
   printDebugging(r);
   Serial.println("WE ARE IN DISCARD BLOCK");
   double discardSpeed;
   if (r.team == square) {
    discardSpeed = STANDARD_SPEED; // We will go at our normal speed bc cylinders still fat
   } else {
    discardSpeed = SMALLER_SPEED; // Going slower for a cube
   }
   // Do this for 2 seconds forwards then backwards
   moveMotors(discardSpeed, 1, discardSpeed, 1);
   delay(2000);
   // Go Backwards
   backUpRoutine(2000);
   // Turn off motors
   turnMotorsOff();
   // Get our goal block
   r.desiredBlock = determineBestBlock(r);
   // Now set into orienting mode
   r.driving = orienting;
}

void moveTowardsBlock(Robot& r) {
  /*
   * Moves towards a block.  
   * This should only trigger if we are somewhat facing the block.
   * First, see if we have managed to capture the block.  If so move to the correct 
   * state
   * Next, check if we are reading the block.  
   * If we are, move straight.
   * If we aren't, turn side to side slightly until we see it again. 
   * TODO: Add a timer to just give up on this block and determine it to be not there.
   * Once we get the block, 
   * if we have goal block and plan on going straight to the goal, end in holdingGoalBlock mode
   * if we have goal block and plan on orienting more, end in orientingWithBlock mode
   * If we have enemy block, end in holdingEnemyBlock mode.
   */
   printDebugging(r);
   Serial.println("WE ARE IN MOVE TOWARDS BLOCK");

   // Make sure the trip wire is on :)
    turnOnTripwire();
    delay(1000);
    readingBlock(false);
    Serial.println("Before holding block check");
    
   // If we are holding the block we are gonna be done
   if (holdingBlock()) {
    Serial.println("We think we have a block");
    if (((whatAreWeHolding(r) == cube) && (r.team == square)) ||  \
        ((whatAreWeHolding(r) == cylinder) && (r.team == circle))) {
          // We have our goal block
          // If we are in the moveBlockOrientState, 
          // then we will say orient with block, 
          // otherwise say we are holding goal block
          if (r.approach == moveBlockOrient) {
            r.driving = orientingWithBlock;
          } else {
            r.driving = holdingGoalBlock;
          }
          // Turn off motors and tripwire
          turnOffTripwire();
          turnMotorsOff();
          r.desiredBlock.state = moved;
          return;
    } else {
      Serial.println("We don't think we have one");
      // We have the wrong one
      r.driving = holdingEnemyBlock;
      // Turn off motors and tripwire
      turnMotorsOff();
      turnOffTripwire();
      r.desiredBlock.state = invisible;
      return;
    }
   }
   // If we are reading the block, then we want to go straight
   if (readingBlock(false)) {
    // We read the block
    r.desiredBlock.state = seen;
    // TODO: Make sure the angle kinda makes sense for where we want to be going
    moveMotors(STANDARD_SPEED, true, STANDARD_SPEED, true);
    return;
   }
   // If we don't read the block orient ourselves.
}

void orientRobot(Robot& r) {
  /*
   * Does several steps.  First determines where to go to move block.
   * Then rotates towards this point.
   * Then drives there.  Then rotates towards the block.
   * Then enters movingTowardsBlock state.
   */
   Serial.println("WE ARE IN ORIENT ROBOT");
   // Get our point based on our approach
   Point desiredPoint;
   if (r.approach == straightApproach) {
    desiredPoint = desiredOrientationPointStraight(r, r.desiredBlock, r.goalPos);
   }
   
   // Add statement for if we are doing a moving orient.
   // Rotate towards our point
   Serial.println("Angling to point");
   printDebugging(r);
   double angle = desiredAngle(r, desiredPoint);
   rotateToAngle(r, angle);
   printDebugging(r);
   // Go towards our point
   Serial.println("driving to point");
   driveTowardsPoint(r, desiredPoint); 
   printDebugging(r);
   // Now we are at our point we want to move to
   // Just rotate to our goal block, then we switch to the moveTowardsBlock state
   Serial.println("Setting angle to face the cylinder");
   
   setRobotPositionAndDirection(r);
   double blockAngle = desiredAngle(r, r.desiredBlock.pos);
   Serial.println(blockAngle);
   rotateToAngle(r, blockAngle);
   turnMotorsOff();
   printDebugging(r);
   // We should orient and stuff but for now just go to moveTowardsBlock
   Serial.println("move towards block");
   r.driving = movingTowardsBlock;
}

void orientWithBlock(Robot& r) {
  /*
   * This orients the robot if we already have the block 
   * so that we can face straight towards a goal.
   * TODO(wcookie): make sure we're still holding our dude
   * Ends in holdingGoalBlock mode
   */
   // First find our desired point
   Serial.println("WE ARE IN ORIENT WITH BLOCK");
   Point desiredPoint = desiredOrientationPointOffset(r, r.desiredBlock, r.goalPos);
   driveTowardsPoint(r, desiredPoint);
   //Now we rotate towards our goal position (with the block)
   double angle = desiredAngle(r, r.goalPos);
   rotateToAngle(r, angle);
   // Now we want to give it off to dropOffBlock (holdingGoalBlock state) 
   // First turn off motors
   turnMotorsOff();
   r.driving = holdingGoalBlock;
}

void backUpRoutine(int t) {
  /*
   * Our routine that goes backwards slowly for some time t
   */
  moveMotors(SMALLER_SPEED, 0, SMALLER_SPEED, 0);
  delay(t);
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

void turnMotorsOff() {
  /*
   * Turns off motors
   */
  moveMotors(0, HIGH, 0, HIGH);
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

void rotateToAngle(Robot& r, double goalAngle) {
  /*
   * This just does a while loop until rotateToAngleDecision returns true
   * THIS IS A BLOCKING FUNCTION
   */
   // loops until rotateToAngleDecision is true
   while(!rotateToAngleDecision(r, goalAngle)) {
    ;
   }
}

bool rotateToAngleDecision(Robot& r, double goalAngle) {
  /*
   * This is the secondary rotate to angle function.
   * First goes at a faster speed.  Then will do it again
   * At a slower speed to get speed AND precision
   * @sourpatchkid9's idea once again
   * 
   * At least FOR NOW, this is a BLOCKING FUNCTION
   */
   setRobotPositionAndDirection(r);
   if (r.turnSpeed == fast){
    rotateToAngleHelper(r, goalAngle, FAST_TURNING_SPEED);
    return false;
   }else {
    return rotateToAngleHelper(r, goalAngle, SLOW_TURNING_SPEED);
   }
}

bool rotateToAngleHelper(Robot& r, double goalAngle, double turningSpeed) {
  /*
   * Helper to help
   * Rotates our robot to a desired angle
   * basically until we're like .1 off
   * stops when we are.  This will turn at a desired speed.
   * 
   * Returns true once we are there
   */
   double angleDiff;
   if (r.turnSpeed == fast) {
    angleDiff = FAST_ANGLE_DIFFERENCE;
   } else {
    angleDiff = SLOW_ANGLE_DIFFERENCE;
   }
   if (abs(r.heading - goalAngle) < angleDiff) {
    turnMotorsOff();
    if (r.turnSpeed == fast) {
      r.turnSpeed = slow;
    } else {
      r.turnSpeed = fast;
    }
    return true;
   }
   if (r.heading > goalAngle) {
    moveMotors(turningSpeed, 1, turningSpeed, 0);
   } else {
    moveMotors(turningSpeed, 0, turningSpeed, 1);
   }
   return false;
}

void driveTowardsPoint(Robot& r, Point p) {
  /*
   * Drives our robot to (the vicinity of) our desired point
   * For now just goes straight.  Should be checking our angle constantly really
   * BLOCKING
   */
   // Calls the helper until it's true
   while(!driveTowardsPointHelper(r, p)){
    ;
   }
}

bool driveTowardsPointHelper(Robot& r, Point p) {
  /*
   * Does the physical driving
   */  
   setRobotPositionAndDirection(r);
   // If our distance is within threshold return true
   if (distance(p, r.pos) < DISTANCE_THRESHOLD) {
    return true;
   }
   // If our angle is too far off, re adjust
   if (abs(desiredAngle(r, p) - r.heading) > .12) {
    // If we are greater than .12, we are gonna call rotate to our desired angle
    rotateToAngle(r, desiredAngle(r, p));
   }

   // Otherwise drive straight
   // First determine speed.  
   // If we are in hodlingGoalBlock mode, or orienting mode, do standard speed.
   double ourSpeed;
   if ((r.driving == holdingGoalBlock) || (r.driving == orienting)) {
    ourSpeed = STANDARD_SPEED;
   } else if (r.team == circle) {
    // If we are on team circle, we have to be holding our cylinder block, so set to cylinder speed
    ourSpeed = CYLINDER_SPEED;
   } else {
    // We are on team cube and we want to use cube speed
    ourSpeed = CUBE_SPEED;
   }
   // Now we have our speed, let's drive at that speed straight.  (we should be checking our angle and be rotating every so often
   moveMotors(ourSpeed, 1, ourSpeed, 1);
   return false;
}

