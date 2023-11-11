
# Desktop
CC := gcc
CFLAGS := -O2 
LDFLAGS := -ldl -lncurses -lSDL2 -lGL

# Web
WEBFLAGS := -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'
EMCC := emcc

# Dirs
MAKEFLAGS += --silent
BUILDDIR := builds/linux
WEBDIR := builds/web 

#------------------------------------------------------
# Making 
 
default: $(BUILDDIR)/main $(BUILDDIR)/hotreload CircaEngine.so  

$(BUILDDIR)/main: main.c
	$(CC) -o $@ $< $(LDFLAGS)

$(BUILDDIR)/hotreload: main.c 
	$(CC) -o $@ $< $(LDFLAGS) -DHOT=1

CircaEngine.so: core.c
	$(CC) $(CFLAGS) $< -shared -o $@ $(LDFLAGS)

web: web.c
	$(EMCC) $(CFLAGS) -DWEB=1 $^ -o index.js $(WEBFLAGS)
	mv *.js builds/web
	mv *.wasm builds/web

#------------------------------------------------------
# Running
run: $(BUILDDIR)/main
	$<

hot: $(BUILDDIR)/hotreload
	$<

webrun:
	emrun builds/web/index.html
	
#------------------------------------------------------
# Cleaning
clean: 	
	rm CircaEngine.so hotreload
