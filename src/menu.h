#ifndef _MENU_H_
#define _MENU_H_

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "window.h"
// This sort of crates a cyclic dependecy (but not between header but between c files)
#include "game.h"

typedef struct {
  SDL_Texture* title;
  SDL_Texture* score;
  SDL_Texture** instructions;
} Menu;

Menu* Menu_init(SDL_Renderer *ren, Game*)
  ;
void Menu_render(Menu* menu, SDL_Renderer* ren, Window* window)
  ;
void Menu_destroy(Menu* menu)
  ;
#endif
