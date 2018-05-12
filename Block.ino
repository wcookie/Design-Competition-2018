#define BLOCK_TO_GOAL_COEFFICIENT .2
#define BOT_TO_BLOCK_COEFFICIENT .25
#define ROTATION_COEFFICIENT 5
#define TEAM_CIRCLE_PATH_COLLISION 3
#define TEAM_CIRCLE_OBSTACLE_COLLISION 2


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


Block cylinders[] = {Block(Point(21.8, 44.4), cylinder, unknown),
                     Block(Point(10.1, 38.5), cylinder, unknown),
                     Block(Point(6.7, 27.30), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown)};
                     

Block cubes[] = {Block(Point(21.80, 44.4), cube, unknown),
                 Block(Point(10.1, 38.5), cube, unknown),
                 Block(Point(6.7, 27.30), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown),
                 Block(Point(142.0, 3.0), cube, unknown)};


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
      double distOutter1 = distance(r.pos, outerCircles[0].center);
      double distOutter2 = distance(r.pos, outerCircles[1].center);

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
      
      //determine if closest enemy block interferes 
      Block interferenceBlock1;
      Block interferenceBlock2;
      Block interferenceBlock;
      double minDistance = 10000;
      double minDistance2 = 1000;
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
      
      Point blockInterferencePoint1 = Point(interferenceBlock.pos.x + TEAM_CIRCLE_OBSTACLE_COLLISION, interferenceBlock.pos.y);
      Point blockInterferencePoint2 = Point(interferenceBlock.pos.x, interferenceBlock.pos.y + TEAM_CIRCLE_OBSTACLE_COLLISION); 
      Point blockInterferencePoint3 = Point(interferenceBlock.pos.x - TEAM_CIRCLE_OBSTACLE_COLLISION, interferenceBlock.pos.y);
      Point blockInterferencePoint4 = Point(interferenceBlock.pos.x, interferenceBlock.pos.y - TEAM_CIRCLE_OBSTACLE_COLLISION); 
      
      //adjust conflict zone based on relative positions
      //Circle adjustedGoal1 = goal;
      //Circle adjustedGoal2 = goal;
      if (((goal.center.x - r.pos.x) * (goal.center.y - r.pos.y)) > 0) { //same sign therefore in quad 1 or 3
        if (blockInterferencePoint3.y < blockInterferencePoint3.x + TEAM_CIRCLE_PATH_COLLISION){ //you have a collision
           
        } else if (blockInterferencePoint1.y > blockInterferencePoint1.x - sqrt(sq(TEAM_CIRCLE_PATH_COLLISION) + sq(r.pos.x))) { //you have a collision
          
        } else if (blockInterferencePoint4.y < blockInterferencePoint4.x - TEAM_CIRCLE_PATH_COLLISION) { //you have a collision
          
        } else if (blockInterferencePoint2.y < blockInterferencePoint2.x + TEAM_CIRCLE_PATH_COLLISION) { //you have a collision
          
        }
        /*
        adjustedGoal1.center.x += TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal1.center.y += TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal2.center.x -= TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal2.center.y -= TEAM_CIRCLE_PATH_COLLISION;
        */
      } else { //in quad 2 or 4
        if (blockInterferencePoint3.y < -(blockInterferencePoint3.x) + TEAM_CIRCLE_PATH_COLLISION){ //you have a collision
           
        } else if (blockInterferencePoint1.y > -(blockInterferencePoint1.x) - TEAM_CIRCLE_PATH_COLLISION) { //you have a collision
          
        } else if (blockInterferencePoint4.y < -(blockInterferencePoint4.x) - TEAM_CIRCLE_PATH_COLLISION) { //you have a collision
          
        } else if (blockInterferencePoint2.y < -(blockInterferencePoint2.x) + TEAM_CIRCLE_PATH_COLLISION) { //you have a collision
          
        }
        /*
        adjustedGoal1.center.x += TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal1.center.y -= TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal2.center.x -= TEAM_CIRCLE_PATH_COLLISION;
        adjustedGoal2.center.y += TEAM_CIRCLE_PATH_COLLISION;
        */
      }

      //now see if adjusted goal lines conflict with blockInterferencePoints
      

      
    } else {
      
    }
   

}

                     
