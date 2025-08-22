CFLAGS = -Wall -Wextra -I lib/
LFLAGS = -lm -lSDL2

PKG_CONFIG := $(shell which pkg-config)
ifeq ($(PKG_CONFIG),)
    SDL_IMG_FLAGS = -lSDL2_image
else
    SDL_IMG_FLAGS = $(shell pkg-config --libs --cflags sdl2 SDL2_image)
endif

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)
                          
%.gif.out: %.c            
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) $(SDL_IMG_FLAGS) -DGERAR_GIF -DIMG_DIR='"/tmp/"'

DURACAO_GIF = 2
%.gif: %.gif.out
	$< & sleep $(DURACAO_GIF)
	killall -s SIGKILL $<
