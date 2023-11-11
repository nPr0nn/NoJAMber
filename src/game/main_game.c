
#include "game_context.h"
#include "../engine/math/linmath.c"
#include "../engine/engine_context.h"
#include <stdio.h>

void game_init(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;

	// SOUTH 
  gctx->mesh_cube[0] = (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f };
  gctx->mesh_cube[1] = (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f };
  
	// EAST 
  gctx->mesh_cube[2] = (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f };
  gctx->mesh_cube[3] = (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f,    1, 1.0f, 0.0f, 1.0f }; 

	// NORTH 
  gctx->mesh_cube[4] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 1.0f, 1.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f }; 
  gctx->mesh_cube[5] = (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f,    1, 0.0f, 0.0f, 1.0f };

	// SOUTH 
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
  gctx->theta = 0.0f;
}

void game_update(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;
  f32 theta = gctx->theta;

  mat4 matRotZ  = mat4_rotZ(theta);
  mat4 matRotX  = mat4_rotX(0.5*theta);
   
  mat4 mat_trans = mat4_translation(1, 1, 1);  
  
  gctx->mat_world = mat4_indentity();
  gctx->mat_world = mat4_mul(&matRotZ, &matRotX);
  // gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotY);  
  gctx->mat_world = mat4_mul(&gctx->mat_world, &mat_trans); 

  gctx->theta += ctx->deltaTime;

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
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
  SDL_RenderClear(ctx->renderer); 
   
  for(int i = 0; i < 12; i++){

    triangle tri = gctx->mesh_cube[i];
    triangle tri_world;
    triangle tri_proj;

    for(int v = 0; v < 3; v++){
      tri_world.p[v] = vec4_transform(&tri.p[v], &gctx->mat_world);
      tri_proj.p[v]  = vec4_transform(&tri_world.p[v], &gctx->mat_proj);

      tri_proj.p[v].x += 2.0;
      tri_proj.p[v].y += 2.0;
      tri_proj.p[v].z += 1.0;

      tri_proj.p[v].x *= 0.5f * (f32) ctx->WIDTH;
      tri_proj.p[v].y *= 0.5f * (f32) ctx->HEIGHT;
    } 

    render_triangle(ctx, tri_proj, 255, 255, 255);
   
  }

  // render_triangle_fill(ctx, t, 255, 0, 0);
  // render_triangle(ctx, t, 255, 255, 255);
  

  // Finish 
  SDL_RenderPresent(ctx->renderer);   
}


