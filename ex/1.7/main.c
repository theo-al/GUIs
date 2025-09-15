#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
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
#define TAM_RET (WIDTH/3)

#define VELOCIDADE 80.0 /*px/s*/

#define SEGUNDO 1000 /*ms*/
#define TIMEOUT 50.0 /*ms*/

int main() {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animação cíclica com teclado",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_Texture* img = IMG_LoadTexture(ren, "tirinha.jpg");
    if (!img)    img = IMG_LoadTexture(ren, "ex/1.7/tirinha.jpg");
    assert(img != NULL);

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
            r.x = CENTRO_X -TAM_RET/2 + RAIO*cos(ang);
            r.y = CENTRO_Y -TAM_RET/2 - RAIO*sin(ang*mult);

            TFX_limpar_tela_cor(ren, BRANCO);
            TFX_desenhar_textura(ren, img, r);

            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZAÇÃO */
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
