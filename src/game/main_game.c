
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

//-----------------------------------------------------------------------------------------------------------------------------

void render_mesh(EngineContext* ctx, mesh* mesh_to_render, vec4* face_color){
  GameContext* gctx = ctx->gameContext;

  mesh triangles_to_render = mesh_create(3);
  
  for(int i = 0; i < mesh_to_render->numTriangles; i++){
    triangle tri = mesh_to_render->triangles[i];
    triangle tri_world, tri_view, tri_proj;

    // world 
    for(int v = 0; v < 3; v++) tri_world.p[v] = vec4_transform(&tri.p[v], &mesh_to_render->mat_world);
   
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
      for(int v = 0; v < 3; v++) tri_view.p[v] = vec4_transform(&tri_world.p[v], &gctx->mat_view);
      
      // screen
      for(int v = 0; v < 3; v++){
        tri_proj.p[v]   = vec4_transform(&tri_view.p[v], &gctx->mat_proj);
        tri_proj.p[v].x += 1.0;
        tri_proj.p[v].y += 1.0;
        tri_proj.p[v].z += 1.0;
        tri_proj.p[v].x *= 0.5f * (f32) ctx->WIDTH;
        tri_proj.p[v].y *= 0.5f * (f32) ctx->HEIGHT;
      }

      // face color
      f32 intensity = 1;
      vec4 color = vec4_mul(face_color, intensity*align_light); 
      vec4 global_ilu = (vec4) {1, 10, 10, 10};
      color = vec4_add(&color, &global_ilu); 
      color = vec4_clamp(&color, 0, 255);
      tri_proj.r = color.x;
      tri_proj.g = color.y;
      tri_proj.b = color.z;
     
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

//-----------------------------------------------------------------------------------------------------------------------------

cube create_cube(vec4 pos, vec4 color, vec4 thetas, f32 scale){
  // Init unit cube
  mesh cube_mesh;
  cube_mesh = mesh_create(12);

	// SOUTH 
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 0.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f });
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f });
  
	// EAST 
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f });
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f,    1, 1.0f, 0.0f, 1.0f }); 

	// NORTH 
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 1.0f, 1.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f }); 
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f,    1, 0.0f, 0.0f, 1.0f });

	// WEST 
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 1.0f,    1, 0.0f, 1.0f, 0.0f }); 
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 1.0f, 0.0f,    1, 0.0f, 0.0f, 0.0f });
 
	// TOP
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 1.0f, 0.0f,    1, 0.0f, 1.0f, 1.0f,    1, 1.0f, 1.0f, 1.0f }); 
  mesh_push_back(&cube_mesh, (triangle) { 1, 0.0f, 1.0f, 0.0f,    1, 1.0f, 1.0f, 1.0f,    1, 1.0f, 1.0f, 0.0f });

  // BOTTOM
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 0.0f }); 
  mesh_push_back(&cube_mesh, (triangle) { 1, 1.0f, 0.0f, 1.0f,    1, 0.0f, 0.0f, 0.0f,    1, 1.0f, 0.0f, 0.0f }); 

  cube c;
  c.mesh_cube = cube_mesh;
  c.pos = pos;
  c.color = color;
  c.thetas = thetas;
  c.scale = scale;

  return c;
}

void render_cube(EngineContext* ctx, cube* c){
  GameContext* gctx = ctx->gameContext;
  render_mesh(ctx, &c->mesh_cube, &c->color);
}

void update_cube(EngineContext* ctx, cube* c){
  GameContext* gctx = ctx->gameContext;  

  // Center origin
  mat4 mat_world = mat4_indentity(); 
  mat4 mat_center = mat4_translation(-0.5, -0.5, -0.5);  
  mat_world = mat4_mul(&mat_world, &mat_center);

  // Rotate
  mat4 matRotX    = mat4_rotX(c->thetas.x);
  mat4 matRotZ    = mat4_rotZ(c->thetas.z);
  mat4 matRotY    = mat4_rotY(c->thetas.y); 
  mat_world = mat4_mul(&mat_world, &matRotX);
  mat_world = mat4_mul(&mat_world, &matRotY); 
  mat_world = mat4_mul(&mat_world, &matRotZ);

  // translate
  mat4 mat_trans = mat4_translation(c->pos.x, c->pos.y, 5);  
  mat_world = mat4_mul(&mat_world, &mat_trans); 

  c->mesh_cube.mat_world = mat_world;
  
  // View
  gctx->v_up = (vec4) {1, 0, 1, 0};
  gctx->v_look_dir = (vec4) {1, 0, 0, 1}; 
  gctx->v_target = vec4_add(&gctx->camera_pos, &gctx->v_look_dir); 
  mat4 mat_camera = mat4_pointAt(&gctx->camera_pos, &gctx->v_target, &gctx->v_up);
  gctx->mat_view   = mat4_qinverse(&mat_camera); 
}

//--------------------------------------------------------------------------------------------------------------------------

void game_init(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;

  vec4 pos = (vec4) {1, 0, -0.5, 0};
  vec4 color = (vec4) {1, 240, 0, 0}; 
  vec4 thetas = (vec4) {1, 0, 0, 0};  
  gctx->example_cube = create_cube(pos, color, thetas, 1);
 
  vec4 pos2 = (vec4) {1, 0, 0.5, 0};
  vec4 color2 = (vec4) {1, 0, 240, 0}; 
  vec4 thetas2 = (vec4) {1, 0.5, 0.5, 0.5};  
  gctx->example_cube_2 = create_cube(pos2, color2, thetas2, 1);
  
  // projection matrix
  gctx->mat_proj = mat4_projection(90.0f, ((f32)ctx->HEIGHT/(f32)ctx->WIDTH), 0.1f, 1000.0f);
  
  // World things
  gctx->theta_x = 0;
  gctx->theta_y = 0;
  gctx->theta_z = 0;

  gctx->camera_pos = (vec4) {1.0, 0.0, 0.0, 0.0};
  gctx->light_pos  = (vec4) {0.5f, 0.5f, 0.5f, -1.0f}; 
}

void game_update(EngineContext* ctx){
  
  GameContext* gctx = ctx->gameContext;
  f32 dt = ctx->deltaTime; 
  
  cube* cube1 = &gctx->example_cube;
  update_cube(ctx, cube1);

  cube* cube2 = &gctx->example_cube_2;
  // cube2->thetas.x += 5*dt;

  cube2->thetas.x = -0.8;
  cube2->thetas.y = 0.5;
  cube2->thetas.z = 0.5; 
  update_cube(ctx, cube2);
  
  // keys
  const u8* currentKeyStates = SDL_GetKeyboardState(NULL);

  if (currentKeyStates[SDL_SCANCODE_A]) cube1->thetas.x -= 5*dt;
  else if (currentKeyStates[SDL_SCANCODE_D]) cube1->thetas.x += 5*dt; 
  else if (currentKeyStates[SDL_SCANCODE_W]) cube1->thetas.z -= 5*dt;
  else if (currentKeyStates[SDL_SCANCODE_S]) cube1->thetas.z += 5*dt; 
  else if (currentKeyStates[SDL_SCANCODE_E]) cube1->thetas.y -= 5*dt;
  else if (currentKeyStates[SDL_SCANCODE_R]) cube1->thetas.y += 5*dt;  

  //
  if (currentKeyStates[SDL_SCANCODE_LEFT]) gctx->camera_pos.x += 8*dt;
  if (currentKeyStates[SDL_SCANCODE_RIGHT]) gctx->camera_pos.x -= 8*dt; 
  if (currentKeyStates[SDL_SCANCODE_UP]) gctx->camera_pos.y += 8*dt;
  if (currentKeyStates[SDL_SCANCODE_DOWN]) gctx->camera_pos.y -= 8*dt;      
}

void game_render(EngineContext* ctx){
  GameContext* gctx = ctx->gameContext;
  cube* cube1 = &gctx->example_cube;
  cube* cube2 = &gctx->example_cube_2;
  
  // Background
  SDL_SetRenderDrawColor(ctx->renderer, 40, 40, 40, 255);
  SDL_RenderClear(ctx->renderer); 

  // cube->mesh_cube.mat_world = gctx->mat_world;
  render_cube(ctx, cube1);
  render_cube(ctx, cube2);

  
  // Finish 
  SDL_RenderPresent(ctx->renderer);   
}


