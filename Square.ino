//Square comes before in arguments

Square Squares[] = {Square(Point(1.0, 2.0), 5.0), // Outer
                    Square(Point(5.0, -4.0), 5.0), // Inner
                    Square(Point(2.0, 0.0), 5.0) // Outer
};

Square innerSquare = Squares[1];
Square outerSquares[] = {Squares[0], Squares[2]};

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

/*
Square bestGoal(Block b, Robot r) {
  /*
   * This will find the best square goal to go to, depending on 
   * whether we are outside or inside, and then determining on distance
   
   if (robotEllipseState(r) == inside) {
    // If our robot is inside we want to bring it outside
   }
}
*/
