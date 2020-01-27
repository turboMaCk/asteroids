#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL_ttf.h>

#define COUNTDOWN 4

typedef struct {
  Uint32 started;
  TTF_Font* font;
  struct SDL_Texture *textures[COUNTDOWN];
} Countdown;

Countdown Countdown_init(SDL_Renderer* ren)
  ;
void Countdown_restart(Countdown* countdown)
  ;
void Contdown_destroy(Countdown *countdown)
  ;
bool Countdown_render(Countdown *c, SDL_Renderer* ren, int win_width, int win_height)
  ;

#endif // _COUNTDOWN_H_
