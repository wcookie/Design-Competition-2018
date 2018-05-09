// Tab for miscalleneous functions / utility

int currentSensorPin = 23; //A9

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
   double sum = 0;
   for (int ii = 0; ii < 10; ++ii){
    sum += analogRead(currentSensorPin);
   }
   sum = sum / 10;

   return sum;
}

