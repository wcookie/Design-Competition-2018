// Circle comes before Point in arguments 


Point findNearestEdge(Circle c, Point p) {
  // Algorithm here on stack exchange:
  // https://math.stackexchange.com/questions/127613/closest-point-on-circle-edge-from-point-outside-inside-the-circle
  double circleXPos = c.radius * (p.x - c.center.x);
  double circleYPos = c.radius * (p.y - c.center.y);
  double denominator = sqrt(sq(p.x - c.center.x) + sq(p.y - c.center.y));
  circleXPos /= denominator;
  circleXPos += c.center.x;
  circleYPos /= denominator;
  circleYPos += c.center.y;
}

