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

#define ATRITO 0.5


static inline
void AUX_ToEndSzLen(void* arr, size_t size, size_t len, size_t idx);
#define AUX_ToEndLen(arr, len, i) AUX_ToEndSzLen(arr, sizeof(*arr), len, i)
#define AUX_ToEnd(arr, i) AUX_ToEndLen(arr, LEN(arr), i)


typedef enum {
    AUX_TIMEOUTEVENT = 0,
    AUX_SURECLICKEVENT,
} AUX_EventType;

void AUX_FillTimeout(SDL_Event* evt) {
     evt->user = (SDL_UserEvent) {
         .type = SDL_USEREVENT,
         .code = AUX_TIMEOUTEVENT,
         .timestamp = SDL_GetTicks(),
     };
}

void AUX_NextEvent(SDL_Event* evt, uint32_t* falta, uint32_t timeout) {
    if (AUX_WaitEventTimeout(evt, falta, timeout));
    else AUX_FillTimeout(evt);
}


typedef struct {
    SDL_Rect r;

    SDL_MouseButtonEvent click;
    SDL_Point offset;
    enum {
        UNCLICKED = 0,
        CLICKING,
        DRAGGING,

        DRAG_STATE_COUNT,
    } state;
} DragDropRect;


void AUX__FillSureClick(SDL_Event* evt, DragDropRect* rect) {
     evt->user = (SDL_UserEvent) {
         .type = SDL_USEREVENT,
         .code = AUX_SURECLICKEVENT,
         .data1 = rect,
         .timestamp = SDL_GetTicks(),
     };
}
void AUX__EmitSureClick(DragDropRect* rect) {
    SDL_Event evt; AUX__FillSureClick(&evt, rect);
    SDL_PushEvent(&evt);
}

void AUX_DragDropCancel(DragDropRect* self, SDL_Event evt) {
    const bool clicked = (self->state == CLICKING) ||
                         (self->state == DRAGGING);

    switch (evt.type) {
      case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
          case SDLK_ESCAPE: if (clicked) {
              self->r.x = self->click.x + self->offset.x;
              self->r.y = self->click.y + self->offset.y;

              self->state = UNCLICKED;
          } break;
      } break;
      case SDL_MOUSEBUTTONDOWN: {
          SDL_Point loc = { evt.button.x, evt.button.y };
          if (SDL_PointInRect(&loc, &self->r)) {
              self->offset.x = self->r.x - loc.x;
              self->offset.y = self->r.y - loc.y;
              self->click = asClick(evt);

              self->state = CLICKING;
          }
      } break;
      case SDL_MOUSEBUTTONUP: {
          if (self->state == CLICKING) AUX__EmitSureClick(self);

          self->state = UNCLICKED;
      } break;
      case SDL_MOUSEMOTION: if (clicked) {
          self->r.x = evt.button.x + self->offset.x;
          self->r.y = evt.button.y + self->offset.y;

          self->state = DRAGGING;
      } break;
    }
}

int main() {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Drag, Drop, Cancel",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        const int rw = W_WIDTH/10, hmid = (W_HEIGHT-rw)/2;
        static DragDropRect quadrados[4] = {
          { .r.x = W_WIDTH*1/3-rw/2, .r.y = hmid,    .r.w=rw, .r.h=rw },
          { .r.x = W_WIDTH*2/3-rw/2, .r.y = hmid,    .r.w=rw, .r.h=rw },
          { .r.x = W_WIDTH/2  -rw/2, .r.y = hmid-rw, .r.w=rw, .r.h=rw },
          { .r.x = W_WIDTH/2  -rw/2, .r.y = hmid+rw, .r.w=rw, .r.h=rw },
        };
        static size_t clicks[4] = {0};

        AUX_NextEvent(&evt, &falta, TIMEOUT);
        switch (evt.type) {
          case SDL_USEREVENT: switch (evt.user.code) {
              case AUX_SURECLICKEVENT: {
                  // size_t idx = evt.user.data1 - (void*)quadrados;
                  clicks[LEN(clicks)-1] += 1;
              } break;

              case AUX_TIMEOUTEVENT: {
                  TFX_limpar_tela_cor(ren, BRANCO);

                  for (size_t i = 0; i < LEN(quadrados); i++) {
                      const SDL_Color idle[] = {AZUL_BEBE, CINZA, AMARELO_M};

                      const SDL_Color cor[DRAG_STATE_COUNT] = {
                          clicks[i] ? idle[(clicks[i]-1) % LEN(idle)]
                                    : PRETO,
                                      AZUL, VERMELHO_M
                      };

                      const uint8_t idx = quadrados[i].state;
                      TFX_desenhar_rect_cor(ren, quadrados[i].r, cor[idx]);
                  }

                  SDL_RenderPresent(ren);
              } break;
          } break;
        }

        /* LÓGICA */
        for (size_t i = LEN(quadrados); i--; ) {
            AUX_DragDropCancel(&quadrados[i], evt);
            if (quadrados[i].state != UNCLICKED) {
                  AUX_ToEnd(quadrados, i); AUX_ToEnd(clicks, i); break;
            }
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

static inline
void AUX_ToEndSzLen(void* arr, size_t size, size_t len, size_t idx) {
  #define elem(arr, size, idx) ((arr) + (size)*(idx))
    char *const base = arr;
    char *const curr = elem(base, size, idx);

    char buf[size];
    memcpy(buf, curr, size);

    char *const next = elem(base, size, idx+1);
    char *const last = elem(base, size, len-1);

    memmove(curr, next, last-curr);
    memcpy(last, buf, size);
  #undef elem
}
