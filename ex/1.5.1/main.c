#include <SDL2/SDL.h>
#include <stdbool.h>

#include "screencapture.h"
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100

#define TIMEOUT 100

#define RW (HEIGHT/10)
#define RH RW

#define VEL (10.0/1000)

int32_t dt(uint32_t antes, uint32_t* depois) {
    uint32_t agora = SDL_GetTicks();
    uint32_t delta = agora - antes;

    if (depois) *depois = agora;
    return delta;
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
        { 40,20, RW,RH }, { 60,50, RW,RH }, { 10,20, RW,RH },
    };

    size_t r = 0;
    SDL_FRect *tempo   = &rects[r++],
              *teclado = &rects[r++],
              *mouse   = &rects[r++];

    SDL_FRect vel_teclado = {0}; //! tipo errado


    /* EXECUÇÃO */
    uint32_t antes = SDL_GetTicks();
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        SDL_WaitEventTimeout(&evt, TIMEOUT);
        uint32_t delta = dt(antes, &antes);

        switch (evt.type) {
          case SDL_KEYDOWN: {
              switch (evt.key.keysym.sym) {
                case SDLK_UP:    vel_teclado.y = -VEL; break;
                case SDLK_DOWN:  vel_teclado.y = +VEL; break;
                case SDLK_LEFT:  vel_teclado.x = -VEL; break;
                case SDLK_RIGHT: vel_teclado.x = +VEL; break;
              }
          } break;
          case SDL_KEYUP: {
              switch (evt.key.keysym.sym) {
                case SDLK_UP:   case SDLK_DOWN:  vel_teclado.y = 0; break;
                case SDLK_LEFT: case SDLK_RIGHT: vel_teclado.x = 0; break;
              }
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

        tempo->x += VEL*delta;
        tempo->y += VEL*delta;
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
