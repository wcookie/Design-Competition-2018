/*
 * 
struct Block {
  // Represents a block on the board
  Point pos; //block's position
  blockType type; // whether it's a cube or cylinder
  blockState state; // Whether we believe it to be still on the ellipse or not 
  Block(Point p=Point(), blockType t=cube,blockState s=unknown):
           pos(p), type(t), state(s){}
};
 */


Block cylinders[] = {Block(Point(1.0, 2.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown),
                     Block(Point(2.0, 3.0), cylinder, unknown)};
                     

Block cubes[] = {Block(Point(1.0, 2.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown),
                 Block(Point(2.0, 3.0), cube, unknown)};



                 
                     
