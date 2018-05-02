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
  Point(double xPos=0.0, double yPos=0.0):x(xPos),
           y(yPos){}
};

struct Circle {
  Point center;
  double radius;
  Circle(Point c=Point(), double r=0.0):center(c),
           radius(r){}
};

struct Square {
  Point center;
  double sideLength;
  Square(Point c=Point(), double s=0.0):center(c),
          sideLength(s){}
};


void setup() {
  Serial.begin(9600);
}

void loop() {
  
}


