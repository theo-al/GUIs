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
#define TIMEOUT (1000/FPS)
#define DOUBLE_CLICK_TIMEOUT 250

#define ATRITO 0.5


//! fazer AUX_NextEventTimeout() com a mesma assinatura WaitEventTimeout

typedef enum {
    AUX_TIMEOUTEVENT = 0,
    AUX_DOUBLECLICKEVENT,
} AUX_EventType;

void AUX_FillTimeout(SDL_Event* evt) {
     evt->user = (SDL_UserEvent) {
         .type = SDL_USEREVENT,
         .code = AUX_TIMEOUTEVENT,
         .timestamp = SDL_GetTicks(),
     };
}

void AUX_EmitTimeout() {
    SDL_Event evt;
    AUX_FillTimeout(&evt);
    SDL_PushEvent(&evt);
}

const uintmax_t half = sizeof(void*)*8/2;
const uintmax_t mask = ((uintmax_t)(1) << half) - 1;
void* data1(const intptr_t x, const intptr_t y) {
    return (void*) (
        (x << half) | (y & mask)
    );
}
SDL_Point xy(const void* data1) {
    const uintptr_t xy = (uintptr_t)data1;
    return (SDL_Point) {
        .x = (xy >> half), .y = (xy & mask),
    };
}

void AUX__FillDoubleClick(SDL_Event* evt, uintptr_t count, SDL_MouseButtonEvent info) {
    evt->user = (SDL_UserEvent) {
        .type = SDL_USEREVENT,
        .code = AUX_DOUBLECLICKEVENT,
        .data1 = data1(info.x, info.y),
        .data2 = (void*)count,
        .timestamp = SDL_GetTicks(),
    };
}
void AUX__EmitDoubleClickAndReset(int* count, SDL_MouseButtonEvent info) {
    SDL_Event evt;
    AUX__FillDoubleClick(&evt, *count, info); *count = 0;
    SDL_PushEvent(&evt);
}

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

    if (timed_out || far_click) AUX__EmitDoubleClickAndReset(&count, old);
    if (new_click) {
        count++; if (!clicking) old = asClick(evt);
    }
}

const SDL_FRect ret_ini = { .x=W_WIDTH/2, .y=W_HEIGHT/2, .w=10, .h=10 };
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
    uint32_t antes = SDL_GetTicks(), falta = TIMEOUT;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        static unsigned int intensidade;
        static SDL_FRect r = ret_ini;
        static SDL_FPoint vel;

        if (AUX_WaitEventTimeout(&evt, &falta, TIMEOUT)) {
            AUX_DoubleClick(evt, DOUBLE_CLICK_TIMEOUT);
            switch (evt.type) {
              case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
                  case SDLK_r: break; //! resetar
              } break;

              case SDL_USEREVENT: switch ((AUX_EventType)evt.user.code) {
                  case AUX_TIMEOUTEVENT: break;
                  case AUX_DOUBLECLICKEVENT: {
                      uint32_t num_cliques = (uintptr_t)evt.user.data2;
                      if (num_cliques == 0) break;

                      if (vel.x != 0 && vel.y != 0) break;
                      SDL_Point pos = xy(evt.user.data1);
                      SDL_FPoint ds = {
                          .x = (r.x - r.w/2) - pos.x,
                          .y = (r.y - r.h/2) - pos.y,
                      };

                      vel.x += ds.x*num_cliques*num_cliques;
                      vel.y += ds.y*num_cliques*num_cliques;

                      intensidade = num_cliques;
                  } break;
              } break;
            }
        } else {
            AUX_EmitTimeout();
            TFX_limpar_tela_cor(ren, BRANCO);

            #define clamp_idx(arr, i) arr[i < LEN(arr) ? i : LEN(arr)-1]
            SDL_Color cores[] = { PRETO, AMARELO, LARANJA, VERMELHO };
            TFX_desenhar_rect_cor_f(ren, r, clamp_idx(cores, intensidade));

            SDL_RenderPresent(ren);
        }
        float dt = DT(antes, &antes)/1000.0;
        r.x += vel.x*dt; vel.x *= powf(ATRITO, dt*2);
        r.y += vel.y*dt; vel.y *= powf(ATRITO, dt*2);

        if (fabs(vel.x) < .8) vel.x = 0;
        if (fabs(vel.y) < .8) vel.y = 0;

        if (vel.x == 0 && vel.y == 0) intensidade = 0;

        SDL_Rect borda = {.w = W_WIDTH, .h = W_HEIGHT};
        SDL_FPoint ds = AUX_RectPosAdjustF(r, borda);
        if (ds.x != 0) { vel.x = -vel.x; r.x += ds.x; }
        if (ds.y != 0) { vel.y = -vel.y; r.y += ds.y; }
        AUX_ClampRectPosF(&r, borda);

    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
