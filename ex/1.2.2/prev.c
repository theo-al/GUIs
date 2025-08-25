#include <SDL2/SDL.h>
#include "TFX.h"
#include "screenshot.h"
#include "cores.h"


/* DEFINIÇÕES */
#define LEN(arr) (sizeof(arr)/sizeof(*arr))

#define WINDOW_WIDTH  720
#define WINDOW_HEIGHT 720

void desenhar_cores(SDL_Renderer* ren, const uint16_t escala);

int main() {
    /* INICIALIZAÇÃO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Desenho qualquer",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WINDOW_WIDTH, WINDOW_HEIGHT,
                          SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    TFX_limpar_tela_cor(ren, BRANCO);

    const int escala = WINDOW_WIDTH*2/3;
    desenhar_cores(ren, escala);

    TFX_mudar_cor(ren, CINZA);

    const int grande = WINDOW_WIDTH/18;
    TFX_mudar_tamanho_fonte(grande);
    TFX_desenhar_texto(ren, "THEO", WINDOW_WIDTH - grande*6, grande);

    const int tam_fonte = WINDOW_WIDTH/55, pad = tam_fonte/4;
    TFX_mudar_tamanho_fonte(tam_fonte);
    TFX_desenhar_texto(ren, "TV faz quengo explodir com whisky JB",
                       pad, WINDOW_WIDTH - pad - (tam_fonte+pad)*2);
    TFX_desenhar_texto(ren, "The quick brown fox jumps over the lazy dog",
                       pad, WINDOW_WIDTH - pad - (tam_fonte+pad)*3);

    SDL_RenderPresent(ren);
    SDL_Delay(1000);

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void desenhar_cores(SDL_Renderer* ren, const uint16_t escala) {
  #define grade(x, y, tam) { x*(tam), y*(tam), tam, tam }
    if (escala <= 1) return;

    const struct quad {
        SDL_Rect ret; struct cor cor;
    } quads[] = {
        { grade(0, 1, escala), AZUL     },
        { grade(1, 2, escala), VERDE    },
        { grade(2, 3, escala), VERMELHO },
        { grade(3, 3, escala), AMARELO  },
        { grade(4, 2, escala), MAGENTA  },
        { grade(2, 1, escala), CIANO    },
    };

    for (size_t i = 0; i<LEN(quads); i++) {
        TFX_mudar_cor(ren, quads[i].cor);
        TFX_desenhar_rect(ren, quads[i].ret);
    }

    desenhar_cores(ren, escala/4);
  #undef grade
}
