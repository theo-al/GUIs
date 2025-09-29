#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <assert.h>
#include <stdbool.h>

#include "cores.h"
#include "AUX.h"
#include "TFX.h"


#define as(T) *(T*)&
#define transmute(T, x) (as(T)x)

#define W_WIDTH  1000
#define W_HEIGHT 350

#define FONT_SIZE 16

#define FPS 60
#define TIMEOUT (1000/FPS)


typedef enum {
    AUX_TIMEOUTEVENT = 0,
} AUX_EventType;

void AUX_FillTimeout(SDL_Event* evt) {
     evt->user = (SDL_UserEvent) {
         .type = SDL_USEREVENT,
         .code = AUX_TIMEOUTEVENT,
         .timestamp = SDL_GetTicks(),
     };
}

void AUX_EmitTimeout() {
    SDL_Event evt;
    AUX_FillTimeout(&evt);
    SDL_PushEvent(&evt);
}


TTF_Font* font;

/* cache types (same as before) */
typedef struct { const char* key; SDL_Texture* tex; int w,h; } _txtcache_t;
#define _TXT_CACHE_SIZE 128
static _txtcache_t _txtcache[_TXT_CACHE_SIZE] = {0};


/* uncached variant: render once and free (use for stack buffers or transient strings) */
void _TFX_desenhar_texto(SDL_Renderer* ren, const char* texto, float x, float y, SDL_Color cor) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, texto, cor);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    int w=0,h=0;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_FreeSurface(surf);
    SDL_FRect dst = { x, y, (float)w, (float)h };
    SDL_RenderCopyF(ren, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

/* cached variant (previous implementation) */
void TFX_desenhar_texto_cache(SDL_Renderer* ren, const char* texto, float x, float y, SDL_Color cor) {
    for (int i = 0; i < _TXT_CACHE_SIZE; ++i) {
        if (_txtcache[i].key) {
            if (strcmp(_txtcache[i].key, texto) == 0) {
                SDL_FRect dst = { x, y, (float)_txtcache[i].w, (float)_txtcache[i].h };
                SDL_RenderCopyF(ren, _txtcache[i].tex, NULL, &dst);
                return;
            }
        } else {
            SDL_Surface* surf = TTF_RenderUTF8_Blended(font, texto, cor);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
            int w=0,h=0;
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            SDL_FreeSurface(surf);

            _txtcache[i].key = texto;
            _txtcache[i].tex = tex;
            _txtcache[i].w = w;
            _txtcache[i].h = h;

            SDL_FRect dst = { x, y, (float)w, (float)h };
            SDL_RenderCopyF(ren, tex, NULL, &dst);
            return;
        }
    }

    /* cache full: render-on-the-fly and destroy */
    _TFX_desenhar_texto(ren, texto, x, y, cor);
}

/* cache free (call at shutdown) */
void TFX_desenhar_texto_cache_free() {
    for (int i = 0; i < _TXT_CACHE_SIZE; ++i) {
        if (_txtcache[i].key) {
            SDL_DestroyTexture(_txtcache[i].tex);
            _txtcache[i].tex = NULL;
            _txtcache[i].key = NULL;
        }
    }
}


struct filme { uint8_t hora, min; char* nome; };
static const struct filme salas[][6] = {
    [1] = {
        {23,10, "Multiplos Maniacos"},
        { 1, 5, "O Lobisomem"},
        { 2,30, "Onibaba"},
        { 4,30, "Lemonade Mouth"},
        { 6,35, "Miracle Mile"},
        { 8, 8, ""},
    },
    [2] = {
        {23, 0, "Os Sete Samurais"},
        { 2,45, "A General"},
        { 4,20, "Viy"},
        { 5,50, "Curva do Destino"},
        { 7,15, "Chime"},
        { 8,45, ""}, //! deve tar errado
    },
    [3] = {
        {23,15, "O Fantasma do Futuro"},
        { 1, 0, "A Mentira"},
        { 2,50, "As Branquelas"},
        { 4,55, "Phenomena"},
        { 6,55, "O Homem-Leopardo"},
        { 8,01, ""},
    },
    [4] = {
        {23, 5, "Videodrome"},
        { 0,50, "Memorias"},
        { 3,00, "The Rutles"},
        { 4,35, "Os Homens Preferem as Loiras"},
        { 6,20, "La Haine"},
        { 7,58, ""},
    },
    [5] = {
        {23,20, "Cantando na Chuva"},
        { 1,25, "Memorias de Um Estrangulador de Loiras"},
        { 2,55, "Janela Indiscreta"},
        { 5, 5, "O Monstro da Lagoa Negra"},
        { 6,45, "Filme Surpresa"},
        { 8,20, ""}, //! deve tar errado
    },
};

float _normalizar_horario(uint8_t hora, uint8_t min) {
    return (hora+1)%24 + min/60.0;
}
float normalizar_horario(struct filme filme) {
    return _normalizar_horario(filme.hora, filme.min);
}

int main() {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Cinema de pijama - O FIM",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    TTF_Init();
    font = TTF_OpenFont("tiny.ttf", FONT_SIZE);
    assert(font != NULL);

    /* EXECUÇÃO */
    uint32_t falta = TIMEOUT;
    for (SDL_Event evt; evt.type != SDL_QUIT; ) {
        if (!AUX_WaitEventTimeout(&evt, &falta, TIMEOUT)) {
            TFX_limpar_tela_cor(ren, BRANCO);

            const float escala = W_WIDTH/10.1;
            const int pad = 3, size = FONT_SIZE*2;
            const int margin_y = (W_HEIGHT - (pad+size)*(LEN(salas)-1))/2;
            const int margin_x = W_WIDTH/18;

            for (uint8_t h = 23; h >= 23 || h <= 9; h = (h+1)%24) {
                for (uint8_t m = 0; m <= 30; m+=30) {
                    char buf[6]; sprintf(buf, "%02u:%02u", h, m);

                    const float t = _normalizar_horario(h, m);
                    const int x = margin_x + t*escala;
                    const int y = margin_y - size;
#define NUM_MEIO
//#define NUM_ANTES
#ifdef NUM_ANTES
                    _TFX_desenhar_texto(ren, buf, x-FONT_SIZE*2.2,y, PRETO);
#elifdef NUM_MEIO
                    _TFX_desenhar_texto(ren, buf, x-20,y, PRETO);
#else
                    _TFX_desenhar_texto(ren, buf, x,y, PRETO);
#endif
                    TFX_mudar_cor(ren, CINZA);
                    SDL_RenderDrawLine(ren, x, y+FONT_SIZE,
                                            x, y+FONT_SIZE+size*(LEN(salas)));
                }
            }
            for (size_t i = 1; i < LEN(salas); i++) {
                const SDL_FRect ini = {
                    .h = size, .y = margin_y + (pad + size)*(i-1),
                };

                char buf[7]; sprintf(buf, "Sala %lu", i);
                _TFX_desenhar_texto(ren, buf, 0, ini.y + FONT_SIZE/2, PRETO);
                for (size_t j = 0; j < LEN(salas[i])-1; j++) {
                    const struct filme filme = salas[i][j];
                    const struct filme prox  = salas[i][j+1];

                    const float t_atual = normalizar_horario(filme);
                    const float t_prox  = normalizar_horario(prox);
                    const float dt = t_prox - t_atual - pad/escala;

                    const SDL_FRect rect = {
                        .x = margin_x + t_atual*escala,
                        .w = dt*escala,
                        .y = ini.y, .h = ini.h,
                    };
                    const SDL_Color cs[] = {
                        AZUL, VERDE, VERMELHO_M, AMARELO_M, CIANO, MAGENTA
                    };
//#define SO_CINZA
#ifdef SO_CINZA
                    const SDL_Color cor = CINZA;
#else
                    const SDL_Color cor = cs[(j^(i*2))%LEN(cs)];
#endif

                    TFX_desenhar_rect_cor_f(ren, rect, cor);
                    TFX_desenhar_texto_cache(ren, filme.nome,
                                             rect.x+pad, rect.y+FONT_SIZE/2,
#ifdef SO_CINZA
                                             BRANCO);
#else
                                             PRETO);
#endif
                }
            }
            SDL_RenderPresent(ren);
        } else {
            switch (evt.type) {
              case SDL_KEYDOWN: switch (evt.key.keysym.sym) {
                  case SDLK_r: break; //! resetar
              } break;
            }
        }
    }

    /* FINALIZACAO */
    TFX_desenhar_texto_cache_free();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
