
#include <math.h>
#include <stdio.h>
#include "linmath.h"
#include "../types.h"

// vec4 functions
 
void vec4_print(vec4* a){
  printf("{ %lf % lf % lf %lf } \n", a->w, a->x, a->y, a->z);
}

vec4 vec4_add(vec4* a, vec4* b){
  vec4 res = {1, a->x + b->x, a->y + b->y, a->z + b->z};
  return res;
}

vec4 vec4_sub(vec4* a, vec4* b){
  vec4 res = {1, a->x - b->x, a->y - b->y, a->z - b->z};
  return res;
}

vec4 vec4_mul(vec4* a, f32 k){
  vec4 res = {1, a->x * k, a->y * k, a->z * k};
  return res;
}

vec4 vec4_div(vec4* a, f32 k){
  vec4 res = {1, a->x * k, a->y * k, a->z * k};
  return res;
}

f32 vec4_dot(vec4* a, vec4* b){
  return (a->x * b->x + a->y * b->y + a->z * b->z);
}

f32 vec4_len(vec4* a){
  return sqrtf(vec4_dot(a, a));
}

vec4 vec4_normalize(vec4* a){
  f32 len = vec4_len(a);
  return vec4_div(a, len);
}

vec4 vec4_cross(vec4* a, vec4* b){
  vec4 res;
  res.w = 1;
  res.x = (a->y * b->z) - (b->y * a->z);
  res.y = (a->z * b->x) - (b->z - a->x);
  res.z = (a->x * b->y) - (b->x - a->y);
  return res;
}

// mat4 functions

void mat4_print(mat4* A){
  printf("\n");
  for (int i = 0; i < 4; i++) {
    printf("|");
    for (int j = 0; j < 4; j++) printf(" %lf ", A->m[i][j]);
    printf("|\n");
  }
  printf("\n");
}

mat4 init_mat4(){
  mat4 A;
  for (int i = 0; i < 4; i++) 
    for (int j = 0; j < 4; j++) A.m[i][j] = 0; 
  return A;
}

mat4 mat4_indentity(){
  mat4 I = init_mat4();
  I.m[0][0] = 1;
  I.m[1][1] = 1;
  I.m[2][2] = 1;
  I.m[3][3] = 1;
  return I;
}

mat4 mat4_transpose(mat4* A){
  mat4 At = init_mat4();
  for (int i = 0; i < 4; i++) 
    for (int j = 0; j < 4; j++) At.m[i][j] = A->m[3-i][3-j]; 
  return At;
}

mat4 mat4_translation(f32 x, f32 y, f32 z){
  mat4 T = mat4_indentity();
  T.m[3][0] = x;
  T.m[3][1] = y;
  T.m[3][2] = z;
  return T;
}

mat4 mat4_rotX(f32 theta){
  mat4 T = mat4_indentity();
  T.m[1][1] = cosf(theta);
  T.m[1][2] = sinf(theta);
  T.m[2][1] = -sinf(theta);
  T.m[2][2] = cosf(theta);
  return T;
}

mat4 mat4_rotY(f32 theta){
  mat4 T = mat4_indentity();
  T.m[0][0] = cosf(theta);
  T.m[0][2] = sinf(theta);
  T.m[2][0] = -sinf(theta);
  T.m[2][2] = cosf(theta);
  return T;
}

mat4 mat4_rotZ(f32 theta){
  mat4 T = mat4_indentity();
  T.m[0][0] = cosf(theta);
  T.m[0][1] = sinf(theta);
  T.m[1][0] = -sinf(theta);
  T.m[1][1] = cosf(theta);
  return T;
}

mat4 mat4_projection(f32 fov, f32 aspect, f32 near, f32 far){
  mat4 T = mat4_indentity();
  f32 fov_rad = 1.0f/(tanf(fov*deg2rad*0.5f)); 
  T.m[0][0] = aspect * fov_rad;
  T.m[1][1] = fov_rad;
  T.m[2][2] = far/(far - near);
  T.m[3][2] = (-far * near)/(far - near);
  T.m[2][3] = 1.0f;
  T.m[3][3] = 0.0f;
  return T;
}

mat4 mat4_mul(mat4* A, mat4* B){
  mat4 res;	

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
				res.m[j][i] = A->m[j][0]*B->m[0][i] + A->m[j][1]*B->m[1][i] + A->m[j][2]*B->m[2][i] + A->m[j][3]*B->m[3][i];

  return res;
}

vec4 vec4_transform(vec4* v, mat4* F){
  vec4 u;
  u.x = (v->w * F->m[3][0] + v->x * F->m[0][0] + v->y * F->m[1][0] + v->z * F->m[2][0]);
  u.y = (v->w * F->m[3][1] + v->x * F->m[0][1] + v->y * F->m[1][1] + v->z * F->m[2][1]);
  u.z = (v->w * F->m[3][2] + v->x * F->m[0][2] + v->y * F->m[1][2] + v->z * F->m[2][2]); 
  u.w = (v->w * F->m[3][3] + v->x * F->m[0][3] + v->y * F->m[1][3] + v->z * F->m[2][3]); 
  return u;
}

