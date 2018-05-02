#include<math.h>
#include<array>

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







double distance(Point p1, Point p2) {
  double xDiff = p1.x - p2.x;
  xDiff *= xDiff;
  double yDiff = p1.y - p2.y;
  yDiff *= yDiff;
  return sqrt(xDiff + yDiff);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  
}


