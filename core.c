
#include <SDL2/SDL.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#include "src/engine/engine_context.h"
#include "src/engine/utils.h"

#ifdef WEB 
  #include <emscripten.h> 
#endif

// TODO: adicionar os checks
bool init_window(void* args){
  EngineContext* ctx = (EngineContext*) args;
    
  SDL_Window* window = NULL;
  window = SDL_CreateWindow(ctx->TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ctx->WIDTH, ctx->HEIGHT, 0); 
  ctx->window = window;

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
  ctx->renderer = renderer;

  return true;
}

void init_context(void* args){
  EngineContext* ctx = (EngineContext*) args;
  
  SDL_Init(SDL_INIT_EVERYTHING); 
  ctx->TITLE  = "NoJamBer Lets go";
  ctx->WIDTH  = 1080;
  ctx->HEIGHT = 640; 
  bool window_created = init_window(ctx);
  if(!window_created) exit(1);

  ctx->startContext = false;
  return;
}

void engine_main(void* args){
  EngineContext* ctx = (EngineContext*) args;

  if(ctx->startContext){
    init_context(ctx);
  }

  SDL_SetWindowSize(ctx->window, 1500, 960);
  SDL_SetWindowTitle(ctx->window, "NoJamBer"); 

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
    ctx->currTime  = SDL_GetTicks();
    ctx->deltaTime = (ctx->currTime - ctx->prevTime)/1000.0f; 

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);
   
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(ctx->renderer, 0, 0, 100, 100);

    SDL_RenderPresent(ctx->renderer); 

    ctx->prevTime = ctx->currTime;
    
    if(!ctx->loop) break;
  }
  
  return;
}
