#include <SDL2/SDL.h>

#include <assert.h>
#include <stdbool.h>

#include "cores.h"
#include "AUX.h"
#include "TFX.h"


#define as(T) *(T*)&
#define transmute(T, x) (as(T)x)
#define asClick(evt) transmute(SDL_MouseButtonEvent, evt)

#define W_WIDTH  720
#define W_HEIGHT 480

#define FPS 60
#define TIMEOUT 1000/FPS


//! ttf

//! criar evento de timeout pra função não demorar pra ver que já foi
//! ^ fazer AUX_NextEventTimeout() com a mesma assinatura WaitEventTimeout
//!   ^ antes, fazer AUX_TimeoutEmit() e pôr no else

void AUX__EmitDoubleClickAndReset(int* count) {
    SDL_Event evt = {
        .user = {
            .type = SDL_USEREVENT,
            .timestamp = SDL_GetTicks(),
            .code = *count,
        },
    };
    *count = 0;
    SDL_PushEvent(&evt);
}

//! lidar com eventos (timestamps) repetidos?
//! tentar permitir desviar alguns pixels do lugar?
void AUX_DoubleClick(SDL_Event evt, uint32_t timeout) {
    static SDL_MouseButtonEvent last_click = {0};
    static int count = 0;

    //! quando for 0, setar para o atual
    const uint32_t then = last_click.timestamp;
    const uint32_t now  = evt.common.timestamp;
    const bool timed_out = (now - then > timeout);

    const bool new_click = (evt.type == SDL_MOUSEBUTTONDOWN);
    const bool same_spot = (evt.button.x == last_click.x) &&
                           (evt.button.y == last_click.y);

    const bool far_click = new_click && !same_spot;

    if (timed_out || far_click) {
        if (count) AUX__EmitDoubleClickAndReset(&count);
    }
    if (new_click) { last_click = asClick(evt); count++; }
}

int main() {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Clique duplo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        static int num_cliques;
        static SDL_Rect r = {.w = 10, .h = 10};

        if (AUX_WaitEventTimeout(&evt, &falta, TIMEOUT)) {
            AUX_DoubleClick(evt, 250);
            switch (evt.type) {
              //case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
              //    case SDLK_UP:   case SDLK_DOWN:  break;
              //    case SDLK_LEFT: case SDLK_RIGHT: break;
              //} break;
              //case SDL_KEYUP: switch (evt.key.keysym.sym) {
              //    case SDLK_UP:   case SDLK_DOWN:  break;
              //    case SDLK_LEFT: case SDLK_RIGHT: break;
              //} break;
              case SDL_MOUSEMOTION: {
                  r.x = evt.button.x - r.w/2;
                  r.y = evt.button.y - r.h/2;
              } break;
              case SDL_USEREVENT: {
                  num_cliques += evt.user.code;
              } break;
            }
        } else {
            TFX_limpar_tela_cor(ren, BRANCO);

            #define mod_idx(arr, i) arr[i%LEN(arr)]
            SDL_Color cores[] = { AZUL, VERDE, VERMELHO, MAGENTA, AMARELO };
            TFX_desenhar_rect_cor(ren, r, mod_idx(cores, num_cliques));

            //char buf[255] = ""; sprintf(buf, "%d", num_cliques);
            //TFX_desenhar_texto(ren, buf, 0,0);

            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
