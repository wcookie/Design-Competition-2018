// Stuff to do with points, aka converting between Physical and virtual points

// Our virtual coordinate range
double X_COORD_RANGE = 120;
double Y_COORD_RANGE = 60;


/*
 * Our Point coordinate system:
 * We have both "physical" and "virtual" points.  
 * Physical Points: What we read from the vive. This will be raw values that we calibrate.  
 * Virtual Points: Points that will be used for our calculations, derived from physical addresses
 * and calibration values.
 * Virtual points go from 0 to 120 and 0 to 60 as the field is 12 x 6
 * Left/right /up and down is defined from looking from the Design Competition sign perspective
 */

// Physical addresses of corners
LightPoint topLeftCorner = LightPoint(.6, 4.0); 
LightPoint bottomLeftCorner = LightPoint(0.77, -3.79);
LightPoint topMiddle = LightPoint(7.5, 3.35);
LightPoint bottomMiddle = LightPoint(8.63, -2.6);
LightPoint topRightCorner = LightPoint(14.7, 4.05);
LightPoint bottomRightCorner = LightPoint(16.3, -2.6);

//Virtual address of ellipse center, plus virtual distance of radii
Point center = Point(58.47, 30.7);
double leftXRadius = 45.4; // TODO: MEASURE
double rightXRadius = 49;
double yRadius = 22; // TODO: MEASURE


Point physicalPointToVirtualPoint(LightPoint lp) {
 /*
  * turns a physical point into our virtual coordinate plane.
  * calculates the physical ranges for x and Y and uses the proportion of the range verse
  * our virtual range.  Then throws that proportion onto our individual point.
  * Separates the board into a leeft half adn right half to try and make the virtual system closer to reality
  */
  double midwayX = (topMiddle.x + bottomMiddle.x) / 2.0;
  if (lp.x > midwayX) {
    return rightVirtualConversion(lp);
  } else {
    return leftVirtualConversion(lp);
  }
  
}

Point leftVirtualConversion(LightPoint lp) {
  /*
   * Goes from 0 to half of X_COORD_Range for the left half of the board. 
   */
 double leftX = (double)(bottomLeftCorner.x + topLeftCorner.x) / 2.0;
 double rightX = (double)(bottomMiddle.x + topMiddle.x) / 2.0;
 double topY = (double)(topMiddle.y + topLeftCorner.y) / 2.0;
 double bottomY = (double)(bottomLeftCorner.y + bottomMiddle.y) / 2.0;
 double xDiff = rightX - leftX;
 double yDiff = topY - bottomY;
 double xProp = (X_COORD_RANGE * .5) / xDiff;
 double yProp = Y_COORD_RANGE / yDiff;
 double virtualX = xProp * (lp.x - leftX);   // Goes from 0 to X_COORD_RANGE
 double virtualY = yProp * (lp.y - bottomY); 
 return Point(virtualX, virtualY);
}

Point rightVirtualConversion(LightPoint lp) {
 /*
  * Goes from 60 to X_COORD_RANGE for the right half of the board
  */
 double leftX = (double)(bottomMiddle.x + topMiddle.x) / 2.0;
 double rightX = (double)(bottomRightCorner.x + topRightCorner.x) / 2.0;
 double topY = (double)(topMiddle.y + topRightCorner.y) / 2.0;
 double bottomY = (double)(bottomMiddle.y + bottomRightCorner.y) / 2.0;
 double xDiff = rightX - leftX;
 double yDiff = topY - bottomY;
 double xProp = (X_COORD_RANGE * .5) / xDiff;
 double yProp = Y_COORD_RANGE / yDiff;
 double virtualX = (xProp * (lp.x - leftX)) + (X_COORD_RANGE * .5); // Goes from X_COORD_RANGE/2 to X_COORD_RANGE 
 double virtualY = yProp * (lp.y - bottomY);
 return Point(virtualX, virtualY);
}

LightPoint virtualPointToPhysicalPoint(Point p) {
  /*
   *The inverse of the above function, gets a physical point for us from a virtual one. 
   */
 double leftX = (double)(bottomLeftCorner.x + topLeftCorner.x) / 2.0;
 double rightX = (double)(bottomRightCorner.x + topRightCorner.x) / 2.0;
 double topY = (double)(topLeftCorner.y + topRightCorner.y) / 2.0;
 double bottomY = (double)(bottomLeftCorner.y + bottomRightCorner.y) / 2.0;
 double xDiff = rightX - leftX;
 double yDiff = topY - bottomY;
 double xProp = X_COORD_RANGE / xDiff;
 double yProp = Y_COORD_RANGE / yDiff;
 double physicalX = (p.x - (X_COORD_RANGE / 2)) / xProp;
 double physicalY = (p.y - (Y_COORD_RANGE / 2)) / yProp;
 return LightPoint(physicalX, physicalY);
}


ellipseState robotEllipseState(Robot r) {
  /*
   * This will determine whether our robot is inside or outside of the ellipse
   * This is used in determining which block to go to, and what to do with it.
   * Taken from the following stack exchange link: 
   * https://math.stackexchange.com/questions/76457/check-if-a-point-is-within-an-ellipse
   */
   double xTerm = sq(r.pos.x - center.x);
   // Using two separate radii to account for different linearity between left and right side of the board
   if (r.pos.x < center.x) {
    xTerm /= sq(leftXRadius);
   } else {
    xTerm /= sq(rightXRadius);
   }
   double yTerm = sq(r.pos.y - center.y);
   yTerm /= sq(yRadius);
   double compVal = xTerm + yTerm;
   // If the sum of the two terms is > 1 we are outside the ellipse, otherwise inside it
   if (compVal > 1) {
    return outside;
   } else {
    return inside;
   }
}


double distance(Point p1, Point p2) {
  /*
   * Calculates distance between two points
   */
  double xDiff = p1.x - p2.x;
  xDiff *= xDiff;
  double yDiff = p1.y - p2.y;
  yDiff *= yDiff;
  return sqrt(xDiff + yDiff);
}

double desiredAngle(Robot r, Point p) {
  /*
   * Finds the angle we want to drive straight to a point.
   * Takes in a robot, and the desired point.
   * Note: Everythign is in radians
   * TODO (JCohner): Figure out solution to how the angle can go from slightly under -Pi to slightly under Pi in such a quick step. 
   * (see readViveSensors in Vive.ino)
   */
   double yDiff = p.y - r.pos.y;
   double xDiff = p.x - r.pos.x;
   return atan2(yDiff, xDiff);
}

double angleBetween2Points(Point p1, Point p2){
  double angle;
  double yDiff = p2.y - p1.y;
  double xDiff = p2.x - p1.x;
  return atan2(yDiff,xDiff);
}

Point desiredOrientationPointStraight(Robot r, Block b, Point goal) {
  /*
   * For our orienting mode, this tells us where to drive to before we rotate to go straight to the block to the goal
   * This only is for the blocks we want to drive straight to the goal, without moving somewhere first and then rotating
   * look at desiredOrientationPointStepped for orienting to get our block to move off the board first then to our goal
   * 
   * JCohner: Please fucking sanity check this with me I feel like i went off the deep end here.
   * 
   * Calculations:
   * 
   * First calculate the angle from the desired goal to the block.  
   * Then basically extend this by steps of .25 in the x direction.  
   * 
   * For now: Say that when the point is not too close to the block and too close to the edge just go there 
   * 
   * TODO: 
   * Then calculate angle difference between
   * our robot and the block of this part, and when it is close to 90 degrees (Pi/2 radians) 
   * then we say we have our desired point to go to.
   * TODO: If the desired point is too close to the block, move a little bit 
   * further along the line just to avoid turning and fucking with the block
   * Also make sure it's not too close to the edge
   */
   double distRobotCenter = distance(r.pos, goal); 
   double distRobotBlock = distance(r.pos, b.pos); 
   double distBlockCenter = distance(b.pos, goal);
   
   double angleRobotCenterBlock = acos(((-(sq(distRobotBlock))) + sq(distRobotCenter) + sq(distBlockCenter))/(2 * (distRobotBlock) * (distBlockCenter)));

   double distRobotDesired = distRobotCenter * sin(angleRobotCenterBlock);
   double distCenterDesired = distRobotCenter * cos(angleRobotCenterBlock);

   Point desiredPoint = Point((r.pos.x + distRobotDesired), (goal.y - distCenterDesired));

   return desiredPoint;
}

Point desiredOrientationPointOffset(Robot r, Block b, Point goal) {
  /*
   * This determines the point we want to go to WHILE HOLDING THE BLOCK
   * At this point we will rotate to face the goal and go straight
   * @JCohner: This is your baby
   */
}

