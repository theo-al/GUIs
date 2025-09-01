#include <SDL2/SDL.h>
#include "screenshot.h"
#include "TFX.h"


/* DEFINIÇÕES */
#define QTD_BLOCOS 5

#define WIDTH  720
#define HEIGHT 720

#define W WIDTH
#define H HEIGHT

void desenhar_cores(SDL_Renderer* ren, const uint16_t escala);

int main() {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Desenho qualquer",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    TFX_limpar_tela_cor(ren, AZUL_BEBE);

    const int raio = 180;
    TFX_desenhar_disco_cor(ren, W/2, H/2, raio, LARANJA);

    TFX_mudar_cor(ren, VERMELHO_M);
    {
        const int tam = raio*1/4;
        const SDL_Rect centro = {
            (W-tam)/2, (H-tam)/2, tam,tam
        };

        const int r = raio-tam;
        for (int i = 0; i < QTD_BLOCOS; i++) {
            const SDL_Rect caixa = {
                .x = centro.x + r*cos(2*M_PI*i/QTD_BLOCOS),
                .y = centro.y + r*sin(2*M_PI*i/QTD_BLOCOS),
                .w = centro.w, .h = centro.h,
            };
            TFX_desenhar_caixa_arredondada(ren, caixa, 10);
        }
        TFX_desenhar_caixa_arredondada(ren, centro, 10);
    }

    TFX_mudar_cor(ren, PRETO);
    {
        const char* texto = "Dragon Ball";
        const int sz = 8, len = strlen(texto);
        TFX_desenhar_texto(ren, texto, (W-len*sz)/2, H*5/6);
    }

    SDL_RenderPresent(ren);
    SDL_Delay(1000);

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
