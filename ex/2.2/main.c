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


typedef enum {
    AUX_TIMEOUTEVENT = 0,
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
    bool clicking;
    bool dragging;
} DragDropRect;

void AUX_DragDropCancel(DragDropRect* self, SDL_Event evt) {
    const bool clicked = (self->clicking || self->dragging);

    switch (evt.type) {
      case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
          case SDLK_ESCAPE: if (clicked) {
              self->r.x = self->click.x + self->offset.x;
              self->r.y = self->click.y + self->offset.y;
              self->clicking = false;
              self->dragging = false;
          } break;
      } break;
      case SDL_MOUSEBUTTONDOWN: {
          SDL_Point loc = { evt.button.x, evt.button.y };
          if (SDL_PointInRect(&loc, &self->r)) {
              self->offset.x = self->r.x - loc.x;
              self->offset.y = self->r.y - loc.y;
              self->click = asClick(evt);
              self->clicking = true;
          }
      } break;
      case SDL_MOUSEBUTTONUP: {
          self->clicking = false;
          self->dragging = false;
      } break;
      case SDL_MOUSEMOTION: if (clicked) {
          self->r.x = evt.button.x + self->offset.x;
          self->r.y = evt.button.y + self->offset.y;
          self->dragging = true;
          self->clicking = false;
      } break;
    }

    assert(self->clicking + self->dragging <= 1);
}

int main() {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Drag&Drop",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        static DragDropRect quadrado = {
            .r.x = W_WIDTH/2,  .r.w = W_WIDTH/10,
            .r.y = W_HEIGHT/2, .r.h = W_WIDTH/10,
        };

        AUX_NextEvent(&evt, &falta, TIMEOUT);
        AUX_DragDropCancel(&quadrado, evt);
        switch (evt.type) {
          case SDL_USEREVENT: switch (evt.user.code) {
              case AUX_TIMEOUTEVENT: {
                  TFX_limpar_tela_cor(ren, BRANCO);

                  SDL_Color cor[] = { PRETO, AZUL, VERMELHO };
                  const uint8_t idx = (quadrado.clicking<<0) +
                                      (quadrado.dragging<<1);
                  assert(idx < LEN(cor));
                  TFX_desenhar_rect_cor(ren, quadrado.r, cor[idx]);

                  SDL_RenderPresent(ren);
              } break;
          } break;
        }
        /* LÓGICA */
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
