
#include "game_context.h"
#include "../engine/math/linmath.c"
#include "../engine/engine_context.h"
#include <stdio.h>

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

//--------------------------------------------------------------------------------------------------------------------------

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

  gctx->cx = 0.0f;
  gctx->cy = 0.0f;  

  gctx->camera_pos = (vec4) {1.0, 0.0, 0.0, 0.0};
  gctx->light_pos  = (vec4) {1.0f, 0.0f, 0.0f, -1.0f}; 
}

void game_update(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;
  f32 dt = ctx->deltaTime; 

  // Center origin
  gctx->mat_world = mat4_indentity(); 
  mat4 mat_center = mat4_translation(-0.5, -0.5, -0.5);  
  gctx->mat_world = mat4_mul(&gctx->mat_world, &mat_center);

  // Rotate
  mat4 matRotX    = mat4_rotX(gctx->theta_x);
  mat4 matRotZ    = mat4_rotZ(gctx->theta_z);
  mat4 matRotY    = mat4_rotZ(gctx->theta_y); 
  gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotX);
  gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotY); 
  gctx->mat_world = mat4_mul(&gctx->mat_world, &matRotZ);

  // View
  gctx->v_up = (vec4) {1, 0, 1, 0};
  gctx->v_look_dir = (vec4) {1, 0, 0, 1};
 
  // gctx->v_target = (vec4) {1, 0, 0, 1};
  // mat4 mat_camera_rot = mat4_rotY(gctx->yaw);
  // gctx->v_look_dir = vec4_transform(&gctx->v_target, &mat_camera_rot);
  // gctx->v_target = vec4_add(&gctx->v_look_dir, &gctx->camera_pos);

  gctx->v_target = vec4_add(&gctx->camera_pos, &gctx->v_look_dir); 
  mat4 mat_camera = mat4_pointAt(&gctx->camera_pos, &gctx->v_target, &gctx->v_up);
  gctx->mat_view   = mat4_qinverse(&mat_camera);

  
  mat4 mat_trans = mat4_translation(0, gctx->cx, 5);  
  gctx->mat_world = mat4_mul(&gctx->mat_world, &mat_trans); 
 
  // keys
  const u8* currentKeyStates = SDL_GetKeyboardState(NULL);

  
  // if (currentKeyStates[SDL_SCANCODE_A]) gctx->yaw -= 2*dt;
  // if (currentKeyStates[SDL_SCANCODE_D]) gctx->yaw += 2*dt; 
  //
  // vec4 v_forward = vec4_mul(&gctx->v_look_dir, 8*dt);
  // 
  // if (currentKeyStates[SDL_SCANCODE_W]) gctx->camera_pos = vec4_add(&gctx->camera_pos, &v_forward);
  // if (currentKeyStates[SDL_SCANCODE_S]) gctx->camera_pos = vec4_sub(&gctx->camera_pos, &v_forward);


  if (currentKeyStates[SDL_SCANCODE_A]) gctx->theta_x -= 5*dt;
  if (currentKeyStates[SDL_SCANCODE_D]) gctx->theta_x += 5*dt; 
  if (currentKeyStates[SDL_SCANCODE_W]) gctx->theta_z -= 5*dt;
  if (currentKeyStates[SDL_SCANCODE_S]) gctx->theta_z += 5*dt; 
  //
  if (currentKeyStates[SDL_SCANCODE_LEFT]) gctx->camera_pos.x += 8*dt;
  if (currentKeyStates[SDL_SCANCODE_RIGHT]) gctx->camera_pos.x -= 8*dt; 
  if (currentKeyStates[SDL_SCANCODE_UP]) gctx->camera_pos.y += 8*dt;
  if (currentKeyStates[SDL_SCANCODE_DOWN]) gctx->camera_pos.y -= 8*dt; 
   
  // 
  gctx->cx += 1.0f*dt;
  gctx->cy += 1.0f*dt;     
}

void render_cube(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;

  mesh triangles_to_render = mesh_create(3);
  
  for(int i = 0; i < 12; i++){
    triangle tri = gctx->mesh_cube[i];
    triangle tri_world, tri_view, tri_proj;

    for(int v = 0; v < 3; v++){ 
      tri_world.p[v] = vec4_transform(&tri.p[v], &gctx->mat_world);
    }
    // printf("\n");
    
    // normal 
    vec4 u1     = vec4_sub(&tri_world.p[1], &tri_world.p[0]);
		vec4 u2     = vec4_sub(&tri_world.p[2], &tri_world.p[0]);
    vec4 normal = vec4_cross(&u1, &u2);
    normal      = vec4_normalize(&normal);

    // light
		vec4 ray_camera = vec4_sub(&tri_world.p[0], &gctx->camera_pos);
    ray_camera = vec4_normalize(&ray_camera);
    f32 align_camera_normal = vec4_dot(&normal, &ray_camera);

    if(align_camera_normal <= 0.15f){
      
      f32 align_light = max(0.1f, vec4_dot(&gctx->light_pos, &normal));      
    
      // view 
      // printf("View\n");
      for(int v = 0; v < 3; v++){ 
        tri_view.p[v] = vec4_transform(&tri_world.p[v], &gctx->mat_view);
        // vec4_print(&tri_view.p[v]);
      }
      // printf("\n");
      
      for(int v = 0; v < 3; v++){
        // projection on screen
        tri_proj.p[v]   = vec4_transform(&tri_view.p[v], &gctx->mat_proj);
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

      tri_proj.r = face_color.x;
      tri_proj.g = face_color.y;
      tri_proj.b = face_color.z;
     
      mesh_push_back(&triangles_to_render, tri_proj);
    } 
  }

  mesh_sort(&triangles_to_render);

  for(int i = 0; i < triangles_to_render.numTriangles; i++){
    triangle tri = triangles_to_render.triangles[i];
    render_triangle_fill(ctx, tri, tri.r, tri.g, tri.b);
  }
  
  mesh_destroy(&triangles_to_render);

  
}

void game_render(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;
  
  // Background
  SDL_SetRenderDrawColor(ctx->renderer, 40, 40, 40, 255);
  SDL_RenderClear(ctx->renderer); 
 
  render_cube(ctx);
 
  // Finish 
  SDL_RenderPresent(ctx->renderer);   
}


