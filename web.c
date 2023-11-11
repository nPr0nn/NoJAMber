

#include "core.c"
#include "src/engine/engine_context.h"
#include <stdio.h>

typedef void (*main_loop_ptr)(void*);
#ifdef WEB 
  #include <emscripten.h>
  void run(void* ctx, main_loop_ptr func){
    emscripten_set_main_loop_arg(func, ctx, -1, 1);   
  }
#else 
  void run(EngineContext* ctx, main_loop_ptr func){
    printf("WEB not defined :(");
  }
#endif

int main(void){ 
  EngineContext context;
  context.startContext = true;
  context.endContext   = false;
  context.loop = false; 
  run(&context, engine_main);
}

