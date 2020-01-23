#ifndef _SHIP_H_
#define _SHIP_H_

#define SSIZE 48 // size of ship sprite

#include <SDL2/SDL.h>
#include <zxc.h>
#include <vec.h>

#include "input.h"

typedef struct {
  SDL_Texture* texture;
  double rotation;
  double rotation_mom;
  Vec pos;
  Vec vel;
} Ship;

Ship init_ship(Vec pos, SDL_Renderer* ren)
  ;
void update_ship(Input* input, Ship* ship, float speed, uint win_width, uint win_height)
  ;
void render_ship(Ship* ship, SDL_Renderer* ren)
  ;
void destroy_ship(Ship* ship)
  ;

#endif // _SHIP_H_
