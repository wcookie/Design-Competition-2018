#include<math.h>
#include<array>
// For Vive:
#include <avr/io.h>
#include <avr/interrupt.h>
#define Vive1PIN 22 // the signal from the FRONT sensor
#define Vive2PIN 23 // the signal from the BACK sensor
#define DEG_PER_US 0.0216 // equal to (180 deg) / (8333 us)
#define DEG_TO_RADIAN 0.0174533 // pi/180
#define LIGHTHOUSEHEIGHT 3.5 // in feet
#define LIGHTHOUSEANGLE 30 // in degrees


// Center of block leeway 
#define GOAL_LEEWAY 2.5 // in virtual coordinates
/*
 * Code is for Design Competition 2018
 * All code and strategy written by
 * Josh Cohen, Wyatt Cook, and The. Daniel Sours (and some from Nick Marchuk) 
 */

// Our Enums 
 
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

enum ellipseState {
  // This says whether we are inside or outside of the ellipse
  inside, // We are inside the ellipse
  outside // We are outside the ellipse
};

enum searchState {
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

enum drivingState {
  // This will indicate what our robot is currently trying to accomplish
  holdingGoalBlock, // Our robot has our desired block in our possession; MOVE TO GOAL
  holdingEnemyBlock, // Our robot has the enemy's block in our possession; Discard it
  movingTowardsBlock, // We are on our way to a block we detected; stay locked in
  orienting, // We are moving our robot in order to eventually move towards a block; orient
  orientingWithBlock, // This is if we are holding a block and aren't ready to go straight to the goal yet.
  other
};

enum blockEngageState {
  outMoveIn, // Robot is outside, move inside and reapproach
  inMoveOut, // Robot is inside, move outside and reapproach
  moveBlockOrient, // obstacles in way of straightline, move block, reorient, reapproach
  straightApproach, // Robot should find straightline approach TODO(JCohner) call desiredOrientationPointStraight()
};

enum turningSpeed {
  fast,
  slow
};

// Our Structs

struct Point {
  // General Point struct
  double x;
  double y;
  Point(double xPos=0.0, double yPos=0.0):
    x(xPos), y(yPos){}
};

struct LightPoint {
  // Point struct for the "physical" address we are reading from the vive
  double x;
  double y;
  LightPoint(double xPos=0.0, double yPos=0.0):
    x(xPos), y(yPos){}
};

struct RawViveData {
  // A struct that contains both vive Points, and our heading
  LightPoint v1LightPoint;
  LightPoint v2LightPoint;
  double heading;
  RawViveData(LightPoint v1LP = LightPoint(), LightPoint v2LP = LightPoint(),
              double h = 0.0):
                v1LightPoint(v1LP), v2LightPoint(v2LP), heading(h){}
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
  goalType team; // which team we are on
  Block desiredBlock; // The block we want (useful for orienting and moving towards block)
  searchState searching; // whether we are still on the ellipse or not
  attackState atk; // whether we are attacking or defending
  drivingState driving; // our driving state (whether we are going to block, etc.)
  blockEngageState approach; //how robot is approaching target block or positioning itself
  ellipseState inOrOut; //whether the robot is in or out of the ellipse
  Point goalPos; // This is mostly relevant for orienting, the goal's position we are going for
  turningSpeed turnSpeed; // Whether we are turning fast or slow
  Robot(Point p = Point(), double h = 0.0, goalType t = circle, Block b = Block(), 
        searchState s = ellipse, attackState a = scoring, 
        drivingState ds = orienting, Point gp = Point(), turningSpeed ts = fast):
          pos(p), heading(h), team(t), desiredBlock(b), searching(s),
          atk(a), driving(ds), goalPos(gp), turnSpeed(ts){}
  
};

// Vive struct

typedef struct {
  unsigned long changeTime[11];
  double horzAng;
  double vertAng;
  int useMe;
  int collected;
} viveSensor;


// Our Global Variables
Robot phoenix; // Phoenix is our robot object used throughout
Circle Circles[] = {Circle(Point(8.6, 53.5), 7.5), // Outer
                    Circle(Point(43.6, 29.5), 7.5), // Inner
                    Circle(Point(113.5, 8.0), 7.5) // Outer
};
Circle innerCircle = Circles[1];
Circle outerCircles[] = {Circles[0], Circles[2]};
Square Squares[] = {Square(Point(1.0, 2.0), 5.0), // Outer
                    Square(Point(5.0, -4.0), 5.0), // Inner
                    Square(Point(2.0, 0.0), 5.0) // Outer
};
Square innerSquare = Squares[1];
Square outerSquares[] = {Squares[0], Squares[2]};

// Vive Global Variables
volatile viveSensor V1, V2;
double xPos1, yPos1, xPos2, yPos2;
double xOld1 = 0, yOld1 = 0, xFilt1 = 0, yFilt1 = 0;
double xOld2 = 0, yOld2 = 0, xFilt2 = 0, yFilt2 = 0;




void setup() {
  Serial.begin(9600);
  motorSetup();
  currentSensorSetup();
  lasersSetup();
  viveSetup();
  lastSeenSetup();
  setupTripwire();
  // set up our phoenix robot based on what we have.
  // Normally circle is getTeam()
  phoenix = Robot(Point(), 0.0, circle, Block(), ellipse, getAttackState(), orienting, Point());
}

void loop() { 

  // THINGS WE DO REGARDLESS OF STATE
  // First update our robot's position and direction
  setRobotPositionAndDirection(phoenix);
  printDebugging(phoenix);
  Serial.print("Desired angle: ");
  Serial.println(desiredAngle(phoenix, phoenix.desiredBlock.pos));
  //readingBlock(true);
  phoenix.goalPos = Circles[0].center;
  //printThreeCylinders(phoenix);
  turnOnTripwire();
  Serial.println(readTripwire());
  Serial.println(holdingBlock());


  // SWITCH ON STATE
  /*switch(phoenix.driving) {
    case holdingGoalBlock: dropOffBlock(phoenix);
    case holdingEnemyBlock: discardEnemyBlock(phoenix);
    case movingTowardsBlock: moveTowardsBlock(phoenix);
    case orienting: orientRobot(phoenix);
    case orientingWithBlock: orientWithBlock(phoenix);
  } 
*/
  //rotateToAngle(phoenix, 0);
  moveMotors(100, 1, 100, 1);
  //driveTowardsPoint(phoenix, innerCircle.center);
  delay(500);
  
}

