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
LightPoint topLeftCorner = LightPoint(-1.26, 3.20); 
LightPoint bottomLeftCorner = LightPoint(0.49, -3.17);
LightPoint topRightCorner = LightPoint(14.50, 3.60);
LightPoint bottomRightCorner = LightPoint(15.4, -1.6);

//Virtual address of ellipse center, plus virtual distance of radii
Point center = Point(60, 30);
double xRadius = 40; // TODO: MEASURE
double yRadius = 25; // TODO: MEASURE


Point physicalPointToVirtualPoint(LightPoint lp) {
 /*
  * turns a physical point into our virtual coordinate plane.
  * calculates the physical ranges for x and Y and uses the proportion of the range verse
  * our virtual range.  Then throws that proportion onto our individual point.
  * Maybe TODO: Make some of this a setup task that affects global variables
  */
 double leftX = (double)(bottomLeftCorner.x + topLeftCorner.x) / 2.0;
 double rightX = (double)(bottomRightCorner.x + topRightCorner.x) / 2.0;
 double topY = (double)(topLeftCorner.y + topRightCorner.y) / 2.0;
 double bottomY = (double)(bottomLeftCorner.y + bottomRightCorner.y) / 2.0;
 double xDiff = rightX - leftX;
 double yDiff = topY - bottomY;
 double xProp = X_COORD_RANGE / xDiff;
 double yProp = Y_COORD_RANGE / yDiff;
 double virtualX = xProp * (lp.x - leftX); // 
 double virtualY = yProp * (lp.y - bottomY); // 
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
   xTerm /= sq(xRadius);
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

