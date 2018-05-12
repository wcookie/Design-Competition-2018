

Square findNearestSquare(Point p) {
  /*
   * given a point, go through the squares array and 
   * find the nearest square goal to go to
   */
   Square ret;
   double minDistance = 10000;
   for (int i =0; i <3; ++i) {
    double tempDistance = distance(Squares[i].center, p);
    if (tempDistance <= minDistance) {
      ret = Squares[i];
      minDistance = tempDistance;
    }
   }
   return ret;
}

Point findNearestEdge(Square s, Point p) {
  /*
   * Given a square and our robot's pont, finds the nearest point
   * along the edge to our robot.
   * This is basically transcribing a circle into the square then calling the circle func
   */
   double cornerDist = (double)s.sideLength * (double)sqrt(2) * .5;
   return findNearestEdge(Circle(s.center, cornerDist), p);
}


Square findBestSquareGoal(Block b, Robot r) {
  /*
   * This will find the best square goal to go to, depending on 
   * whether we are outside or inside, and then determining on distance
   */
   Square bestGoal;
   double minDistance = 1000.0;
   if (robotEllipseState(r) == inside) {
    // If our robot is inside we want to bring it outside
    for (int i = 0; i < 2; ++i) {
      double blockDistance = distance(outerSquares[i].center, b.pos);
      if (blockDistance < minDistance) {
        minDistance = blockDistance;
        bestGoal = outerSquares[i];
      }
    }
   } else {
    // Our robot is outside, so we want to check it inside, basically just return
    // The only inside goal
    bestGoal = innerSquare;
   }
   return bestGoal;
}

bool inSquareGoal(Robot r) {
  /*
   * Checks to see if we are by the center of a circle goal (has some leeway)
   */
   for (int i = 0; i < 3; ++i) {
    Square s = Squares[i];
    // If we're fairly close to center return true
    if (distance(s.center, r.pos) < GOAL_LEEWAY) {
      return true;
    }
   }
   // If we were never that close, return false
   return false;
}

