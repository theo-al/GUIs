#ifdef NOME_GIF
  #define GERAR_GIF
#endif
#ifdef GERAR_GIF
  #define MSF_GIF_IMPL
  #include "msf_gif.h"
  #include <SDL2/SDL.h>

  static struct {
      MsfGifState  estado;
      SDL_Surface* superficie;
      char*        arquivo;
  } GIF;

  #define GIF_INIT(nome_arquivo, w, h) do { \
              msf_gif_begin(&GIF.estado, w, h); \
              GIF.superficie = SDL_CreateRGBSurface(0, w,h,32, 0, 0, 0, 0); \
              GIF.arquivo = nome_arquivo; \
          } while(0)

  //! deve dar pra simplificar redefinindo SDL Delay, RenderPresent
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

  #ifdef FURTO_DE_SCREENSHOT
    #define SDL_Delay(...) 
    #define SDL_RenderPresent(ren) do { \
            GIF_INIT(NOME_GIF, WINDOW_WIDTH, WINDOW_HEIGHT); \
            GIF_FRAME(ren, 2, 1, 0); \
        } while(0)
    #define SDL_DestroyRenderer(ren) do { \
        GIF_SAVE(); \
        SDL_DestroyRenderer(ren); \
    } while (0)
  #endif

  #ifndef QUALITY
    #define QUALITY 16
  #endif
#else
  #define GIF_INIT(...)
  #define GIF_FRAME(...)
  #define GIF_SAVE(...)
#endif//GERAR_GIF
