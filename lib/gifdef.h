#ifdef NOME_GIF 

#define MSF_GIF_IMPL
#include "msf_gif.h"
#include <SDL2/SDL.h>

static struct {
    MsfGifState  estado;
    SDL_Surface* superficie;
    char*        arquivo;
} GIF;

#ifndef QUALITY
  #define QUALITY 16
#endif

#define GIF_INIT(nome_arquivo, w, h) do { \
            msf_gif_begin(&GIF.estado, w, h); \
            GIF.superficie = SDL_CreateRGBSurface(0, w,h,32, 0, 0, 0, 0); \
            GIF.arquivo = nome_arquivo; \
        } while(0)

#define GIF_FRAME(renderer, dt, cond_min, cond_max) { \
            if (!(cond_min)) goto GIF_CONT; \
            SDL_RenderReadPixels(renderer, NULL,           \
                                 SDL_PIXELFORMAT_ABGR8888, \
                                 GIF.superficie->pixels,   \
                                 GIF.superficie->pitch);   \
            msf_gif_frame(&GIF.estado, GIF.superficie->pixels, \
                          dt, QUALITY, GIF.superficie->pitch); \
            if (!(cond_max)) goto GIF_END; \
            GIF_CONT: continue; \
        }

#define GIF_SAVE() GIF_END: do { \
            SDL_FreeSurface(GIF.superficie); \
            MsfGifResult gif = msf_gif_end(&GIF.estado);  \
                FILE* f = fopen(GIF.arquivo, "wb");       \
                    fwrite(gif.data, gif.dataSize, 1, f); \
                fclose(f);                                \
            msf_gif_free(gif);                            \
        } while(0)
#else

#define GIF_INIT(...)
#define GIF_FRAME(...)
#define GIF_SAVE(...)

#endif//_GIF
