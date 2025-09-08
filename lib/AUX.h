#ifndef _AUX_H_
#define _AUX_H_

#include <stdbool.h>
#include <SDL2/SDL.h>

#define DT(...) AUX_dt(__VA_ARGS__)
int32_t AUX_dt(uint32_t antes, uint32_t* depois) {
    uint32_t agora = SDL_GetTicks();
    uint32_t delta = agora - antes;

    if (depois) *depois = agora;
    return delta;
}

bool AUX_WaitEventTimeoutCount(SDL_Event* evt, uint32_t* ms) {
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
    bool evento = AUX_WaitEventTimeoutCount(evt, ms);
    if (!evento) *ms = timeout;

    return evento;
}

#endif//_AUX_H_
