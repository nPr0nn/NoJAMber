
#ifndef LINMATH_H
#define LINMATH_H

#include "../types.h"

const f64 PI = 3.14159265358;
const f64 deg2rad = 180.0f/PI;
const f64 rad2deg = PI/180.0f;

typedef struct vec4{
  f32 w;
  f32 x;
  f32 y;
  f32 z;
} vec4;

typedef struct mat4{
  f32 m[4][4];
} mat4;

typedef struct triangle{
  vec4 p[3];
  u32 r;
  u32 g;
  u32 b;
} triangle;

typedef struct mesh {
  triangle* triangles;
  mat4 mat_world;
  u32 numTriangles;
  u32 capacity;
} mesh;

typedef struct cube {
  mesh mesh_cube;
  vec4 color;
  vec4 pos;
  vec4 thetas;
  f32 scale;
} cube;

#endif // LINMATH_H
