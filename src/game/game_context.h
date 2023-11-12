
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../engine/types.h" 
#include "../engine/math/linmath.h"

typedef struct GameContext { 
  triangle mesh_cube[12];
  
  mat4 mat_proj;
  mat4 mat_world;
  mat4 mat_view;

  vec4 v_up;
  vec4 v_look_dir;
  vec4 v_target;

	vec4 camera_pos;
  vec4 light_pos;
  
  f32 theta_x;
  f32 theta_y;
  f32 theta_z;

  f32 yaw;

  f32 cx;
  f32 cy;
} GameContext;

#endif




