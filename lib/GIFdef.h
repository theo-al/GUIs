#ifdef GIF
#include <SDL2/SDL.h>
#define MSF_GIF_IMPL
#include "msf_gif.h"

static MsfGifState ESTADO_GIF;
static char*      ARQUIVO_GIF;

#ifndef QUALITY
  #define QUALITY 16
#endif

#define GIF_INIT(arquivo, w, h) do { \
            msf_gif_begin(&ESTADO_GIF, w, h); \
            ARQUIVO_GIF = arquivo; \
        } while(0)

#define GIF_FRAME(renderer, dt) do { \
            const int w = ESTADO_GIF.width, h = ESTADO_GIF.height; \
            SDL_Surface* s = SDL_CreateRGBSurface(0, w,h,32, 0, 0, 0, 0); \
            SDL_RenderReadPixels(renderer, NULL, \
                                 SDL_PIXELFORMAT_ABGR8888, \
                                 s->pixels, s->pitch); \
            msf_gif_frame(&ESTADO_GIF, s->pixels, dt, QUALITY, w*4); \
            SDL_FreeSurface(s); \
        } while(0)

#define GIF_SAVE() do { \
            MsfGifResult gif = msf_gif_end(&ESTADO_GIF); \
            FILE* arquivo = fopen(ARQUIVO_GIF, "wb"); \
            fwrite(gif.data, gif.dataSize, 1, arquivo); \
            fclose(arquivo); \
            msf_gif_free(gif); \
        } while(0)
#endif
