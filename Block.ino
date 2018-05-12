#define BLOCK_TO_GOAL_COEFFICIENT .2
#define BOT_TO_BLOCK_COEFFICIENT .25
#define ROTATION_COEFFICIENT 5
#define TEAM_CIRCLE_PATH_COLLISION 3
#define CIRCLE_TEAM_OFFSET 100
#define SQUARE_TEAM_OFFSET 3


/*
 * 
struct Block {
  // Represents a block on the board
  Point pos; //block's position
  blockType type; // whether it's a cube or cylinder
  blockState state; // Whether we believe it to be still on the ellipse or not 
  Block(Point p=Point(), blockType t=cube,blockState s=unknown):
           pos(p), type(t), state(s){}
};
 */


Block cylinders[] = {Block(Point(10.8, 23.6), cylinder, unknown),
                     Block(Point(15.5, 39.7), cylinder, unknown),
                     Block(Point(25.3, 13.40), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown),
                     Block(Point(144.0, 3.0), cylinder, unknown)};
                     

Block cubes[] = {Block(Point(17.3, 16.5), cube, unknown),
                 Block(Point(10.2, 31.8), cube, unknown),
                 Block(Point(23.4, 44), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown)};


void printThreeCylinders(Robot& r) {
    for (int i = 0; i < 3; ++i) {
    printBlockEngageState(cylinders[i], r);
  }
}

Block determineBestBlock(Robot& r) {
  Block bestBlock;
  double minHeuristic = 100000;
  for (int i = 0; i < 12; ++i) {
    Block b;
    if (r.team == circle) {
      b = cylinders[i];
    } else {
      b = cubes[i];
    }
    double tempHeuristic = blockDeterminingHeuristic(b, r);
    if (tempHeuristic < minHeuristic) {
      bestBlock = b;
      minHeuristic = tempHeuristic;
    }
  }
  return bestBlock;
}

double blockDeterminingHeuristic(Block b, Robot& r) {
  /*
   * This determines the quality of a block, given whether or robot is inside or outside
   * Returns a value, which is a huge part of our robot design
   * TODO: Add a timer to not call this for 10 seconds or so to give us ample time to go for a block
   */
   double tempHeuristic = 0.0;
   if(r.team == circle) {
    tempHeuristic += (distance(findBestCircleGoal(b, r).center, b.pos)* BLOCK_TO_GOAL_COEFFICIENT);
   } else {
    tempHeuristic += (distance(findBestSquareGoal(b, r).center, b.pos) * BLOCK_TO_GOAL_COEFFICIENT);
   }
   tempHeuristic += (distance(b.pos, r.pos) * BOT_TO_BLOCK_COEFFICIENT);
   return tempHeuristic;
}

blockEngageState blockDetermineEngageState (Block b, const Robot& r) {
   /*
    * This determines the best enagagement approach for the relative position of block, robot, & goal
    * Returns a blockEngageState [outMoveIn, inMoveOut, moveBlockOrient, straightLine]
    * TODO: (JCohner) add heuristic altering values in blockDetermineHeuristic
    */
  
    if (r.team == circle) {
      // find which of outter circle is closer
      double distOutter1 = abs(b.pos.y - outerCircles[0].center.y);
      double distOutter2 = abs(b.pos.y - outerCircles[1].center.y);

      Circle bestOutter;
      if (distOutter1 < distOutter2) {
        bestOutter = outerCircles[0];
      } else {
        bestOutter = outerCircles[1];
      }
      
      ellipseState inOrOut = r.inOrOut;
      Circle goal;
      if (inOrOut == inside){
        goal = bestOutter;
      } else {
        goal = innerCircle;
      }
      printInEllipse(r);
      Serial.print("Goal position: ");
      Serial.print(goal.center.x);
      Serial.print(" x and ");
      Serial.print(goal.center.y);
      Serial.println(" y");
      
      //determine if closest enemy block interferes 
      Block interferenceBlock1;
      Block interferenceBlock2;
      Block interferenceBlock;
      double minDistance = 10000;
      double minDistance2 = 10000;
      for (int ii = 0; ii < 12; ++ii){
         if (distance(b.pos, cubes[ii].pos) < minDistance) {
          interferenceBlock2 = interferenceBlock1;
          minDistance2 = minDistance;
          interferenceBlock1 = cubes[ii];
          minDistance = distance(b.pos, cubes[ii].pos);
         } else if (distance(b.pos, cubes[ii].pos) < minDistance2){
          interferenceBlock2 = cubes[ii];
          minDistance2 = distance(b.pos, cubes[ii].pos);
         }
      } 
      if (distance(interferenceBlock1.pos, goal.center) < distance(interferenceBlock2.pos, goal.center)){
        interferenceBlock = interferenceBlock1;
      } else {
        interferenceBlock = interferenceBlock2;
      }
      
      Point blockInterferencePoint = Point(interferenceBlock.pos.x, interferenceBlock.pos.y);

      double approachAngle = angleBetween2Points(b.pos, goal.center);
      double approachDistance = distance(b.pos, goal.center);

      double offsetAngle = approachAngle + M_PI/2;
      Circle adjustedGoal1 = goal;
      Circle adjustedGoal2 = goal;

      double angleBlockToAdjGoal1;
      double angleBlockToAdjGoal2;
      double slopeToAdjusted1;
      double slopeToAdjusted2;
      /*
      double angleInterferenceToAdjGoal1; = angleBetween2Points(blockInterferencePoint, adjustedGoal1.center);
      double angleInterferenceToAdjGoal2; = angleBetween2Points(blockInterferencePoint, adjustedGoal2.center);
      */
      if ((b.pos.x - goal.center.x) * (b.pos.y - goal.center.y) > 0){
        //quad 1 and 3 y = mx + b, m = adjustedGoal1.center.y/adjustedGoal.center.x
        adjustedGoal1.center.x = goal.center.x - (cos(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal1.center.y = goal.center.y + (sin(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal2.center.x = goal.center.x + (cos(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal2.center.y = goal.center.y - (sin(offsetAngle) * CIRCLE_TEAM_OFFSET);
        /*
        angleBlockToAdjGoal1 = angleBetween2Points(b.pos, adjustedGoal1.center);
        angleBlockToAdjGoal2 = angleBetween2Points(b.pos, adjustedGoal2.center);
        */
        slopeToAdjusted1 = adjustedGoal1.center.y/adjustedGoal1.center.x;
        slopeToAdjusted2 = adjustedGoal2.center.y/adjustedGoal2.center.x;
        if ((blockInterferencePoint.y < (slopeToAdjusted1 * blockInterferencePoint.x)) && (blockInterferencePoint.y > (slopeToAdjusted2 * blockInterferencePoint.x))){
           Serial.println(b.pos.x);
           return moveBlockOrient;
        } else {
          return straightApproach;
        }
      } else {
        //quad 2 and 4 y = -mx + b, m = adjustedGoal1.center.y/adjustedGoal.center.x
        adjustedGoal1.center.x = goal.center.x + (cos(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal1.center.y = goal.center.y + (sin(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal2.center.x = goal.center.x - (cos(offsetAngle) * CIRCLE_TEAM_OFFSET);
        adjustedGoal2.center.y = goal.center.y - (sin(offsetAngle) * CIRCLE_TEAM_OFFSET);
        /*
        angleBlockToAdjGoal1 = angleBetween2Points(b.pos, adjustedGoal1.center);
        angleBlockToAdjGoal2 = angleBetween2Points(b.pos, adjustedGoal2.center);
        */
        slopeToAdjusted1 = adjustedGoal1.center.y/adjustedGoal1.center.x;
        slopeToAdjusted2 = adjustedGoal2.center.y/adjustedGoal2.center.x;
        if ((blockInterferencePoint.y < (-(slopeToAdjusted1) * blockInterferencePoint.x)) && (blockInterferencePoint.y > (-(slopeToAdjusted2) * blockInterferencePoint.x))){
           Serial.println(b.pos.x);
           return moveBlockOrient;
        } else {
          return straightApproach;
        }
      }
  
    } else { //Square team 
      
    }
 }

                     
