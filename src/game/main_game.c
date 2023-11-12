
#include "game_context.h"
#include "../engine/math/linmath.c"
#include "../engine/engine_context.h"
#include <stdio.h>

void game_init(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;

	// SOUTH 
  gctx->mesh_cube[0] = (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 0.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f };
  gctx->mesh_cube[1] = (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f };
  
	// EAST 
  gctx->mesh_cube[2] = (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f };
  gctx->mesh_cube[3] = (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f,    1, 1.0f, 0.0f, 1.0f }; 

	// NORTH 
  gctx->mesh_cube[4] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 1.0f, 1.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f }; 
  gctx->mesh_cube[5] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f,    1, 0.0f, 0.0f, 1.0f };

	// WEST 
  gctx->mesh_cube[6] = (triangle) { 1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f,    1, 0.0f, 1.0f, 0.0f }; 
  gctx->mesh_cube[7] = (triangle) { 1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 0.0f,    1, 0.0f, 0.0f, 0.0f };
 
	// TOP
  gctx->mesh_cube[8] = (triangle) { 1, 0.0f, 1.0f, 0.0f,    1, 0.0f, 1.0f, 1.0f,    1, 1.0f, 1.0f, 1.0f }; 
  gctx->mesh_cube[9] = (triangle) { 1, 0.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f,    1, 1.0f, 1.0f, 0.0f };

  // BOTTOM
  gctx->mesh_cube[10] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 0.0f }; 
  gctx->mesh_cube[11] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f }; 
  
  // projection matrix
  gctx->mat_proj = mat4_projection(90.0f, ((f32)ctx->HEIGHT/(f32)ctx->WIDTH), 0.1f, 1000.0f);
  
  // World things
  gctx->theta_x = 0.0f;
  gctx->theta_y = 0.0f;
  gctx->theta_z = 0.0f;
 

  gctx->camera_pos = (vec4) {1.0, 0.0, 0.0, 0.0};
  gctx->light_pos  = (vec4) {1.0f, 0.0f, 0.0f, -1.0f}; 
}

void game_update(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;

  mat4 matRotZ  = mat4_rotZ(gctx->theta_z);
  mat4 matRotX  = mat4_rotX(gctx->theta_x);
    
  gctx->mat_world = mat4_indentity();
  
  mat4 mat_center = mat4_translation(-0.5, -0.5, -0.5);  
  gctx->mat_world = mat4_mul(&gctx->mat_world, &mat_center);

  gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotX);
  gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotZ);
  
  mat4 mat_trans = mat4_translation(0, 0, 3);  
  gctx->mat_world = mat4_mul(&gctx->mat_world, &mat_trans); 
 
  // keys
  const u8* currentKeyStates = SDL_GetKeyboardState(NULL);
  if (currentKeyStates[SDL_SCANCODE_A]) gctx->theta_x -= 0.01;
  if (currentKeyStates[SDL_SCANCODE_D]) gctx->theta_x += 0.01;
   
  if (currentKeyStates[SDL_SCANCODE_W]) gctx->theta_z -= 0.01;
  if (currentKeyStates[SDL_SCANCODE_S]) gctx->theta_z += 0.01; 

  // gctx->theta += ctx->deltaTime;
}

void render_triangle_fill(EngineContext* ctx, triangle t, i32 r, i32 g, i32 b){
  SDL_Vertex triangleVertex[3] = 
  { 
    {{t.p[0].x, t.p[0].y}, {r, g, b, 0xFF}, {0, 0}}, 
    {{t.p[1].x, t.p[1].y}, {r, g, b, 0xFF}, {0, 0}}, 
    {{t.p[2].x, t.p[2].y}, {r, g, b, 0xFF}, {0, 0}},  
  };
  SDL_RenderGeometry(ctx->renderer, NULL, triangleVertex, 3, NULL, 0); 
}

void render_triangle(EngineContext* ctx, triangle t, i32 r, i32 g, i32 b){
  SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 255);   
  SDL_RenderDrawLine(ctx->renderer, t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y);
  SDL_RenderDrawLine(ctx->renderer, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
  SDL_RenderDrawLine(ctx->renderer, t.p[2].x, t.p[2].y, t.p[0].x, t.p[0].y); 
}

void game_render(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;
  
  // Background
  SDL_SetRenderDrawColor(ctx->renderer, 40, 40, 40, 255);
  SDL_RenderClear(ctx->renderer); 
   
  for(int i = 0; i < 12; i++){

    triangle tri = gctx->mesh_cube[i];
    triangle tri_world;
    triangle tri_proj;

    // world
    for(int v = 0; v < 3; v++){ 
      tri_world.p[v] = vec4_transform(&tri.p[v], &gctx->mat_world);
    }
    
    // normal 
    vec4 u1 = vec4_sub(&tri_world.p[1], &tri_world.p[0]);
		vec4 u2 = vec4_sub(&tri_world.p[2], &tri_world.p[0]);
    vec4 normal = vec4_cross(&u1, &u2);
    normal      = vec4_normalize(&normal);

    // light
		vec4 ray_camera = vec4_sub(&tri_world.p[0], &gctx->camera_pos);
    ray_camera = vec4_normalize(&ray_camera);
    f32 align_camera_normal = vec4_dot(&normal, &ray_camera);

    if(align_camera_normal <= 0.15f){
      
      f32 align_light = max(0.1f, vec4_dot(&gctx->light_pos, &normal));      
     
      for(int v = 0; v < 3; v++){
        // projection on screen
        tri_proj.p[v]   = vec4_transform(&tri_world.p[v], &gctx->mat_proj);
        tri_proj.p[v].x += 1.0;
        tri_proj.p[v].y += 1.0;
        tri_proj.p[v].z += 1.0;
        tri_proj.p[v].x *= 0.5f * (f32) ctx->WIDTH;
        tri_proj.p[v].y *= 0.5f * (f32) ctx->HEIGHT;
      }
 
      f32 intensity = 1;
      vec4 face_color = (vec4) {1, 240, 10, 10};
      face_color = vec4_mul(&face_color, intensity*align_light); 
      face_color = vec4_clamp(&face_color, 0, 255); 
      render_triangle_fill(ctx, tri_proj, face_color.x, face_color.y, face_color.z);
    }
    
  } 

  // Finish 
  SDL_RenderPresent(ctx->renderer);   
}


