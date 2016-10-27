#include "struct.h"
// Ploymorphism in C

void shadePixel(double *Col, Img *image, int i, int j) {
  image->color[(int)(j * 3 + (image->height - i - 1) * image->width*3)] = (unsigned char)(Col[0]*255);
  image->color[(int)(j * 3 + (image->height - i - 1) * image->width*3+1)] = (unsigned char)(Col[1]*255);
  image->color[(int)(j * 3 + (image->height - i - 1) * image->width*3+2)] = (unsigned char)(Col[2]*255);
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

double sphere_intersection(double *Ro, double *Rd, double *cent, double r) {
  double valueA = (sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[2]));
  double valueB = 2 * Rd[0] * (Ro[0] - cent[0]) + 2 * Rd[1] * (Ro[1] - cent[1]) + 2 * Rd[2] * (Ro[2] - cent[2]);
  double valueC = sqr(cent[0]) + sqr(cent[1]) + sqr(cent[2]) + sqr(Ro[0]) + sqr(Ro[1]) + sqr(Ro[2]) -2 * (cent[0] * Ro[0] + cent[1] * Ro[1] + cent[2] * Ro[2]) - sqr(r);
  
  double d = sqr(valueB) - 4 * valueA * valueC;
  if(d < 0)
    return -1;
  
  double t0 = (-valueB - sqrt(d)) / (2*valueA);
  if (t0 > 0)
    return t0;
  
  double t1 = (-valueB + sqrt(d)) / (2*valueA);
  if (t1 > 0)
    return t1;
  
  return -1;
}

double plane_intersection(double *Ro, double *Rd, double *norm, double *pos) {
  V3 v = malloc(sizeof(double) * 3);
  v3_sub(Ro, pos, v);
  
  double dist = v3_dot(norm, v);
  double denom = v3_dot(norm, Rd);
  
  dist = -(dist / denom);
  if (dist > 0)
    return dist;
  
  return 0;
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
  image->color = (char*)malloc(sizeof(char) * image->height * image->width * 3);


  // loop through the pixels
  for (y = 0; y < M; y++) {
    for (x = 0; x < N; x++) {
      double Ro[3] = {0, 0, 0};

      double Rd[3] = { objects.objValue[i].sphere.center[0] - (camW/2) + pixwidth * (x + 0.5), objects.objValue[i].sphere.center[1] - (camH/2) + pixheight * (y + 0.5), 1 };
      normalize(Rd);

      int colHelp = 0;
      double best_t = INFINITY;
      // loop through the objects figuring out which one to trace
      for (i=0; i < numObjs; i++) {
        double t = 0;

        // if the current objects is a sphere then call intersect
        if (objects.objValue[i].sphere.center && objects.objValue[i].sphere.radius) {
          t = sphere_intersection(Ro, Rd, objects.objValue[i].sphere.center, objects.objValue[i].sphere.radius);
        // if the current objects is a plane then call intersect
        } else if (objects.objValue[i].plane.position && objects.objValue[i].plane.normal) {
          t = plane_intersection(Ro, Rd, objects.objValue[i].plane.normal, objects.objValue[i].plane.position);
        }
        if (t > 0 && t < best_t) {
          best_t = t;
          colHelp = i;
        }
      }
      if (best_t > 0 && best_t != INFINITY) {
        if (objects.objValue[colHelp].sphere.center && objects.objValue[colHelp].sphere.radius) {
          //printf("Sphere: %lf %lf %lf\n", objects.objValue[colHelp].color[0], objects.objValue[colHelp].color[1], objects.objValue[colHelp].color[2]);
          shadePixel(objects.objValue[colHelp].color, image, y, x);
        } else if (objects.objValue[colHelp].plane.position && objects.objValue[colHelp].plane.normal) {
          shadePixel(objects.objValue[colHelp].color, image, y, x);
        }
      }
    }
  };

  FILE* output = fopen(argv[4], "w+");
  ppmMaker(image, output, mNum);

  fclose(output);
  return 0;
}
