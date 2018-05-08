// Stuff to do with points, aka converting between Physical and virtual points

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


 
