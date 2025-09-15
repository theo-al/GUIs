#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>

#include "screencapture.h"
#include "AUX.h"
#include "TFX.h"

#define WIDTH 210
#define HEIGHT 100

#define TIMEOUT 30

#define RW (HEIGHT/10)
#define RH RW

#define VEL_TECL (30.0/1000)
#define VEL_AUTO (20.0/1000)


void inicializar_rects(SDL_FRect* rects);
bool chegou(SDL_FRect* competidor, SDL_FRect* linha_de_chegada);
void desenhar_tabuleiro(SDL_Renderer* ren, const SDL_FRect tab,
                        const int num_casas_x, const int num_casas_y);


void desenhar_tabuleiro(SDL_Renderer* ren, const SDL_FRect tab,
                        const int num_casas_x, const int num_casas_y) {
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

void inicializar_rects(SDL_FRect* rects){
    *rects++ = (SDL_FRect){ .x = RW, .y = RH + 0*HEIGHT/3, RW,RH };
    *rects++ = (SDL_FRect){ .x = RW, .y = RH + 1*HEIGHT/3, RW,RH };
    *rects++ = (SDL_FRect){ .x = RW, .y = RH + 2*HEIGHT/3, RW,RH };
}

bool chegou(SDL_FRect* competidor, SDL_FRect* linha_de_chegada) {
    return SDL_HasIntersectionF(competidor, linha_de_chegada);
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


    /* INICIALIZAÇÃO DO ESTADO */
    SDL_Color cores[3] = { AZUL, VERDE, AMARELO, };
    SDL_FRect rects[3]; inicializar_rects(rects);

    SDL_FRect* r = rects;
    SDL_FRect *tempo = r++, *teclado = r++, *mouse = r++;

    uint32_t final = 0;
    SDL_FRect* vencedor = NULL;

    float vel_auto = VEL_AUTO;
    float vel_teclado = 0;


    /* LINHA DE CHEGADA */
    const int pad = RH/2, ipad = WIDTH - pad;
    SDL_FRect chegada = {
        .x = ipad - WIDTH/5, .y = pad,
        .w =        WIDTH/5, .h = HEIGHT-pad*2,
    };
    const int num_casas_x = 7, num_casas_y = 11;


    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    uint32_t antes = SDL_GetTicks();
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        AUX_WaitEventTimeout(&evt, &falta, TIMEOUT); //!
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

          case SDL_MOUSEMOTION: if (!chegou(mouse, &chegada)) {
              mouse->x = evt.button.x;
          } break;
        }

        /* MOVIMENTO DOS RETÂNGULOS */
        if (!chegou(teclado, &chegada)) teclado->x += vel_teclado*delta;
        if (!chegou(tempo, &chegada))   tempo->x   += vel_auto*delta;

        for (size_t i = 0; i < LEN(rects); i++) {
            TFX_ClampRectPosF(&rects[i], janela);
        }

        /* REINÍCIO */
        if (final && SDL_GetTicks() - final > 1000) {
            final = 0; vencedor = NULL;
            inicializar_rects(rects);
        }

        /* CONDIÇÃO DE CHEGADA */
        bool fim = true;
        for (size_t i = 0; i < LEN(rects); i++) {
            if (chegou(&rects[i], &chegada)) {
                if (!vencedor) vencedor = &rects[i];
            } else fim = false;
        }
        if (fim) final = final ?: SDL_GetTicks();

        /* RENDERIZAÇÃO */
        TFX_limpar_tela_cor(ren, BRANCO);
        desenhar_tabuleiro(ren, chegada, num_casas_x, num_casas_y);
        TFX_desenhar_rects_cor_f(ren, rects, cores, LEN(rects));
        if (final) {
            char* nome_vencedor = NULL;
            if      (vencedor == tempo)   nome_vencedor = "tempo";
            else if (vencedor == teclado) nome_vencedor = "teclado";
            else if (vencedor == mouse)   nome_vencedor = "mouse";
            else assert(!"unreachable");

            char msg[LEN("O vencedor eh ") + LEN("teclado")] = "";
            sprintf(msg, "O vencedor eh %s", nome_vencedor);
            TFX_desenhar_texto_cor(ren, msg, 0,10, PRETO);
        }

        SDL_RenderPresent(ren);
    }


    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
