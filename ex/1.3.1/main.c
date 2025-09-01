#include <SDL2/SDL.h>
#include <math.h>
#include "gifdef.h"

/* DEFINIÇÕES */
#define PI  M_PI
#define TAU (2*PI)

#define BRANCO 0xFF,0xFF,0xFF,0x00
#define AZUL   0x00,0x00,0xFF,0x00

#define WIDTH  400
#define HEIGHT 400

#define COR_FUNDO BRANCO
#define COR_RET   AZUL

#define CENTRO_X (WIDTH/2)
#define CENTRO_Y (HEIGHT/2)

#define RAIO    (WIDTH/5)
#define TAM_RET (WIDTH/5/4)

#define VELOCIDADE 10.0 /*px/s*/

#define SEGUNDO 1000 /*ms*/
#define DT      50.0 /*ms*/

int main() {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animação cíclica",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    GIF_INIT(NOME_GIF, WIDTH, HEIGHT); //NOOP

    /* EXECUÇÃO */
    SDL_Rect r = { 0,0, TAM_RET,TAM_RET };

    for (float t = 0 ;; t += DT/SEGUNDO) {
        const float ang = t*VELOCIDADE/RAIO;

        r.x = CENTRO_X + RAIO*cos(ang);
        r.y = CENTRO_Y - RAIO*sin(ang);
        #ifdef LISSAJOUS // movimento cíclico mais complexo
          r.y = CENTRO_Y - RAIO*sin(ang*2); // vel != 10px/s :(
        #endif

        SDL_SetRenderDrawColor(ren, COR_FUNDO);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, COR_RET);
        SDL_RenderFillRect(ren, &r);

        GIF_FRAME_COND(ren, DT, ang < TAU); //NOOP
        SDL_RenderPresent(ren);
        SDL_Delay(DT);
    }

    /* FINALIZAÇÃO */
    GIF_SAVE(); //NOOP
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
