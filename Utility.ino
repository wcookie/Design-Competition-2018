// Tab for miscalleneous functions / utility
#define BLOCK_LASER_THRESHOLD 75 // If the difference is above this we see a block

int currentSensorPin1 = 14; // A0
int currentSensorPin2 = 15; // A1
int tripwirePin = 22; // A8
int blockPTPin = 18; // A2
int blockLaserPin2 = 21;
int blockLaserPin1 = 20;

double tripwireThreshold = 40; // If we are below this, we are tripped.  Maybe should be calibrated


void currentSensorSetup() {
  pinMode(currentSensorPin1, INPUT);
  pinMode(currentSensorPin2, INPUT);
}

void lasersSetup() {
  pinMode(tripwirePin, INPUT);
  pinMode(blockPTPin, INPUT);
  pinMode(blockLaserPin1, OUTPUT);
  pinMode(blockLaserPin2, OUTPUT);
}

double readCurrentSensor() {
  /*
   * Gets the current sensor values and averages them
   */
   double sum1 = 0.0;
   double sum2 = 0.0;
   double counter1 = 0.0;
   double counter2 = 0.0;
   for (int ii = 0; ii < 140; ++ii){
    // First sensor, making sure the values are reasonable
    double sensorVal1 = analogRead(currentSensorPin1);
    if ((sensorVal1 > 50) && (sensorVal1 < 950)) {
      counter1 += 1.0;
      sum1 += sensorVal1;
    }
    // Second sensor, making sure the values are reasonable
    double sensorVal2 = analogRead(currentSensorPin2);
    if ((sensorVal2 > 50) && (sensorVal2 < 950)) {
      counter2 += 1.0;
      sum2 += sensorVal2;
    }
    delay(1);
   }
   // Dividing sums by the number of times
   sum1 /= counter1;
   sum2 /= counter2;
   // Returning the average
   return (sum1 + sum2) / 2.0;
}

double readCurrentSensorBasic() {
  /*
   * basic version of above
   */
   return (analogRead(currentSensorPin1) + analogRead(currentSensorPin2)) / 2.0;
}

double readTripwire() {
  /*
   * reads the trip wire
   */
   return analogRead(tripwirePin);
}

bool holdingBlock() {
  /*
   * Determines whether we are holding a block or not based on tripwire. 
   * Might want to make this more robust ?? @JCohner
   * Less than because normally we will have laser hitting the phototransistor, and a block stops
   * the connection
   */
   return (readTripwire() < tripwireThreshold);
}

double readBlockLaser() {
  /*
   * reads the block laser
   */
  return analogRead(blockPTPin);   
}

void flashBlockLasers(bool mode) {
  digitalWrite(blockLaserPin1, mode);
  digitalWrite(blockLaserPin2, mode);
}

bool readingBlock(bool debug) {
  /*
   * reads the block laser PT and determines whether or not we can see a block
   * it does this by reading value a few times without lasers on then reading
   * again with the lasers on and seeing if there is a significant difference
   * Total delays = 10 ms
   */
   if(debug) {
    Serial.print("Threshold: ");
    Serial.println(BLOCK_LASER_THRESHOLD);
   }
   // Set the block laser to low
   flashBlockLasers(LOW);
   double darkSum = 0.0;
   for (int i = 0; i < 100; ++i) {
    darkSum += readBlockLaser();
    delay(1);
   }
   double darkAverage = darkSum / 100.0; // Our average with the lasers off
   if(debug) {
    Serial.print("Dark Average: ");
    Serial.println(darkAverage);
   }
   // Set the block laser to high 
   flashBlockLasers(HIGH);
   delay(4);
   double lightSum = 0.0;
   for (int i = 0; i < 100; ++i) {
    lightSum += readBlockLaser();
    delay(1);
   }
   double lightAverage = lightSum / 100.0;  // Our average with the lasers on
   if(debug) {
    Serial.print("Light Average: ");
    Serial.println(lightAverage);
   }
   // Set lasers back to off
   flashBlockLasers(LOW);
   // If our difference is greater than the threshold, then we are reading a block
   return (lightAverage > (darkAverage + BLOCK_LASER_THRESHOLD));
}

void printInEllipse(Robot r) {
  /*
   * Prints whether or not our robot is in the ellipse
   */
  Serial.print("Robot is ");
  if (robotEllipseState(r) == inside) {
    Serial.println("Inside the ellipse");
  } else {
    Serial.println("Outside the ellipse");
  }
}

void printRobotState(Robot r) {
  /*
   * Prints out the state of our robot
   */
   Serial.print("Robot State: ");
   switch(r.driving) {
    case holdingGoalBlock: Serial.println("Holding Goal block");
    case holdingEnemyBlock: Serial.println("Holding Enemy block");
    case movingTowardsBlock: Serial.println("Moving Towards block");
    case orienting: Serial.println("Orienting");
    case orientingWithBlock: Serial.println("Orienting with Block");
    case other: Serial.println("Other");
   }
}

void printBestBlock(Robot r) {
  /*
   * Prints out R's best block from the heuristic
   */
   Serial.print("Best Block x, y: ");
   Block b = determineBestBlock(r);
   Serial.print(b.pos.x);
   Serial.print(", ");
   Serial.println(b.pos.y);
}

void printDebugging(Robot r) {
  /*
   * Prints all debugging information
   */
  Serial.print("Current:");
  Serial.println(readCurrentSensor());
  RawViveData rvd = readViveSensors();
  printRawVivePositions();
  printVirtualPositions(physicalPointToVirtualPoint(rvd.v1LightPoint),
                        physicalPointToVirtualPoint(rvd.v2LightPoint),
                        rvd.heading);
  Serial.print("Robot Virtual Position: ");
  Serial.print(r.pos.x);
  Serial.print(", ");
  Serial.println(r.pos.y);
  printInEllipse(r);
  printRobotState(r);
  printBestBlock(r);
}

