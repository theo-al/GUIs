#include <SDL2/SDL.h>
#include <math.h>

#include "screencapture.h"
#include "AUX.h"
#include "TFX.h"

/* DEFINIÇÕES */
#define PI M_PI

#define WIDTH  400
#define HEIGHT 400

#define CENTRO_X (WIDTH/2)
#define CENTRO_Y (HEIGHT/2)

#define RAIO    (WIDTH/5)
#define TAM_RET (WIDTH/5/4)

#define VELOCIDADE 80.0 /*px/s*/

#define SEGUNDO 1000 /*ms*/
#define TIMEOUT 16.0 /*ms*/

int main() {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animação cíclica com teclado",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* ESTADO */
    float mult = 1;
    uint32_t falta = TIMEOUT;
    uint32_t antes = SDL_GetTicks();
    SDL_Rect r = { 0,0, TAM_RET,TAM_RET };

    /* EXECUÇÃO */
    float t = 0;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        const bool timeout = !AUX_WaitEventTimeout(&evt, &falta, TIMEOUT);

        t += DT(antes, &antes)/(float)SEGUNDO;

        const float ang = t*VELOCIDADE/RAIO;

        if (!timeout) {
          switch (evt.type) {
            case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
                case SDLK_1: mult = 1; break;
                case SDLK_2: mult = 2; break;
                case SDLK_3: mult = 3; break;
                case SDLK_4: mult = 4; break;
                case SDLK_5: mult = 5; break;
                case SDLK_6: mult = 6; break;
                case SDLK_7: mult = 7; break;
                case SDLK_8: mult = 8; break;
                case SDLK_9: mult = 9; break;
            } break;
          }
        } else {
            r.x = CENTRO_X + RAIO*cos(ang);
            r.y = CENTRO_Y - RAIO*sin(ang*mult);

            TFX_limpar_tela_cor(ren, BRANCO);
            TFX_desenhar_rect_cor(ren, r, AZUL);

            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
