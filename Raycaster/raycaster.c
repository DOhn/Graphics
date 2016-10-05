#include "struct.h"
// Plymorphism in C

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

  // If value of t is below 0 return 0.
  if (t < 0.0) {
    return 0;
  }

  // Else return value of t
  return t;
}

int main() {

  Object** objects;
  objects = malloc(sizeof(Object*)*10);
  objects[0] = malloc(sizeof(Object));
  objects[0]->kind = "plane";
  
  objects[1] = malloc(sizeof(Object));
  objects[1]->kind = "sphere";

  // Objects for sphere struct.  Needs vector of center and radius.
  objects[1]->sphere.center[0] = 0;
  objects[1]->sphere.center[1] = 0;
  objects[1]->sphere.center[2] = 10;

  objects[1]->sphere.radius = 8;

  // Objects for plane.  Needs vector of positions and vector for of Norm.
  objects[0]->plane.position[0] = .2;
  objects[0]->plane.position[1] = .2;
  objects[0]->plane.position[2] = .2;

  objects[0]->plane.normal[0] = 0;
  objects[0]->plane.normal[1] = 0;
  objects[0]->plane.normal[2] = 1;

  //objects[1] = NULL;
  
  double cx = 0;;
  double cy = 0;
  double h = 100;
  double w = 100;

  int M = 80;
  int N = 80;
  int y, x, i;

  double pixheight = h / M;
  double pixwidth = w / N;

  for (y = 0; y < M; y += 1) {
    for (x = 0; x < N; x += 1) {
      double Ro[3] = {0, 0, 0};
      // Rd = normalize(P - Ro)
      double Rd[3] = {cx - (w/2) + pixwidth * (x + 0.5),
	                    cy - (h/2) + pixheight * (y + 0.5), 1};
      int current;
      normalize(Rd);
      double best_t = INFINITY;
      for (i=0; objects[i] != 0; i += 1) {
	      double t = 0;
        current = i;

  	    if(objects[i]->kind == "plane") {
    	    t = plane_intersection(Ro, Rd, objects[i]->plane.position, objects[i]->plane.normal);
    	    break;
    	  }
        else if (objects[i]->kind == "sphere") {
          t = sphere_intersection(Ro, Rd, objects[i]->sphere.center, objects[i]->sphere.radius);
          break;
        } else {
          exit(1);
        }

  	    if (t > 0 && t < best_t) {
          best_t = t; 
        }
      }
    
      if (best_t > 0 && best_t != INFINITY) {
  	    if (objects[i]->kind == "plane") {
          printf("#");
          break;
        }
        else if (objects[i]->kind == "sphere") {
          printf("0");
          break;
        }
        else {
  	      printf(".");
          break;
        }
      }
    printf("\n");
    }
  }
  
  return 0;
}
