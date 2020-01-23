#ifndef _SHIP_H_
#define _SHIP_H_

#define SSIZE 48 // size of ship sprite

#include <SDL2/SDL.h>
#include <vec.h>

#include "input.h"

typedef struct {
  SDL_Texture* texture;
  double rotation;
  double rotation_mom;
  Vec pos;
  Vec vel;
} Ship;

Ship Ship_init(Vec pos, SDL_Renderer* ren)
  ;
void Ship_update(Input* input, Ship* ship, float speed, uint win_width, uint win_height)
  ;
void Ship_render(Ship* ship, SDL_Renderer* ren)
  ;
void Ship_destroy(Ship* ship)
  ;

#endif // _SHIP_H_
