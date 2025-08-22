#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef FRAMES_PULAR
  #define FRAMES_PULAR 10
#endif

#define SALVA_FRAME_NOME(arquivo, renderer, w, h) do { \
            SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0); \
            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch); \
            \
            IMG_SavePNG(surface, arquivo); \
            SDL_FreeSurface(surface); \
        } while(0)

#define SALVA_FRAME_NUM(frame_idx, renderer, w, h) do { \
            char nome[20]; \
            sprintf(nome, IMG_DIR "frame%d.png", frame_idx); \
            SALVA_FRAME_NOME(nome, renderer, w, h); \
        } while(0)

#define SALVA_FRAME(renderer, w, h) do { \
            static int it; if (it % FRAMES_PULAR == 0) { \
                SALVA_FRAME_NUM(it, renderer, w, h); \
            } it++; \
        } while(0);

#ifndef GERAR_GIF
  #define SALVA_PNG_DA_TELA(...)
  #define SALVA_FRAME(...)
#endif//GERAR_GIF
