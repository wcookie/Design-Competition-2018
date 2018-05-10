// Tab for miscalleneous functions / utility

int currentSensorPin = 23; //A9
int tripwirePin = 22; // A8

double  tripwireThreshold = 40; // If we are above this, we are tripped.  Maybe should be calibrated



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

void tripwireSetup() {
  pinMode(tripwirePin, INPUT);
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
   * Determines whether we are holding a block or not based on tripwire
   * Might want to make this more robust ?? @JCohner
   */
   return (readTripwire() > tripwireThreshold);
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

