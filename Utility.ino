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

