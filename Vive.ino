// Code for Vive calculations
// Reminder that V1 is the front Vive sensor and V2 is the back Vive sensor

// Vive Global Variables


void viveSetup() {
  V1.horzAng = 0;
  V1.vertAng = 0;
  V1.useMe = 0;
  V1.collected = 0;
  pinMode(Vive1PIN, INPUT); // to read the sensor
  attachInterrupt(digitalPinToInterrupt(Vive1PIN), ISRVive1, CHANGE);
  V2.horzAng = 0;
  V2.vertAng = 0;
  V2.useMe = 0;
  V2.collected = 0;
  pinMode(Vive2PIN, INPUT); // to read the sensor
  attachInterrupt(digitalPinToInterrupt(Vive2PIN), ISRVive2, CHANGE);
}


RawViveData readViveSensors() {
  if (V1.useMe == 1) {
    V1.useMe = 0;
    // calculate the position and filter it
    xPos1 = tan((V1.vertAng - LIGHTHOUSEANGLE) * DEG_TO_RADIAN) * LIGHTHOUSEHEIGHT;
    yPos1 = LIGHTHOUSEHEIGHT / cos((V1.vertAng - LIGHTHOUSEANGLE) * DEG_TO_RADIAN)  * tan((V1.horzAng - 90.0) * DEG_TO_RADIAN);
    xFilt1 = xOld1 * 0.5 + xPos1 * 0.5; // filter
    yFilt1 = yOld1 * 0.5 + yPos1 * 0.5; // filter
    xOld1 = xFilt1; // remember for next loop
    yOld1 = yFilt1; // remember for next loop
  }
  if (V2.useMe == 1) {
    V2.useMe = 0;

    // calculate the position and filter it
    xPos2 = tan((V2.vertAng - LIGHTHOUSEANGLE) * DEG_TO_RADIAN) * LIGHTHOUSEHEIGHT;
    yPos2 = LIGHTHOUSEHEIGHT / cos((V2.vertAng - LIGHTHOUSEANGLE) * DEG_TO_RADIAN)  * tan((V2.horzAng - 90.0) * DEG_TO_RADIAN);
    xFilt2 = xOld2 * 0.5 + xPos2 * 0.5; // filter
    yFilt2 = yOld2 * 0.5 + yPos2 * 0.5; // filter
    xOld2 = xFilt2; // remember for next loop
    yOld2 = yFilt2; // remember for next loop
  }
  double xDiff = xPos2 - xPos1; // 2 is front
  double yDiff = yPos2 - yPos1;
  double heading = atan2(yDiff, xDiff);
  return RawViveData(LightPoint(xPos1, yPos1), LightPoint(xPos2, yPos2), heading);
}

void printRawVivePositions() {
  Serial.print("Physical V1: ");
  Serial.print(xPos1);
  Serial.print(", ");
  Serial.println(yPos1);
  Serial.print("Physical V2: ");
  Serial.print(xPos2);
  Serial.print(", ");
  Serial.println(yPos2);
}

void printVirtualPositions(Point virtual1, Point virtual2, double heading) {
  Serial.print("Virtual V1: ");
  Serial.print(virtual1.x);
  Serial.print(", ");
  Serial.println(virtual1.y);
  Serial.print("Virtual V2: ");
  Serial.print(virtual2.x);
  Serial.print(", ");
  Serial.println(virtual2.y);
  Serial.print("Heading: ");
  Serial.println(heading);
}


//Interrupts:

void ISRVive1() {
  // get the time the interrupt occured
  unsigned long mic = micros();
  int i;

  // shift the time into the buffer
  for (i = 0; i < 10; i++) {
    V1.changeTime[i] = V1.changeTime[i + 1];
  }
  V1.changeTime[10] = mic;

  // if the buffer is full
  if (V1.collected < 11) {
    V1.collected++;
  }
  else {
    // if the times match the waveform pattern
    if ((V1.changeTime[1] - V1.changeTime[0] > 7000) && (V1.changeTime[3] - V1.changeTime[2] > 7000) && (V1.changeTime[6] - V1.changeTime[5] < 50) && (V1.changeTime[10] - V1.changeTime[9] < 50)) {
      V1.horzAng = (V1.changeTime[5] - V1.changeTime[4]) * DEG_PER_US;
      V1.vertAng = (V1.changeTime[9] - V1.changeTime[8]) * DEG_PER_US;
      V1.useMe = 1;
    }
  }
}
void ISRVive2() {
  // get the time the interrupt occured
  unsigned long mic = micros();
  int i;

  // shift the time into the buffer
  for (i = 0; i < 10; i++) {
    V2.changeTime[i] = V2.changeTime[i + 1];
  }
  V2.changeTime[10] = mic;

  // if the buffer is full
  if (V2.collected < 11) {
    V2.collected++;
  }
  else {
    // if the times match the waveform pattern
    if ((V2.changeTime[1] - V2.changeTime[0] > 7000) && (V2.changeTime[3] - V2.changeTime[2] > 7000) && (V2.changeTime[6] - V2.changeTime[5] < 50) && (V2.changeTime[10] - V2.changeTime[9] < 50)) {
      V2.horzAng = (V2.changeTime[5] - V2.changeTime[4]) * DEG_PER_US;
      V2.vertAng = (V2.changeTime[9] - V2.changeTime[8]) * DEG_PER_US;
      V2.useMe = 1;
    }
  }
}
