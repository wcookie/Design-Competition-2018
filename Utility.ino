// Tab for miscalleneous functions / utility

int currentSensorPin = 15; // An analog pin

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

double readCurrentSensor() {
  /*
   * Gets the current sensor value
   */
   return analogRead(currentSensorPin);
}

