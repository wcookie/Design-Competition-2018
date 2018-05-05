/*
 * The struct ontains our robot information like our current position, our state,
 * Our current block we are going for, our robot's block, and other viable things.
 * One thing we may add is whether we are an attack our defense robot.  
 * This (mostly) contains functions relating to driving, etc.
 */


 // Motors pins

 int leftA = 4; // Left analog pin
 int leftD = 6; // Left digital pins
 int rightA = 5; // Right analog pin
 int rightD = 7; //Right digital pin

 int teamSwitchD = 12; // Switch to tell us which team we're on


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
