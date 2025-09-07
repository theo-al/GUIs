#include <SDL2/SDL.h>
#include <stdbool.h>

#include "screencapture.h"
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100

#define TIMEOUT 100

#define RW (HEIGHT/10)
#define RH RW

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

    /* INICIALIZAÇÃO DO ESTADO */
    uint32_t antes = SDL_GetTicks();

    SDL_Color cores[3] = { AZUL, VERDE, AMARELO, };
    SDL_FRect rects[3] = {
        { 40,20, RW,RH }, { 60,50, RW,RH }, { 10,20, RW,RH },
    };

    size_t r = 0;
    SDL_FRect *tempo   = &rects[r++],
              *teclado = &rects[r++],
              *mouse   = &rects[r++];

    /* EXECUÇÃO */
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        /* EVENTOS E TEMPORIZAÇÃO */
        SDL_WaitEventTimeout(&evt, TIMEOUT);

        uint32_t agora = SDL_GetTicks();
        uint32_t delta = agora - antes;

        switch (evt.type) {
          /* RETÂNGULO MOVIDO POR MOUSE */
          case SDL_MOUSEMOTION: {
              mouse->x = evt.button.x -RW/2;
              mouse->y = evt.button.y -RH/2;
          } break;

          /* RETÂNGULO MOVIDO POR TECLADO */
          case SDL_KEYDOWN: {
              switch (evt.key.keysym.sym) {
                case SDLK_UP:    teclado->y -= 5; break;
                case SDLK_DOWN:  teclado->y += 5; break;
                case SDLK_LEFT:  teclado->x -= 5; break;
                case SDLK_RIGHT: teclado->x += 5; break;
              }
              TFX_ClampRectPosF(teclado, janela);
          } break;
        }

        /* RETÂNGULO MOVIDO POR TEMPO */ {
            const float vel = 10;
            tempo->x += (vel/1000)*delta;
            tempo->y += (vel/1000)*delta;
            TFX_WrapRectPosF(tempo, janela);
        }

        antes = agora;

        /* RENDERIZAÇÃO */
        TFX_limpar_tela_cor(ren, BRANCO);
        TFX_desenhar_rects_cor_f(ren, rects, cores, LEN(rects));
        SDL_RenderPresent(ren);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
