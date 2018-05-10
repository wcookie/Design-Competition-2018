// Tab for miscalleneous functions / utility
#define BLOCK_LASER_THRESHOLD 6 // If the difference is above this we see a block

int currentSensorPin = 23; //A9
int tripwirePin = 22; // A8
int blockPTPin = 15; // A1
int blockLaserPin3 = 11;
int blockLaserPin2 = 10;
int blockLaserPin1 = 9;

double  tripwireThreshold = 40; // If we are below this, we are tripped.  Maybe should be calibrated


double distance(Point p1, Point p2) {
  double xDiff = p1.x - p2.x;
  xDiff *= xDiff;
  double yDiff = p1.y - p2.y;
  yDiff *= yDiff;
  return sqrt(xDiff + yDiff);
}

void currentSensorSetup() {
  pinMode(currentSensorPin, INPUT);
}

void lasersSetup() {
  pinMode(tripwirePin, INPUT);
  pinMode(blockPTPin, INPUT);
  pinMode(blockLaserPin1, OUTPUT);
  pinMode(blockLaserPin2, OUTPUT);
  pinMode(blockLaserPin3, OUTPUT);
}

double readCurrentSensor() {
  /*
   * Gets the current sensor value
   */
   double sum = 0;
   double counter = 0;
   for (int ii = 0; ii < 140; ++ii){
    double sensorVal = analogRead(currentSensorPin);
    if ((sensorVal > 50) && (sensorVal < 950)) {
      counter += 1.0;
      sum += sensorVal;
    }
    delay(1);
   }
   sum = sum / counter;
   return sum;
}

double readCurrentSensorBasic() {
  /*
   * basic version of above
   */
   return analogRead(currentSensorPin);
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
  digitalWrite(blockLaserPin3, mode);
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


void printDebugging() {
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
}

