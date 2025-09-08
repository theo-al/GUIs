#include <SDL2/SDL.h>
#include <stdbool.h>

#include "screencapture.h"
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100

#define TIMEOUT 30

#define RW (HEIGHT/10)
#define RH RW

#define VEL_TECL (30.0/1000)
#define VEL_AUTO (20.0/1000)

int32_t dt(uint32_t antes, uint32_t* depois) {
    uint32_t agora = SDL_GetTicks();
    uint32_t delta = agora - antes;

    if (depois) *depois = agora;
    return delta;
}

bool TFX_WaitEventTimeout(SDL_Event* evt, uint32_t* ms, uint32_t timeout) {
    static uint32_t antes;
    if (antes == 0) antes = SDL_GetTicks();

    bool evento = SDL_WaitEventTimeout(evt, *ms);
    if (!evento) *ms = timeout;
    else {
        uint32_t delta = dt(antes, &antes);
        *ms = (delta < *ms) ? *ms - delta : 0;
    }

    return evento;
}

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    return TFX_WaitEventTimeout(evt, ms, TIMEOUT);
}

static const SDL_Rect janela = {.w = WIDTH, .h = HEIGHT};
int main () {
    /* INICIALIZAÇÃO DO SDL */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo 3 Retângulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    /* INICIALIZAÇÃO DOS RETÂNGULOS */
    SDL_Color cores[3] = { AZUL, VERDE, AMARELO, };
    SDL_FRect rects[3] = {
        { 40,20, RW,RH }, { 60,50, RW,RH }, { WIDTH,HEIGHT, RW,RH },
    };

    size_t r = 0;
    SDL_FRect *tempo   = &rects[r++],
              *teclado = &rects[r++],
              *mouse   = &rects[r++];

    SDL_FRect vel_teclado = {0}; //! tipo errado


    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    uint32_t antes = SDL_GetTicks();
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        AUX_WaitEventTimeoutCount(&evt, &falta);
        uint32_t delta = dt(antes, &antes);

        switch (evt.type) {
          case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
              case SDLK_UP:    vel_teclado.y = -VEL_TECL; break;
              case SDLK_DOWN:  vel_teclado.y = +VEL_TECL; break;
              case SDLK_LEFT:  vel_teclado.x = -VEL_TECL; break;
              case SDLK_RIGHT: vel_teclado.x = +VEL_TECL; break;
          } break;
          case SDL_KEYUP: switch (evt.key.keysym.sym) {
              case SDLK_UP:   case SDLK_DOWN:  vel_teclado.y = 0; break;
              case SDLK_LEFT: case SDLK_RIGHT: vel_teclado.x = 0; break;
          } break;

          case SDL_MOUSEMOTION: {
              mouse->x = evt.button.x -RW/2;
              mouse->y = evt.button.y -RH/2;
              TFX_ClampRectPosF(teclado, janela);
          } break;
        }

        /* MOVIMENTO DOS RETÂNGULOS */
        teclado->x += vel_teclado.x*delta;
        teclado->y += vel_teclado.y*delta;
        TFX_ClampRectPosF(teclado, janela);

        tempo->x += VEL_AUTO*delta;
        tempo->y += VEL_AUTO*delta;
        TFX_WrapRectPosF(tempo, janela);

        /* RENDERIZAÇÃO */
        TFX_limpar_tela_cor(ren, BRANCO);
        TFX_desenhar_rects_cor_f(ren, rects, cores, LEN(rects));
        SDL_RenderPresent(ren);
    }


    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
