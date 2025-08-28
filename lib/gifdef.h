#ifdef NOME_GIF
  #define GERAR_GIF
#endif

#ifdef GERAR_GIF
  #define MSF_GIF_IMPL
  #include "msf_gif.h"
  #include <SDL2/SDL.h>

  #ifndef QUALITY
    #define QUALITY 8 /*max 16*/
  #endif
  #ifndef SKIP
    #define SKIP 10
  #endif
  #ifndef GIF_CEGO
    #define GIF_CEGO 1
  #endif
  #ifndef GIF_RAPIDO
    #define GIF_RAPIDO 1
  #endif

  static struct {
      MsfGifState  estado;
      SDL_Surface* superficie;
      char*        arquivo;
  } GIF;

  #define GIF_INIT(nome_arquivo, w, h) do { \
              msf_gif_begin(&GIF.estado, w, h); \
              GIF.superficie = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0); \
              GIF.arquivo = nome_arquivo; \
          } while(0)

  #define GIF_FRAME(renderer, ms) do { \
              const int dt = ms/10; \
              SDL_RenderReadPixels(renderer, NULL, \
                                   SDL_PIXELFORMAT_ABGR8888, \
                                   GIF.superficie->pixels,   \
                                   GIF.superficie->pitch);   \
              msf_gif_frame(&GIF.estado, GIF.superficie->pixels, \
                            dt, QUALITY, GIF.superficie->pitch); \
          } while(0)

  #define GIF_FRAME_COND(renderer, dt, cond) do { \
              static int it, tt; tt += dt; \
              if ((it++ % SKIP) == 0) GIF_FRAME(renderer, tt); \
              if (!(cond)) goto GIF_END; \
          } while(0)

  #define GIF_SAVE() GIF_END: do { \
              SDL_FreeSurface(GIF.superficie); \
              MsfGifResult gif = msf_gif_end(&GIF.estado);  \
                  FILE* f = fopen(GIF.arquivo, "wb");       \
                      fwrite(gif.data, gif.dataSize, 1, f); \
                  fclose(f);                                \
              msf_gif_free(gif);                            \
          } while(0)

  #if GIF_CEGO
    #define SDL_RenderPresent(...) 
  #endif
  #if GIF_RAPIDO
    #define SDL_Delay(...) 
  #endif

  #ifdef FURTO_DE_SCREENSHOT
    #define SDL_Delay(...) 
    #define SDL_RenderPresent(ren) do { \
            GIF_INIT(NOME_GIF, WINDOW_WIDTH, WINDOW_HEIGHT); \
            GIF_FRAME_COND(ren, 1, 0); \
        } while(0)
    #define SDL_DestroyRenderer(ren) do { \
        GIF_SAVE(); \
        SDL_DestroyRenderer(ren); \
    } while (0)
  #endif
#else
  #define GIF_INIT(...)
  #define GIF_FRAME(...)
  #define GIF_FRAME_COND(...)
  #define GIF_SAVE(...)
#endif//GERAR_GIF
