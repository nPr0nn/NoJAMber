
#include <SDL2/SDL.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#include "src/engine/engine_context.h"
#include "src/engine/math/linmath.h"
#include "src/engine/utils.h"

// coisas
#include "src/game/main_game.c"

#ifdef WEB 
  #include <emscripten.h> 
#endif

// TODO: adicionar os checks
bool init_window(void* args){
  EngineContext* ctx = (EngineContext*) args;
    
  SDL_Window* window = NULL;
  window = SDL_CreateWindow(ctx->TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ctx->WIDTH, ctx->HEIGHT, 0); 
  ctx->window = window;

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); 
  ctx->renderer = renderer;

  return true;
}

void init_context(void* args){
  EngineContext* ctx = (EngineContext*) args;
  
  SDL_Init(SDL_INIT_EVERYTHING); 
  ctx->TITLE  = "NoJamBer";
  ctx->WIDTH  = 1024;
  ctx->HEIGHT = 640;
  ctx->FPS    = 60;
  ctx->FRAME_TARGET = ctx->FPS/1000;
  ctx->deltaTime = 0.0f;
  ctx->prevTime = 0.0f;
  ctx->currTime = 0.0f;
  bool window_created = init_window(ctx);
  if(!window_created) exit(1);

  game_init(ctx);
  ctx->startContext = false;
  return;
}

void engine_main(void* args){
  EngineContext* ctx = (EngineContext*) args;

  if(ctx->startContext){
    init_context(ctx); 
  }
  
  SDL_SetWindowSize(ctx->window, 1080, 840);
  SDL_SetWindowTitle(ctx->window, "Circa");
  
  bool running = true;
  while(running){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT){ 
        ctx->endContext = true;
        running = false;
        #ifdef WEB 
          emscripten_cancel_main_loop();
        #endif
      }
    }
   
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), ctx->prevTime + ctx->FRAME_TARGET));
    ctx->currTime  = SDL_GetTicks();
    ctx->deltaTime = (ctx->currTime - ctx->prevTime)/1000.0f;


    game_update(ctx);   
    game_render(ctx);

    ctx->prevTime = ctx->currTime;
    
    if(!ctx->loop) break;
  }
  
  return;
}
