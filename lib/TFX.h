#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "cores.h"

#ifndef TFXDEF
  #define TFXDEF static inline
#endif

#define LEN(arr) (sizeof(arr)/sizeof(*arr))
#define UNUSED(x) (void) x


typedef struct { int16_t x, y; } TFX_Point;
typedef struct { int16_t x0,y0, x1,y1; } TFX_Box;
typedef struct { int16_t x0,y0, x1,y1; } TFX_Line;
typedef struct { int16_t x0,y0, x1,y1, x2,y2, x3,y3; } TFX_Quad;

TFXDEF
TFX_Box rect_para_caixa(SDL_Rect rect) {
    return (TFX_Box){ rect.x, rect.y, rect.x+rect.w, rect.y+rect.h };
}


/* GERAL */

TFXDEF
void TFX_limpar_tela(SDL_Renderer* ren) {
    SDL_RenderClear(ren);
}

TFXDEF
void TFX_limpar_tela_cor(SDL_Renderer* ren, struct cor cor) {
    SDL_SetRenderDrawColor(ren, splat(cor));
    SDL_RenderClear(ren);
}

TFXDEF
void TFX_mudar_cor(SDL_Renderer* ren, struct cor cor) {
    SDL_SetRenderDrawColor(ren, splat(cor));
}

TFXDEF
struct cor TFX_cor_atual(SDL_Renderer *renderer) {
    struct cor cor;
    SDL_GetRenderDrawColor(renderer, splat(&cor));
    return cor;
}


/* RECT */

#define TFX_desenhar_rect(ren, ret) TFX_desenhar_rect_offs(ren, ret, 0, 0)

TFXDEF
void TFX_desenhar_rect_offs(SDL_Renderer* ren,
                            SDL_Rect ret, const int x, const int y) {
    ret.x += x; ret.y += y;
    SDL_RenderFillRect(ren, &ret);
}

TFXDEF
void TFX_desenhar_rect_cor(SDL_Renderer* ren,
                           SDL_Rect ret, const struct cor cor) {
    TFX_mudar_cor(ren, cor);
    TFX_desenhar_rect(ren, ret);
}

TFXDEF
void TFX_desenhar_lista_rect_offs(SDL_Renderer* ren,
                                  const struct SDL_Rect* rects,
                                  const size_t len,
                                  const int x, const int y) {
    for (size_t i = 0; i<len; i++) {
        TFX_desenhar_rect_offs(ren, rects[i], x, y);
    }
}


/* OUTRAS FORMAS */

TFXDEF
void TFX_desenhar_caixa_arredondada_cor(SDL_Renderer* renderer,
                                        SDL_Rect rect, int16_t r,
                                        struct cor cor) {
    TFX_Box caixa = rect_para_caixa(rect);
    roundedBoxRGBA(renderer, caixa.x0, caixa.y0,
                             caixa.x1, caixa.y1, r, splat(cor));
}

TFXDEF
void TFX_desenhar_caixa_arredondada(SDL_Renderer* renderer,
                                    SDL_Rect caixa, int16_t rad) {
    struct cor cor = TFX_cor_atual(renderer);
    TFX_desenhar_caixa_arredondada_cor(renderer, caixa, rad, cor);
}

TFXDEF
void TFX_desenhar_disco_cor(SDL_Renderer *renderer,
                            int16_t cx, int16_t cy, int16_t rad,
                            const struct cor cor) {
    filledCircleRGBA(renderer, cx, cy, rad, splat(cor));
}

TFXDEF
void TFX_desenhar_disco(SDL_Renderer *renderer,
                        int16_t cx, int16_t cy, int16_t rad) {
    struct cor cor = TFX_cor_atual(renderer);
    TFX_desenhar_disco_cor(renderer, cx, cy, rad, cor);
}

TFXDEF
void TFX_desenhar_linha_cor(SDL_Renderer *renderer,
                            TFX_Line linha, uint8_t largura, struct cor cor) {
    thickLineRGBA(renderer, linha.x0,linha.y0,
                            linha.x1,linha.y1, largura, splat(cor));
}

TFXDEF
void TFX_desenhar_linha(SDL_Renderer *renderer,
                        TFX_Line linha, uint8_t largura) {
    struct cor cor = TFX_cor_atual(renderer);
    TFX_desenhar_linha_cor(renderer, linha, largura, cor);
}


/* TEXTO */

TFXDEF
void TFX_mudar_tamanho_fonte(unsigned int tam_fonte) {
    UNUSED(tam_fonte); //! implementar
}

TFXDEF
void TFX_desenhar_texto_cor(SDL_Renderer *ren,
                            const char *str, int16_t x, int16_t y,
                            const struct cor cor) {
    stringRGBA(ren, x, y, str, splat(cor));
}

TFXDEF
void TFX_desenhar_texto(SDL_Renderer *ren,
                        const char *str, int16_t x, int16_t y) {
    struct cor cor = TFX_cor_atual(ren);
    TFX_desenhar_texto_cor(ren, str, x, y, cor);
}

