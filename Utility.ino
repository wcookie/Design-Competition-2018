// Tab for miscalleneous functions / utility
double distance(Point p1, Point p2) {
  double xDiff = p1.x - p2.x;
  xDiff *= xDiff;
  double yDiff = p1.y - p2.y;
  yDiff *= yDiff;
  return sqrt(xDiff + yDiff);
}
