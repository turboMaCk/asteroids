#ifndef _SHIP_H_
#define _SHIP_H_

#define SSIZE 48 // size of ship sprite

#include <SDL2/SDL.h>
#include <vec.h>
#include <stdbool.h>

#include "input.h"

typedef struct {
  SDL_Texture* texture;
  SDL_Texture* fire_texture;
  uint tick;
  double rotation;
  double rotation_mom;
  Vec pos;
  Vec vel;
} Ship;

Ship Ship_init(Vec pos, SDL_Renderer* ren)
  ;
void Ship_update(Input* input, Ship* ship, float speed, uint win_width, uint win_height)
  ;
void Ship_render(const Ship* ship, SDL_Renderer* ren)
  ;
void Ship_destroy(Ship* ship)
  ;
void Ship_render_engines(Ship* ship, Input* input, SDL_Renderer* ren, bool keyframe)
  ;

#endif // _SHIP_H_
