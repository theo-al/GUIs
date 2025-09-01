#ifdef NOME_GIF
  #define GERAR_GIF
#endif

#ifdef GERAR_GIF
  #define MSF_GIF_IMPL
  #include "msf_gif.h"
  #include <SDL2/SDL.h>

  #ifndef QUALITY // nível de qualidade das cores do gif
    #define QUALITY 16 /*1 a 16*/
  #endif
  #ifndef TANTOS // de quantos em quantos frames capturar
    #define TANTOS 1
  #endif

  #ifndef GIF_INTERATIVO
    #define SDL_RenderPresent(...)
    #define SDL_Delay(...)
  #endif

  static struct {
      MsfGifState  estado;
      SDL_Surface* superficie;
      char*        arquivo;
  } GIF;

  #define GIF_INIT(nome_arquivo, w, h) do { \
              msf_gif_begin(&GIF.estado, w, h); \
              GIF.superficie = SDL_CreateRGBSurface(0, w,h,32, \
                                                    0,0,0,0);  \
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

  #define TODOS (TANTOS <= 1)
  #define CADA(tantos, frame) ((frame % tantos) == 0)
  #define GIF_FRAME_COND(renderer, dt, cond) do { \
              static int fr, tt; tt += dt; \
              if (TODOS || CADA(TANTOS, fr)) { \
                  GIF_FRAME(renderer, tt); tt = 0; \
              } fr++; \
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

  #ifdef FURTO_DE_SCREENSHOT
    #define FURTO
    #define SCREENSHOT 1
  #endif

  #ifdef FURTO_DE_GIF
    #define FURTO
    #define SCREENSHOT 0
  #endif

  #ifdef FURTO
    #define SDL_Init(...) do { \
        SDL_Init(__VA_ARGS__); \
        GIF_INIT(NOME_GIF, WIDTH, HEIGHT); \
    } while(0)

    #ifndef GIF_INTERATIVO
        #undef  SDL_RenderPresent
        #define SDL_RenderPresent(ren) \
                GIF_FRAME_COND(ren, 10, !SCREENSHOT)
    #else
        #define SDL_RenderPresent(ren) do { \
            GIF_FRAME_COND(ren, 10, !SCREENSHOT); \
            SDL_RenderPresent(ren); \
        } while(0)
    #endif

    #define SDL_DestroyRenderer(ren) do { \
        GIF_SAVE(); \
        SDL_DestroyRenderer(ren); \
    } while(0)
  #endif

#else
  #define GIF_INIT(...)
  #define GIF_FRAME(...)
  #define GIF_FRAME_COND(...)
  #define GIF_SAVE(...)
#endif//GERAR_GIF
