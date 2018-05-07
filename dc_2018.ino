#include<math.h>
#include<array>

/*
 * Code is for Design Competition 2018
 * All code and strategy written by
 * Josh Cohen, Wyatt Cook, and The. Daniel Sours
 */

 
enum goalType {
  // Whether or not we are on the square or circle team
  circle,
  square
};

enum blockType {
  // Whether the block is a cube or a cylinder
  cube,
  cylinder
};

enum blockState {
  // This shows our assumed state of the block, in order of 
  // least likely to be there to most likely to be there, plus unknown
  // This is going to be mostly on useful in the original stages of the blocks
  moved, // We are confident that we have moved it away physically
  invisible, // We tried to find it but didn't see it where we expected it 
  unknown, // We haven't checked that block, also means probably on ellipse, especiallly in start
  seen // We have locked in on a block that we're fairly confident is in that position
};

enum drivingState {
  // This is whether or not we are looking for blocks on the ellipse or
  // if we are just driving around scanning randomly
  ellipse, // We are looking for blocks around the ellipse,
  scanning // We are just scanning for blocks
};

enum attackState {
  // This will determine whether our robot is an attack or defense bot 
  // For our multibot system
  // for clarification circle team with defense, will be messing with 
  // the square team
  scoring, // We are trying to score points
  defending // we are trying to limit the opponent's point total
};

struct Point {
  // General Point struct
  double x;
  double y;
  Point(double xPos=0.0, double yPos=0.0):x(xPos),
           y(yPos){}
};

struct Circle {
  // Represents Circles on the board
  Point center;
  double radius;
  Circle(Point c=Point(), double r=0.0):center(c),
           radius(r){}
};

struct Square {
  // Represents Squares on the board
  Point center;
  double sideLength;
  Square(Point c=Point(), double s=0.0):center(c),
          sideLength(s){}
};

struct Block {
  // Represents a block on the board
  Point pos; //block's position
  blockType type; // whether it's a cube or cylinder
  blockState state; // Whether we believe it to be still on the ellipse or not 
  Block(Point p=Point(), blockType t=cube,blockState s=unknown):
           pos(p), type(t), state(s){}
};

struct Robot {
  // Represents our robot on the board
  Point pos; // our robot's position (center)
  double heading; // the direction our robot is facing.
  // Heading goes from 0 to 360, 0 is facing directly to the right of our orientation,
  // 180 directly to left, 90 directly straight, -90 directly down (this can change)
  goalType team;
  Block desiredBlock; 
  drivingState driving;
  attackState atk;
  Robot(Point p = Point(), double h = 0.0, goalType t = circle, Block b = Block(), 
        drivingState d = ellipse, attackState a = scoring):
          pos(p), heading(h), team(t), desiredBlock(b), driving(d), atk(a){}
  
};

Robot phoenix; // Phoenix is our robot object used throughout
 

void setup() {
  Serial.begin(9600);
  motorSetup();
  // set up our phoenix robot based on what we have.
  phoenix = Robot(Point(), 0.0, getTeam(), Block(), ellipse, getAttackState());
}

void loop() {

  moveMotors(120, 1, 120, 1);
  delay(1000);
  
}


