
#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "types.h" 

typedef struct EngineContext {
  // EngineContext Lifetime
  bool startContext;
  bool endContext;
  bool loop;
  
  // Render Context 
  char* TITLE;
  u32 WIDTH, HEIGHT;
  SDL_Renderer* renderer;
  SDL_Window* window;

  // Things
  SDL_Event event;
  u64 currTime;
  u64 prevTime;
  f64 deltaTime;
  bool running;
  bool on_web;
  
} EngineContext;

#endif

