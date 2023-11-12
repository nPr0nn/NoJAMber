
#include <math.h>
#include <stdio.h>
#include "linmath.h"
#include "../types.h"

#include <stdio.h>
#include <stdlib.h>

f32 min(f32 a, f32 b){
  if(a < b) return a;
  return b;
}

f32 max(f32 a, f32 b){
  if(a > b) return a;
  return b;
}

vec4 vec4_clamp(vec4* a, f32 minimum, f32 maximum){
  vec4 res;
  res.x = min(max(a->x, minimum), maximum);
  res.y = min(max(a->y, minimum), maximum);
  res.z = min(max(a->z, minimum), maximum);
  return res;
}

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
  vec4 res = {1, (a->x)*k, (a->y)*k, (a->z)*k};
  return res;
}

vec4 vec4_div(vec4* a, f32 k){
  vec4 res = {1, a->x / k, a->y / k, a->z / k};
  return res;
}

f32 vec4_dot(vec4* a, vec4* b){
  return ( (a->x * b->x) + (a->y * b->y) + (a->z * b->z) );
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
  res.y = (a->z * b->x) - (b->z * a->x);
  res.z = (a->x * b->y) - (b->x * a->y);
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

mat4 mat4_pointAt(vec4* pos, vec4* target, vec4* up){
  vec4 new_forward = vec4_sub(target, pos);	
  new_forward      = vec4_normalize(&new_forward);

	// New Up direction
  f32  k   = vec4_dot(up, &new_forward);
	vec4 aux = vec4_mul(&new_forward, k);
  vec4 new_up = vec4_sub(up, &aux);
  new_up = vec4_normalize(&new_up);

	// New Right direction
  vec4 new_right = vec4_cross(&new_up, &new_forward);	

	mat4 T = init_mat4();
	T.m[0][0] = new_right.x;	
  T.m[0][1] = new_right.y;
  T.m[0][2] = new_right.z;	
  T.m[0][3] = 0.0f;
	
  T.m[1][0] = new_up.x;
  T.m[1][1] = new_up.y;
  T.m[1][2] = new_up.z;
  T.m[1][3] = 0.0f;

  T.m[2][0] = new_forward.x;
  T.m[2][1] = new_forward.y;
  T.m[2][2] = new_forward.z;
  T.m[2][3] = 0.0f;

  T.m[3][0] = pos->x;
  T.m[3][1] = pos->y;
  T.m[3][2] = pos->z;
  T.m[3][3] = 1.0f;
	
  return T;
}

mat4 mat4_qinverse(mat4* m){
  mat4 T;
  T.m[0][0] = m->m[0][0]; 
  T.m[0][1] = m->m[1][0]; 
  T.m[0][2] = m->m[2][0]; 
  T.m[0][3] = 0.0f;
  
  T.m[1][0] = m->m[0][1]; 
  T.m[1][1] = m->m[1][1]; 
  T.m[1][2] = m->m[2][1]; 
  T.m[1][3] = 0.0f;
  
  T.m[2][0] = m->m[0][2]; 
  T.m[2][1] = m->m[1][2]; 
  T.m[2][2] = m->m[2][2]; 
  T.m[2][3] = 0.0f;
  
  T.m[3][0] = -(m->m[3][0] * T.m[0][0] + m->m[3][1] * T.m[1][0] + m->m[3][2] * T.m[2][0]);
  T.m[3][1] = -(m->m[3][0] * T.m[0][1] + m->m[3][1] * T.m[1][1] + m->m[3][2] * T.m[2][1]);
  T.m[3][2] = -(m->m[3][0] * T.m[0][2] + m->m[3][1] * T.m[1][2] + m->m[3][2] * T.m[2][2]);
  T.m[3][3] = 1.0f;
  
  return T;
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

// mesh 0-0
mesh mesh_create(u32 numTriangles) {
    mesh m;
    m.numTriangles = 0;
    m.capacity = numTriangles;
    m.triangles = (triangle*)malloc(numTriangles * sizeof(triangle));

    if (m.triangles == NULL) {
        printf("Memory allocation failed\n");
    }

    return m;
}

void mesh_destroy(mesh* m) {
    free(m->triangles);
    m->numTriangles = 0;
    m->capacity = 0;
}

void mesh_ensureCapacity(mesh* m, u32 newCapacity) {
    if (newCapacity > m->capacity) {
        m->capacity = newCapacity;
        m->triangles = realloc(m->triangles, m->capacity * sizeof(triangle));

        if (m->triangles == NULL) {
            printf("Memory reallocation failed\n");
            // Handle memory reallocation failure more gracefully in your actual code
        }
    }
}

void mesh_push_back(mesh* m, triangle newTriangle) {
    mesh_ensureCapacity(m, m->numTriangles * 2);
    m->triangles[m->numTriangles++] = newTriangle;
}

int comp_triangles(const void *a, const void *b) {
    const triangle *triangleA = (const triangle *)a;
    const triangle *triangleB = (const triangle *)b;

    float z_A = (triangleA->p[0].z + triangleA->p[1].z + triangleA->p[2].z) / 3.0f;
    float z_B = (triangleB->p[0].z + triangleB->p[1].z + triangleB->p[2].z) / 3.0f;
    
    if (z_A < z_B) return -1;
    if (z_A > z_B) return 1;
    return 0;
}

void mesh_sort(mesh* m) {
    qsort(m->triangles, m->numTriangles, sizeof(triangle), comp_triangles);
}
