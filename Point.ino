// Stuff to do with points, aka converting between Physical and virtual points
double X_COORD_RANGE = 120;
double Y_COORD_RANGE = 60;


/*
 * Our Point coordinate system:
 * We have both "physical" and "virtual" points.  
 * Physical Points: What we read from the vive. This will be raw values that we calibrate.  
 * Virtual Points: Points that will be used for our calculations, derived from physical addresses
 * and calibration values.
 * Virtual points go from -60 to +60 and -30 to +30 as the field is 12 x 6
 * Left/right /up and down is defined from looking from the Design Competition sign perspective
 */

 LightPoint bottomLeftCorner = LightPoint(-4.0, -2.4); 
 LightPoint bottomRightCorner = LightPoint(-4.0, 2.4);
 LightPoint topLeftCorner = LightPoint(4.0, -2.4);
 LightPoint topRightCorner = LightPoint(4.0, 2.4);


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
  double virtualX = xProp * lp.x;
  double virtualY = yProp * lp.y;
  return Point(virtualX, virtualY);
 }

