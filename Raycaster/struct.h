// #ifndef JSON_READ_H
// #define JSON_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3dmath.h"


struct Object{
  char *kind;
  double color[3];
  union {
  // Camera structure.  Keeps track of width and height
    struct {
      double width;
      double height;
    } camera;
    // Sphere structure.  Keeps track of the center/position and radius
    struct {
      double center[3];
      double radius;
    } sphere;
    // Plane structure.  Keeps track of normal vector and position vector
    struct {
      double position[3];
      double normal[3];
    } plane;
  };
} Object;

// Objects structure of the values used in the parser.
typedef struct objValues {
	struct Object *objValue;
	size_t objPos;
} objValues;

// Structure for images.  Used with color and ppm
typedef struct Img {
	double width;
	double height;
	char* color;
} Img;

struct objValues read_scene(FILE* json);
void ppmMaker(Img *image, FILE* fput, int num);
//#endif
