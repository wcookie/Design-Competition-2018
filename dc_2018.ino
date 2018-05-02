#include<math.h>

/*
 * Code is for Design Competition 2018
 * All code and strategy written by
 * Josh Cohen, Wyatt Cook, and The. Daniel Sours
 */
struct Point {
  double x;
  double y;
};

struct Circle {
  Point center;
  double radius;
};

struct Square {
  Point center;
  double sideLength;
};

// INCLUDE circle center and radius for each circle







double distance(double x1, double x2, double y1, double y2) {
  double xDiff = x1 - x2;
  xDiff *= xDiff;
  double yDiff = y1 - y2;
  yDiff *= yDiff;
  return sqrt(xDiff + yDiff);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  
}


