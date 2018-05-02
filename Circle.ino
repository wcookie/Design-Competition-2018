// Circle comes before Point in arguments 

Circle Circles[] = {Circle(Point(1.0, 2.0), 5.0),
                    Circle(Point(5.0, -4.0), 5.0),
                    Circle(Point(2.0, 0.0), 5.0)
};


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
}

