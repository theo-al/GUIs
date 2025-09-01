#include <SDL2/SDL.h>
#include "screencapture.h"
#include "TFX.h"

#define WIDTH 200
#define HEIGHT 100
#define WINDOW_WIDTH WIDTH
#define WINDOW_HEIGHT HEIGHT

#define RW 10
#define RH RW

#define MAX_RECTS 10

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
    struct cor cores[MAX_RECTS+1] = {
        AZUL,
        VERDE, AMARELO, VERMELHO, PRETO, AZUL_BEBE,
        CIANO, LARANJA, VERMELHO_M, CINZA, MAGENTA,
    };
    SDL_Rect rect[MAX_RECTS+1] = {
        [0] = { 40,20, RW,RH },
    };
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        TFX_limpar_tela_cor(ren, BRANCO);
        TFX_desenhar_rects_cor(ren, rect, cores, num_rects);
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
        switch (evt.type) {
          case SDL_MOUSEBUTTONUP: if (num_rects < LEN(rect)) {
               rect[num_rects++] = (SDL_Rect) {
                   evt.button.x -RW/2, evt.button.y -RH/2, RW, RH
               };
          } break;
          case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
              case SDLK_UP:    rect->y -= 5; break;
              case SDLK_DOWN:  rect->y += 5; break;
              case SDLK_LEFT:  rect->x -= 5; break;
              case SDLK_RIGHT: rect->x += 5; break;
          } break;
        }

        if (rect->y    < 0)      rect->y = 0;
        if (rect->y+RH > HEIGHT) rect->y = HEIGHT - RH;
        if (rect->x    < 0)      rect->x = 0;
        if (rect->x+RW > WIDTH)  rect->x = WIDTH - RW;
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
