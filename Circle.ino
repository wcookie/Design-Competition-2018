// Circle comes before Point in arguments 

Circle Circles[] = {Circle(Point(1.0, 2.0), 5.0), // Outer
                    Circle(Point(5.0, -4.0), 5.0), // Inner
                    Circle(Point(2.0, 0.0), 5.0) // Outer
};

Circle innerCircle = Circles[1];
Circle outerCircles[] = {Circles[0], Circles[2]};

Circle findNearestCircle(Point p) {
  /*
   * check all circles on map and determine optimal one to go to.  
   * At first just using distance in heuristic
   * TODO(IMPORTANT): Include angle in the calculation, so that we dont have to turn around
   */
   Circle ret;
   double minDistance = 100000;
   for (int i = 0; i < 3; ++i) {
     double tempDistance = distance(Circles[i].center, p);
     if (tempDistance <= minDistance) {
       ret = Circles[i];
       minDistance = tempDistance;
     }
   }
   return ret;
}


Point findNearestEdge(Circle c, Point p) {
  /*
   * For a given circle on map, find the closest edge to our robot.
   * Algorithm here on stack exchange:
   * https://math.stackexchange.com/questions/127613/closest-point-on-circle-edge-from-point-outside-inside-the-circle
  */
  double circleXPos = c.radius * (p.x - c.center.x);
  double circleYPos = c.radius * (p.y - c.center.y);
  double denominator = sqrt(sq(p.x - c.center.x) + sq(p.y - c.center.y));
  circleXPos /= denominator;
  circleXPos += c.center.x;
  circleYPos /= denominator;
  circleYPos += c.center.y;
  return Point(circleXPos, circleYPos);
}

Circle findBestCircleGoal(Block b, Robot r) {
  /*
   * Finds the best circle goal based on whether we are inside or outside the circle
   * TODO(Jcohner): Take into account if we are on the side of the board that has the center nearer...
   * May want to only go for inner goal if we are still on the half that has the inner goal. 
   */
   Circle bestGoal;
   double minDistance = 1000.0;
   if (robotEllipseState(r) == inside) {
    // If our robot is inside we want to bring it outside
    for (int i = 0; i < 2; ++i) {
      double blockDistance = distance(outerCircles[i].center, b.pos);
      if (blockDistance < minDistance) {
        minDistance = blockDistance;
        bestGoal = outerCircles[i];
      }
    }
   } else {
    // Our robot is outside, so we want to check it inside, basically just return
    // The only inside goal
    bestGoal = innerCircle;
   }
   return bestGoal;
}

