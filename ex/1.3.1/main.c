#include <SDL2/SDL.h>
#include <math.h>

#include "GIFdef.h" // para gerar o gif


/* DEFINIÇÕES */
#define BRANCO 0xFF,0xFF,0xFF,0x00
#define AZUL   0x00,0x00,0xFF,0x00

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 200

#define COR_FUNDO BRANCO
#define COR_RET   AZUL

#define CENTRO_X (WINDOW_WIDTH/2)
#define CENTRO_Y (WINDOW_HEIGHT/2)

#define RAIO_INICIAL 40

int main() {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animação cíclica",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUCAO */
    float raio = RAIO_INICIAL;
    SDL_Rect r = {
        CENTRO_X+raio*cos(0), CENTRO_Y+raio*sin(0), 10,10
    };

    for (float t = 0 ;; t += 0.01) {
        SDL_SetRenderDrawColor(ren, COR_FUNDO);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, COR_RET);
        SDL_RenderFillRect(ren, &r);

      #ifdef LISSAJOUS
        // faz um movimento cíclico mais complexo
        raio = RAIO_INICIAL*sin(t*2); 
      #endif

        r.x = CENTRO_X + raio*cos(t);
        r.y = CENTRO_Y + raio*sin(t);

        SDL_RenderPresent(ren);
        SDL_Delay(10); 

        // salva o frame atual num png sse GERAR_GIF tiver definido
        SALVA_FRAME(ren, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
