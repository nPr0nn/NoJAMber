
#include "../engine/engine_context.h"
// #include "../engine/math/linmath.c"

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
