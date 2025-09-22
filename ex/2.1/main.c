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


//! sinuca

//! criar evento de timeout pra função não demorar pra ver que já foi
//! ^ fazer AUX_NextEventTimeout() com a mesma assinatura WaitEventTimeout
//!   ^ antes, fazer AUX_TimeoutEmit() e pôr no else

void AUX__EmitDoubleClickAndReset(int* count) { //! receber first_click
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
    static SDL_MouseButtonEvent old = {0};
    static int count = 0;

    const bool clicking = (count != 0);

    const uint32_t then = old.timestamp;
    const uint32_t now  = evt.common.timestamp;
    const bool timed_out = clicking && (now - then > timeout);

    const bool new_click = (evt.type == SDL_MOUSEBUTTONDOWN);
    const bool same_spot = (evt.button.x == old.x) &&
                           (evt.button.y == old.y);

    const bool far_click = clicking && (new_click && !same_spot);

    if (timed_out || far_click) AUX__EmitDoubleClickAndReset(&count);
    if (new_click) {
        count++; if (!clicking) old = asClick(evt);
    }
}

const SDL_Rect ret_ini = {.x = W_WIDTH/2, .y = W_HEIGHT/2, .w = 10, .h = 10};
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
        static unsigned int num_cliques;
        static SDL_Rect r = ret_ini;

        if (AUX_WaitEventTimeout(&evt, &falta, TIMEOUT)) {
            AUX_DoubleClick(evt, 250);
            switch (evt.type) {
              case SDL_MOUSEMOTION: {
                  r.x = evt.button.x - r.w/2;
                  r.y = evt.button.y - r.h/2;
              } break;
              case SDL_USEREVENT: {
                  num_cliques = evt.user.code;
                  SDL_Log("%d", num_cliques);
              } break;
            }
        } else {
            TFX_limpar_tela_cor(ren, BRANCO);

            #define clamp_idx(arr, i) arr[i < LEN(arr) ? i : LEN(arr)-1]
            SDL_Color cores[] = { AMARELO, LARANJA, VERMELHO };
            TFX_desenhar_rect_cor(ren, r, clamp_idx(cores, num_cliques-1));

            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
