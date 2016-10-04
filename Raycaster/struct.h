#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3dmath.h"


typedef struct {
  int kind; // 0 = plane, 1 = sphere, 2 = teapot
  double color[3];
  
  union {
    struct {
      double center[3];
      double radius;
    } cylinder;

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

