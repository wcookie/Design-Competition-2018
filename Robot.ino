/*
 * The struct ontains our robot information like our current position, our state,
 * Our current block we are going for, our robot's block, and other viable things.
 * One thing we may add is whether we are an attack our defense robot.  
 * This (mostly) contains functions relating to driving, etc.
 */


 // Motors pins

 int leftPWM = 21; // Left pwm pin
 int leftDIR = 17; // Left digital pins
 int rightPWM = 20; // Right pwm pin
 int rightDIR = 16; //Right digital pin

 int modePin = 15; //enable/mode pin of both hbridges

 int teamSwitchD = 12; // Switch to tell us which team we're on //TBD
 int attackSwitchD = 13; // Switch to tell us if we're scoring or defending //TDB1q


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

void motorSetup() {
  /*
   * Sets the motor pins to OUTPUT
   */
  pinMode(leftA, OUTPUT);
  pinMode(leftD, OUTPUT);
  pinMode(rightA, OUTPUT);
  pinMode(rightD, OUTPUT);
}

 void moveMotors(int leftSpeed, bool leftDirection, int rightSpeed, bool rightDirection) {
  /*
   * moves motors based on given left/right speed and direction
   * True is forwards for direction, False is backwards
   * speeds go between 0 and 255
   */
  analogWrite(leftA, leftSpeed);
  digitalWrite(leftD, leftDirection);
  analogWrite(rightA, rightSpeed);
  digitalWrite(rightD, rightDirection);
}
