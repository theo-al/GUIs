#include <SDL2/SDL.h>
#include "suplemento.h"
#include "cores.h"


#define LEN(arr) (sizeof(arr)/sizeof(*arr))

/* DEFINIÇÕES */
#define WINDOW_WIDTH  720
#define WINDOW_HEIGHT 720

/* FUNÇÕES */
void desenho_recursivo(SDL_Renderer* ren, const uint16_t escala) {
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
        mudar_cor(ren, quads[i].cor);
        desenhar_rect(ren, quads[i].ret);
    }

    desenho_recursivo(ren, escala/4);
  #undef grade
}

void desenhar_texto(SDL_Renderer* ren,
                    const char* texto, const int tam_fonte, 
                    const int x, const int y) {
    #define HORI sz, ln
    #define VERT ln, sz
    
    const int sz  = tam_fonte;
    const int ln  = tam_fonte/8;
    const int pad = tam_fonte/4;

    const int end = sz - ln;

    const SDL_Rect TOP_H = { 0,     0, HORI };
    const SDL_Rect BOT_H = { 0,   end, HORI };
    const SDL_Rect MID_H = { 0, end/2, HORI };

    const SDL_Rect ESQ_V = { 0,     0, VERT };
    const SDL_Rect DIR_V = { end,   0, VERT };
    const SDL_Rect MID_V = { end/2, 0, VERT };

    const struct SDL_Rect FONTE[][6] = {
        ['-'] = {        MID_H,                              },
        ['a'] = { TOP_H, MID_H,         ESQ_V,        DIR_V, },
        ['b'] = { TOP_H, MID_H, BOT_H,  ESQ_V,        DIR_V, },
        ['c'] = { TOP_H,        BOT_H,  ESQ_V,               },
        ['d'] = { TOP_H,        BOT_H,  ESQ_V,        DIR_V, }, //! o
        ['e'] = { TOP_H, MID_H, BOT_H,  ESQ_V,               },
        ['f'] = { TOP_H, MID_H,         ESQ_V,               },
        //! falta g
        ['h'] = {        MID_H,         ESQ_V,        DIR_V, },
        ['i'] = {                       MID_V,               },
        ['j'] = { BOT_H,                DIR_V,               },
        //! falta k
        ['l'] = { BOT_H,                ESQ_V,               },
        ['n'] = { TOP_H,                ESQ_V,        DIR_V, }, //! m
        ['m'] = { TOP_H,                ESQ_V,        DIR_V, }, //! n
        ['o'] = { TOP_H,        BOT_H,  ESQ_V,        DIR_V, },
        //! falta p
        //! falta q
        //! falta r
        //! falta s
        ['t'] = { TOP_H,                       MID_V,        },
        ['u'] = { BOT_H,                ESQ_V,        DIR_V, },
        ['v'] = { BOT_H,                ESQ_V,        DIR_V, }, //! u
        //! falta w
        //! falta x
        //! falta y
        //! falta z
    };

    for (size_t i = 0; texto[i]; i++) {
        const size_t c = texto[i];
        desenhar_lista_offs(ren, FONTE[c], LEN(FONTE[c]),
                                 x+(tam_fonte+pad)*i, y);
    }

    #undef VERT
    #undef HORI
}

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
    mudar_cor(ren, BRANCO);
    SDL_RenderClear(ren);

    const int escala = WINDOW_WIDTH*2/3;
    desenho_recursivo(ren, escala);

    mudar_cor(ren, CINZA);
    desenhar_texto(ren, "theo", 40, WINDOW_WIDTH*2/3, 40);
    //desenhar_texto(ren, "the lazy brown fox jumped over the lazy dog", 40,
    //               WINDOW_WIDTH*2/3, WINDOW_WIDTH - 40);
    //desenhar_texto(ren, "TV faz quengo explodir com whisky JB", 40,
    //               WINDOW_WIDTH*2/3, WINDOW_WIDTH - 40);

    SDL_RenderPresent(ren);
    SDL_Delay(6000); 

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
