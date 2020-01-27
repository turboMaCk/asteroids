#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "countdown.h"

Countdown Countdown_init(SDL_Renderer* ren)
{
  // TODO: this might fail and we should handle that
  TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 164);

  Countdown res;
  res.font = font;
  res.started = SDL_GetTicks();

  // Loop from highest to 0
  for (int i = COUNTDOWN; i > 0; --i) {
    char to_start_str[10];
    snprintf(to_start_str, 10, "%d", i - 1);
    SDL_Color color = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, to_start_str, color);
    res.textures[COUNTDOWN - i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
  }

  return res;
}

void Countdown_restart(Countdown* countdown)
{
  countdown->started = SDL_GetTicks();
}

void Contdown_destroy(Countdown *countdown)
{
  TTF_CloseFont(countdown->font);
  for (int i = 0; i <= COUNTDOWN; ++i) {
    SDL_DestroyTexture(countdown->textures[i]);
  }
}

bool Countdown_render(Countdown *c, SDL_Renderer* ren, int win_width, int win_height)
{
  Uint32 ms = SDL_GetTicks();
  Uint32 runs_for = ms - c->started;
  Uint32 index = runs_for/1000;

  if (index >= COUNTDOWN) return true;

  SDL_Texture* texture = c->textures[index];
  int width;
  int height;

  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  SDL_Rect dest = {win_width/2 - width/2, win_height/2 - height/2, width, height};

  SDL_RenderCopy(ren, texture, NULL, &dest);

  return false;
}
