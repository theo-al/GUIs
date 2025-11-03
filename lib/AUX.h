#ifndef _AUX_H_
#define _AUX_H_

#include <stdbool.h>
#include <SDL2/SDL.h>


/* MACROS */
#define LEN(arr) (sizeof(arr)/sizeof(*arr))
#define UNUSED(x) (void) x


/* EVENTOS */
#define DT(...) AUX_dt(__VA_ARGS__)
int32_t AUX_dt(uint32_t antes, uint32_t* depois) {
    uint32_t agora = SDL_GetTicks();
    uint32_t delta = agora - antes;

    if (depois) *depois = agora;
    return delta;
}

bool AUX__WaitEventTimeoutCount(SDL_Event* evt, uint32_t* ms) {
    static uint32_t antes;
    if (antes == 0) antes = SDL_GetTicks();

    bool evento = SDL_WaitEventTimeout(evt, *ms);
    if (evento) {
        uint32_t delta = DT(antes, &antes);
        *ms = (delta < *ms) ? *ms - delta : 0;
    }
    return evento;
}

bool AUX_WaitEventTimeout(SDL_Event* evt, uint32_t* ms, uint32_t timeout) {
    bool evento = AUX__WaitEventTimeoutCount(evt, ms);
    if (!evento) *ms = timeout;

    return evento;
}

/* MATEMÁTICA */
void AUX_ClampRectPos(SDL_Rect* ret, const SDL_Rect win) {
    if (ret->x < win.x) ret->x = win.x;
    if (ret->y < win.y) ret->y = win.y;

    if (ret->x+ret->w > win.x+win.w) ret->x = win.x+win.w - ret->w;
    if (ret->y+ret->h > win.y+win.h) ret->y = win.y+win.h - ret->h;
}

void AUX_ClampRectPosF(SDL_FRect* ret, const SDL_Rect win) {
    if (ret->x < win.x) ret->x = win.x;
    if (ret->y < win.y) ret->y = win.y;

    if (ret->x+ret->w > win.x+win.w) ret->x = win.x+win.w - ret->w;
    if (ret->y+ret->h > win.y+win.h) ret->y = win.y+win.h - ret->h;
}

void AUX_WrapRectPos(SDL_Rect* ret, const SDL_Rect win) {
    if (ret->x < win.x) ret->x = win.x+win.w - ret->w;
    if (ret->y < win.y) ret->y = win.y+win.h - ret->h;

    if (ret->x+ret->w > win.x+win.w) ret->x = win.x;
    if (ret->y+ret->h > win.y+win.h) ret->y = win.y;
}

void AUX_WrapRectPosF(SDL_FRect* ret, const SDL_Rect win) {
    if (ret->x < win.x) ret->x = win.x+win.w - ret->w;
    if (ret->y < win.y) ret->y = win.y+win.h - ret->h;

    if (ret->x+ret->w > win.x+win.w) ret->x = win.x;
    if (ret->y+ret->h > win.y+win.h) ret->y = win.y;
}

SDL_FPoint AUX_RectPosAdjustF(const SDL_FRect ret, const SDL_Rect win) {
    SDL_FPoint diff = {0.0, 0.0};

    const float owed_x = win.x - ret.x;
    const float owed_y = win.y - ret.y;
    if (owed_x > 0) diff.x = -owed_x;
    if (owed_y > 0) diff.y = -owed_y;

    const float extra_x = (ret.x+ret.w) - (win.x+win.w);
    const float extra_y = (ret.y+ret.h) - (win.y+win.h);
    if (extra_x > 0) diff.x = -extra_x;
    if (extra_y > 0) diff.y = -extra_y;

    return diff;
}

#endif//_AUX_H_
