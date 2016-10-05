#include "struct.h"
// Plymorphism in C

void shadePixel(double *Col, Img *image, int i, int j) {
  image->color[(int)(i * image->width*4 + j*4+0)] = (char)Col[0];
  image->color[(int)(i * image->width*4 + j*4+1)] = (char)Col[1];
  image->color[(int)(i * image->width*4 + j*4+2)] = (char)Col[2];
  image->color[(int)(i * image->width*4 + j*4+3)] = 255;

}

static inline double sqr(double v) {
  return v*v;
}

static inline void normalize(double* v) {
  double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
  v[0] /= len;
  v[1] /= len;
  v[2] /= len;
}

double sphere_intersection(double *Ro, double *Rd, double *Cent, double R) {
  double num_a, num_b, num_c, sol_a, sol_b;

  num_a = sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[2]);
  num_b = (2*Ro[0]*Rd[0]-Rd[0]*Cent[0]) + (Ro[1]*Rd[1]-Rd[1]*Cent[1]) + (Ro[2]*Rd[2]-Rd[2]*Cent[2]);
  num_c = sqr(Ro[0]-Cent[0]) + sqr(Ro[1]-Cent[1]) + sqr(Ro[2]-Cent[2]) - sqr(R);

  double dist = sqr(num_b) - 4*num_a*num_c;

  if (dist < 0) return -1;

  dist = sqrt(dist);

  double t = (-num_b, - dist) / (2 * num_a);
  if (t > 0) return t;

  t = (-num_b + dist) / (2*num_a);
  if (t > 0) return t;

  return -1;

  // if (dist < 0) {
  //   return -1;  
  // } else if (dist == 0) {
  //   sol_a = -1 * (num_b / (2*num_a));
  //   sol_b = -1 * (num_b / (2 * num_a));
  // } else {
  //   sol_a = (-1*num_b - sqrt(sqrt(num_b) - 4 * num_c))/2;
  //   sol_b = (-1*num_b + sqrt(sqrt(num_b) - 4 * num_c))/2;
  // }

  // if (sol_a < 0 && sol_b < 0) {
  //   return 0;
  // } else if (sol_a < 0 && sol_b > 0) {
  //   return sol_a;
  // } else if (sol_a > 0 && sol_b < 0) {
  //   return sol_a;
  // } else {
  //   if (sol_a <= sol_b) {
  //     return sol_a;
  //   } else {
  //     return sol_b;
  //   }
  // }
}

double plane_intersection(double *Ro, double *Rd, double *Norm, double *Pos) {
  // Declaring variables and normalizing Norm argument
  double temp_d;
  normalize(Norm);
  double temp_a = v3_dot(Norm, Rd);
  V3 a_vector;

  // Subtracting vectors Pos and Ro. a_vector is set to the vaule of the resulting vector.
  v3_sub(Pos, Ro, a_vector);

  if(fabs(temp_a) < .0001) {
    return -1;
  }

  temp_d = v3_dot(a_vector, Norm);
  double t = -(temp_d/temp_a);
  double colA = 0;

  // If value of t is below 0 return 0.
  if (t < 0.0) {
    return 0;
  }

  // Else return value of t
  return t;
}

int main(int argc, char *argv[]) {

  if (argc != 5) {
    fprintf(stderr, "Error: Must be four arguments.");
    exit(1);
  }


  FILE* fput = fopen(argv[3], "rb");

  if (fput == NULL) {
    fprintf(stderr, "Error: JSON File failed to load or open.  Make sure it exists.");
    exit(1);
  } 

  struct objValues objects;
  int numObjs, x, y;

  int cameraType = 0;
  double camH;
  double camW;

  int sphKind = 1;
  double sphColX, sphColY, sphColZ;
  double sphPosX;
  double sphPosY;
  double sphPosZ;
  double radius;

  int plnKind = 2;
  double plnColX, plnColY, plnColZ;
  double plnPosX, plnPosY, plnPosZ;
  double plnNormX, plnNormY, plnNormZ;

  
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  
  objects = read_scene(fput);

  // Extracting from JSON file
  numObjs = objects.objPos;

  int i;

  int mNum = 6;

  // loop through the number of objects
  for (i = 0; i < numObjs; i++) {
    // if there is a camera objects, set the width and height of the camera
    if (objects.objValue[i].camera.width && objects.objValue[i].camera.height) {
      printf("There is a camera\n");
      camW = objects.objValue[i].camera.width;
      camH = objects.objValue[i].camera.height;
    }
  }

  // error check for the camera width and height
  if (!camW || !camH) {
    fprintf(stderr, "ERROR: There is no valid camera width or height\n");
    exit(1);
  }

  // set the pixel width and height
  double pixheight = camH / M;
  double pixwidth = camW / N;

  // create the image
  Img *image = (Img *)malloc(sizeof(Img));
  image->height = N;
  image->width = M;
  image->color = (char*)malloc(sizeof(char) * image->height * image->width * 4);


  // loop through the pixels
  for (y = 0; y < M; y++) {
    for (x = 0; x < N; x++) {
      printf("inside for\n");
      double Ro[3] = {0, 0, 0};

      double Rd[3] = { objects.objValue[i].sphere.center[0] - (camW/2) + pixwidth * (x + 0.5), objects.objValue[i].sphere.center[1] - (camH/2) + pixheight * (y + 0.5), 1 };
      normalize(Rd);

      int colHelp = 0;
      double best_t = INFINITY;
      // loop through the objects figuring out which one to trace
      for (i=0; i < numObjs; i++) {
        printf("hello\n");
        double t = 0;

        // if the current objects is a sphere then call intersect
        if (objects.objValue[i].sphere.center && objects.objValue[i].sphere.radius) {
          t = sphere_intersection(Ro, Rd, objects.objValue[i].sphere.center, objects.objValue[i].sphere.radius);
          printf("T\n");
        // if the current objects is a plane then call intersect
        } else if (objects.objValue[i].plane.position && objects.objValue[i].plane.normal) {
          t = plane_intersection(Ro, Rd, objects.objValue[i].plane.position, objects.objValue[i].plane.normal);
        }
        //printf("This is T: %lf\n", t);
        if (t > 0 && t < best_t) {
          best_t = t;
          colHelp = i;
        }
      }
      if (best_t > 0 && best_t != INFINITY) {
        if (objects.objValue[colHelp].sphere.center && objects.objValue[colHelp].sphere.radius) {
          shadePixel(objects.objValue[colHelp].color, image, y, x);
          printf("Shaded Sphere\n");
        } else if (objects.objValue[i].plane.position && objects.objValue[i].plane.normal) {
          shadePixel(objects.objValue[colHelp].color, image, y, x);
          printf("Shaded plane\n");
        }
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  };

  FILE* output = fopen(argv[4], "W+");
  ppmMaker(image, output, mNum);
  return 0;
}
