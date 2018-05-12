#define BLOCK_TO_GOAL_COEFFICIENT .2
#define BOT_TO_BLOCK_COEFFICIENT .25
#define ROTATION_COEFFICIENT 5

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

blockEngageState blockDetermineEngageState (Block b, Robot& r, Point goal) {
   /*
    * This determines the best enagagement approach for the relative position of block, robot, & goal
    * Returns a blockEngageState [outMoveIn, inMoveOut, straightLine
    */

}

                     
