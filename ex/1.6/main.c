#include <SDL2/SDL.h>
#include <stdbool.h>

#include "screencapture.h"
#include "AUX.h"
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100

#define TIMEOUT 30

#define RW (HEIGHT/10)
#define RH RW

#define VEL_TECL (30.0/1000)
#define VEL_AUTO (20.0/1000)

void desenhar_tabuleiro(SDL_Renderer* ren, SDL_Rect tab,
                        int num_casas_x, int num_casas_y) {
    SDL_Rect casa = {
        .w = tab.w/num_casas_x,
        .h = tab.h/num_casas_y,
    };
    for (int i = 0; i < num_casas_x; i++) {
        for (int j = 0; j < num_casas_y; j++) {
            const SDL_Color cor = (i % 2)^(j % 2) ? BRANCO : PRETO;

            casa.x = tab.x + i*casa.w;
            casa.y = tab.y + j*casa.h;

            TFX_desenhar_rect_cor(ren, casa, cor);
        }
    }
}

static const SDL_Rect janela = {.w = WIDTH, .h = HEIGHT};
int main () {
    /* INICIALIZAÇÃO DO SDL */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Corrida 3 Retângulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);


    /* INICIALIZAÇÃO DOS RETÂNGULOS */
    const int pad = RH;
    SDL_Color cores[3] = { AZUL, VERDE, AMARELO, };
    SDL_FRect rects[3] = {
        { .x = pad, .y = pad + 0*(HEIGHT)/3, RW,RH },
        { .x = pad, .y = pad + 1*(HEIGHT)/3, RW,RH },
        { .x = pad, .y = pad + 2*(HEIGHT)/3, RW,RH },
    };

    size_t r = 0;
    SDL_FRect *tempo   = &rects[r++],
              *teclado = &rects[r++],
              *mouse   = &rects[r++];

    float vel_auto = VEL_AUTO;
    float vel_teclado = 0;


    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    uint32_t antes = SDL_GetTicks();
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        AUX_WaitEventTimeout(&evt, &falta, TIMEOUT);
        uint32_t delta = DT(antes, &antes);

        switch (evt.type) {
          case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
              case SDLK_LEFT:  vel_teclado = -VEL_TECL; break;
              case SDLK_RIGHT: vel_teclado = +VEL_TECL; break;
          } break;
          case SDL_KEYUP: switch (evt.key.keysym.sym) {
              case SDLK_LEFT:  vel_teclado = 0; break;
              case SDLK_RIGHT: vel_teclado = 0; break;
          } break;

          case SDL_MOUSEMOTION: {
              mouse->x = evt.button.x;
              TFX_ClampRectPosF(mouse, janela);
          } break;
        }

        /* LINHA DE CHEGADA */
        const int pad = RH/2, ipad = WIDTH - pad;
        SDL_Rect chegada = {
            .x = ipad - WIDTH/5, .y = pad,
            .w =        WIDTH/5, .h = HEIGHT-pad*2,
        };
        const int num_casas_x = 7, num_casas_y = 11;

        /* MOVIMENTO DOS RETÂNGULOS */
        teclado->x += vel_teclado*delta;
        tempo->x += vel_auto*delta;
        TFX_ClampRectPosF(teclado, janela);
        TFX_ClampRectPosF(tempo, janela);

        /* GANHAR */
        //! [colidir com chegada]

        /* RENDERIZAÇÃO */
        TFX_limpar_tela_cor(ren, BRANCO);
        desenhar_tabuleiro(ren, chegada, num_casas_x, num_casas_y);
        TFX_desenhar_rects_cor_f(ren, rects, cores, LEN(rects));
        SDL_RenderPresent(ren);
    }


    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
