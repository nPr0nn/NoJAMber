
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

  f32 theta;
  
} GameContext;

#endif
