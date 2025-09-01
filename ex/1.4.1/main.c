#include <SDL2/SDL.h>
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100

#define W 10
#define H W

#define MAX_RECTS (10 + 1)

int main () {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    size_t num_rects = 1;
    struct cor cores[MAX_RECTS] = {
        AZUL,
        VERDE, AMARELO, VERMELHO, PRETO, AZUL_BEBE,
        CIANO, LARANJA, VERMELHO_M, CINZA, MAGENTA,
    };
    SDL_Rect rect[MAX_RECTS] = {
        [0] = { 40,20, W,H },
    };
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        TFX_limpar_tela_cor(ren, BRANCO);
        for (size_t i = 0; i < LEN(rect); i++) {
            TFX_desenhar_rect_cor(ren, rect[i], cores[i]);
        }
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
        switch (evt.type) {
          case SDL_MOUSEBUTTONUP: if (num_rects < LEN(rect)) {
               rect[num_rects++] = (SDL_Rect) {
                   evt.button.x -W/2, evt.button.y -H/2, W, H
               };
          } break;
          case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
              case SDLK_UP:    rect->y -= 5; break;
              case SDLK_DOWN:  rect->y += 5; break;
              case SDLK_LEFT:  rect->x -= 5; break;
              case SDLK_RIGHT: rect->x += 5; break;
          } break;
        }

        if (rect->y   < 0)      rect->y = 0;
        if (rect->y+H > HEIGHT) rect->y = HEIGHT - H;
        if (rect->x   < 0)     rect->x = 0;
        if (rect->x+W > WIDTH) rect->x = WIDTH - W;
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
