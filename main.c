
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <dlfcn.h>

#include "src/engine/engine_context.h"

void run(EngineContext* context){ 
  while(1){
    while(system("make CircaEngine.so") != 0){
      fprintf(stderr, "[ERROR] Ooof! Failed to compile!\n");
      fprintf(stderr, "Press enter to try again.\n"); 
      getchar();
    }

    void* exec_module = dlopen("./CircaEngine.so", RTLD_NOW); 
    if(exec_module == NULL){
      fprintf(stderr, "[ERROR] Failed to load library. (%s)\n", dlerror());
      fprintf(stderr, "Press enter to try again.\n"); 
      getchar();
      continue;
    }

    typedef void main_func(void* context);
    main_func* engine_main = dlsym(exec_module, "engine_main");
      
    engine_main(context);
    
    endwin();
    dlclose(exec_module);
    if(context->endContext) return;
  } 
}

int main(void){ 
  EngineContext context;
  context.startContext = true;
  context.endContext   = false;
 
  #ifdef HOT
    context.loop = false;  
  #else 
    context.loop = true;
  #endif   

  run(&context);  
}

