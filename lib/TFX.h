#ifndef _TFX_H_
#define _TFX_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "cores.h"
#include "AUX.h"

#ifndef TFXDEF
  #define TFXDEF static inline
#endif


typedef struct { int16_t x, y; } TFX_Point;
typedef struct { int16_t x0,y0, x1,y1; } TFX_Box;
typedef struct { int16_t x0,y0, x1,y1; } TFX_Line;
typedef struct { int16_t x0,y0, x1,y1, x2,y2, x3,y3; } TFX_Quad;

TFXDEF
TFX_Box rect_para_caixa(SDL_Rect rect) {
    return (TFX_Box){ rect.x, rect.y, rect.x+rect.w, rect.y+rect.h };
}

TFXDEF
SDL_Rect rect_f_para_rect(SDL_FRect ret) {
    return (SDL_Rect) {
        .x = (long)ret.x, .y = (long)ret.y,
        .w = (long)ret.w, .h = (long)ret.h,
    };
}


/* GERAL */

TFXDEF
void TFX_limpar_tela(SDL_Renderer* ren) {
    SDL_RenderClear(ren);
}

TFXDEF
void TFX_limpar_tela_cor(SDL_Renderer* ren, SDL_Color cor) {
    SDL_SetRenderDrawColor(ren, splat(cor));
    SDL_RenderClear(ren);
}

TFXDEF
void TFX_mudar_cor(SDL_Renderer* ren, SDL_Color cor) {
    SDL_SetRenderDrawColor(ren, splat(cor));
}

TFXDEF
SDL_Color TFX_cor_atual(SDL_Renderer *renderer) {
    SDL_Color cor;
    SDL_GetRenderDrawColor(renderer, splat(&cor));
    return cor;
}


/* RECT */

// Funções movidas, mantidas como macros pela retrocompatibilidade
#define TFX_ClampRectPosF(...) AUX_ClampRectPosF(__VA_ARGS__)
#define TFX_ClampRectPos(...)  AUX_ClampRectPos(__VA_ARGS__)
#define TFX_WrapRectPosF(...) AUX_WrapRectPosF(__VA_ARGS__)
#define TFX_WrapRectPos(...)  AUX_WrapRectPos(__VA_ARGS__)

#define TFX_desenhar_rect(ren, ret) TFX_desenhar_rect_offs(ren, ret, 0, 0)

TFXDEF
void TFX_desenhar_rect_offs(SDL_Renderer* ren,
                            SDL_Rect ret, const int x, const int y) {
    ret.x += x; ret.y += y;
    SDL_RenderFillRect(ren, &ret);
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

TFXDEF
void TFX_desenhar_rect_cor(SDL_Renderer* ren,
                           const SDL_Rect ret, const SDL_Color cor) {
    TFX_mudar_cor(ren, cor);
    TFX_desenhar_rect(ren, ret);
}

TFXDEF
void TFX_desenhar_rects_cor(SDL_Renderer* ren,
                            const SDL_Rect rects[],
                            const SDL_Color cores[],
                            size_t num_rects) {
    for (size_t i = 0; i < num_rects; i++) {
        TFX_desenhar_rect_cor(ren, rects[i], cores[i]);
    }
}

#define TFX_desenhar_rect_f(ren, ret) TFX_desenhar_rect_f_offs(ren, ret, 0, 0)

TFXDEF
void TFX_desenhar_rect_f_offs(SDL_Renderer* ren,
                              SDL_FRect ret, const float x, const float y) {
    ret.x += x; ret.y += y;
    TFX_desenhar_rect(ren, rect_f_para_rect(ret));
}

TFXDEF
void TFX_desenhar_rect_cor_f(SDL_Renderer* ren,
                             const SDL_FRect ret, const SDL_Color cor) {
    TFX_mudar_cor(ren, cor);
    TFX_desenhar_rect_f(ren, ret);
}

TFXDEF
void TFX_desenhar_rects_cor_f(SDL_Renderer* ren,
                              const SDL_FRect rects[],
                              const SDL_Color cores[],
                              size_t num_rects) {
    for (size_t i = 0; i < num_rects; i++) {
        TFX_desenhar_rect_cor_f(ren, rects[i], cores[i]);
    }
}


/* OUTRAS FORMAS */

TFXDEF
void TFX_desenhar_caixa_arredondada_cor(SDL_Renderer* renderer,
                                        SDL_Rect rect, int16_t r,
                                        SDL_Color cor) {
    TFX_Box caixa = rect_para_caixa(rect);
    roundedBoxRGBA(renderer, caixa.x0, caixa.y0,
                             caixa.x1, caixa.y1, r, splat(cor));
}

TFXDEF
void TFX_desenhar_caixa_arredondada(SDL_Renderer* renderer,
                                    SDL_Rect caixa, int16_t rad) {
    SDL_Color cor = TFX_cor_atual(renderer);
    TFX_desenhar_caixa_arredondada_cor(renderer, caixa, rad, cor);
}

TFXDEF
void TFX_desenhar_disco_cor(SDL_Renderer *renderer,
                            int16_t cx, int16_t cy, int16_t rad,
                            const SDL_Color cor) {
    filledCircleRGBA(renderer, cx, cy, rad, splat(cor));
}

TFXDEF
void TFX_desenhar_disco(SDL_Renderer *renderer,
                        int16_t cx, int16_t cy, int16_t rad) {
    SDL_Color cor = TFX_cor_atual(renderer);
    TFX_desenhar_disco_cor(renderer, cx, cy, rad, cor);
}

TFXDEF
void TFX_desenhar_linha_cor(SDL_Renderer *renderer,
                            TFX_Line linha, uint8_t largura, SDL_Color cor) {
    thickLineRGBA(renderer, linha.x0,linha.y0,
                            linha.x1,linha.y1, largura, splat(cor));
}

TFXDEF
void TFX_desenhar_linha(SDL_Renderer *renderer,
                        TFX_Line linha, uint8_t largura) {
    SDL_Color cor = TFX_cor_atual(renderer);
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
                            const SDL_Color cor) {
    stringRGBA(ren, x, y, str, splat(cor));
}

TFXDEF
void TFX_desenhar_texto(SDL_Renderer *ren,
                        const char *str, int16_t x, int16_t y) {
    SDL_Color cor = TFX_cor_atual(ren);
    TFX_desenhar_texto_cor(ren, str, x, y, cor);
}


/* TEXTURAS e IMAGENS */

TFXDEF
void TFX_desenhar_textura(SDL_Renderer *ren, SDL_Texture* tex, SDL_Rect r) {
    SDL_RenderCopy(ren, tex, NULL, &r);
}

#endif//_TFX_H_
