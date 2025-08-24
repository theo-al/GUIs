#include <SDL2/SDL.h>
#include "cores.h"

#define LEN(arr) (sizeof(arr)/sizeof(*arr))

void mudar_cor(SDL_Renderer* ren, struct cor cor) {
    SDL_SetRenderDrawColor(ren, splat(cor));
}

#define desenhar_rect(ren, ret) desenhar_rect_offs(ren, ret, 0, 0)
void desenhar_rect_offs(SDL_Renderer* ren,
                        SDL_Rect ret, const int x, const int y) {
    ret.x += x; ret.y += y;
    SDL_RenderFillRect(ren, &ret);
}

void desenhar_lista_offs(SDL_Renderer* ren,
                         const struct SDL_Rect* rects, const size_t len,
                         const int x, const int y) {
    for (size_t i = 0; i<len; i++) {
        desenhar_rect_offs(ren, rects[i], x, y);
    }
}

