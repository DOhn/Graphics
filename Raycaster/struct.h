// #ifndef JSON_READ_H
// #define JSON_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3dmath.h"


typedef struct {
  char *kind;
  double color[3];
  
  union {
    struct {
      double width;
      double height;
    } camera;

    struct {
      double center[3];
      double radius;
    } sphere;

    struct {
      double position[3];
      double normal[3];
    } plane;
  };
} Object;

typedef struct objValues {
	Object *objValue;
	size_t objPos;
} objValues;

//struct objValues read_scene(FILE* json);
//#endif
