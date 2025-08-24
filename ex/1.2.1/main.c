#include <SDL2/SDL.h>
#include "suplemento.h"
#include "cores.h"


/* DEFINIÇÕES */
#define LEN(arr) (sizeof(arr)/sizeof(*arr))

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
    const int sz  = tam_fonte;
    const int ln  = tam_fonte/8;
    const int pad = tam_fonte/4;

    const int end = sz - ln;
    const int mid = end/2;

    #define HORI sz, ln
    #define VERT ln, sz
      const SDL_Rect TOP_H = { 0,   0, HORI };
      const SDL_Rect BOT_H = { 0, end, HORI };
      const SDL_Rect MID_H = { 0, mid, HORI };

      const SDL_Rect ESQ_V = {   0, 0, VERT };
      const SDL_Rect DIR_V = { end, 0, VERT };
      const SDL_Rect MID_V = { mid, 0, VERT };
    #undef VERT
    #undef HORI

    #define MEIO_HORI sz/2, ln
    #define MEIO_VERT ln, sz/2
      const SDL_Rect MEIO_H_MID_ESQ = {   0, mid, MEIO_HORI };
      const SDL_Rect MEIO_H_MID_DIR = { mid, mid, MEIO_HORI };

      const SDL_Rect MEIO_H_TOP_ESQ = {   0, 0, MEIO_HORI };
      const SDL_Rect MEIO_H_TOP_DIR = { mid, 0, MEIO_HORI };

      const SDL_Rect MEIO_H_BOT_ESQ = {   0, end, MEIO_HORI };
      const SDL_Rect MEIO_H_BOT_DIR = { mid, end, MEIO_HORI };

      const SDL_Rect MEIO_V_TOP_ESQ = {   0, 0, MEIO_VERT };
      const SDL_Rect MEIO_V_TOP_MID = { mid, 0, MEIO_VERT };
      const SDL_Rect MEIO_V_TOP_DIR = { end, 0, MEIO_VERT };

      //const SDL_Rect MEIO_V_BOT_ESQ = {   0, end, MEIO_VERT };
      const SDL_Rect MEIO_V_BOT_MID = { mid, mid, MEIO_VERT };
      const SDL_Rect MEIO_V_BOT_DIR = { end, mid, MEIO_VERT };
    #undef MEIO_VERT
    #undef MEIO_HORI

    const struct SDL_Rect FONTE[][6] = {
        ['a'] = { TOP_H, MID_H,         ESQ_V,        DIR_V, },

        ['c'] = { TOP_H,        BOT_H,  ESQ_V,               },
        ['d'] = { TOP_H,        BOT_H,         MID_V, DIR_V, },
        ['e'] = { TOP_H, MID_H, BOT_H,  ESQ_V,               },
        ['f'] = { TOP_H, MID_H,         ESQ_V,               },

        ['h'] = {        MID_H,         ESQ_V,        DIR_V, },
        ['i'] = { TOP_H,        BOT_H,         MID_V,        },
        ['j'] = {               BOT_H,  DIR_V,               },

        ['l'] = {               BOT_H,  ESQ_V,               },
        ['m'] = { TOP_H,                ESQ_V, MID_V, DIR_V, },
        ['n'] = { TOP_H,                ESQ_V,        DIR_V, },
        ['o'] = { TOP_H,        BOT_H,  ESQ_V,        DIR_V, },

        ['t'] = { TOP_H,                       MID_V,        },
        ['u'] = {               BOT_H,  ESQ_V,        DIR_V, },

        ['w'] = {               BOT_H,  ESQ_V, MID_V, DIR_V, },
        ['x'] = {        MID_H,                MID_V,        }, //!

        ['b'] = {
            MID_H, BOT_H,  ESQ_V,
            MEIO_V_BOT_DIR,
        },
        ['g'] = {
            TOP_H, BOT_H,  ESQ_V,
            MEIO_H_MID_DIR, MEIO_V_BOT_DIR,
        },
        ['k'] = {
            ESQ_V,
            MEIO_H_BOT_DIR, MEIO_H_TOP_DIR,
            MEIO_H_MID_ESQ,
            MEIO_V_TOP_MID, MEIO_V_BOT_MID,
        },
        ['p'] = { TOP_H, MID_H,  ESQ_V,         MEIO_V_TOP_DIR, },
        ['q'] = { TOP_H, BOT_H,  ESQ_V, DIR_V,  MEIO_V_BOT_MID, },
        ['r'] = { MID_V,  MEIO_H_TOP_DIR, },
        ['s'] = { MID_V,  MEIO_H_BOT_ESQ, MEIO_H_TOP_DIR, },
        ['v'] = {
            ESQ_V,
            MEIO_V_BOT_MID, MEIO_V_TOP_DIR,
            MEIO_H_MID_DIR, MEIO_H_BOT_ESQ,
        },
        ['y'] = { MID_H, BOT_H, DIR_V,  MEIO_V_TOP_ESQ,   },
        ['z'] = { MID_V,  MEIO_H_BOT_DIR, MEIO_H_TOP_ESQ, },
    };

    for (size_t i = 0; texto[i]; i++) {
        const size_t c = texto[i];
        desenhar_lista_offs(ren, FONTE[c], LEN(FONTE[c]),
                                 x+(tam_fonte+pad)*i, y);
    }
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

    const int tam_fonte = 12, pad = tam_fonte/4;
    desenhar_texto(ren, "tv faz quengo explodir com whisky jb",
                   tam_fonte, pad, WINDOW_WIDTH - pad - (tam_fonte+pad)*2);
    desenhar_texto(ren, "the quick brown fox jumped over the lazy dog",
                   tam_fonte, pad, WINDOW_WIDTH - pad - (tam_fonte+pad)*3);

    SDL_RenderPresent(ren);
    SDL_Delay(10000); 

    /* FINALIZAÇÃO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit(); 
}
