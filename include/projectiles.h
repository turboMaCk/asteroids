#ifndef _PROJECTILES_H_
#define _PROJECTILES_H_

#include <SDL2/SDL.h>
#include <zxc.h>
#include <stdbool.h>
#include "asteroids.h"
#include "explosions.h"

typedef struct {
  int id;
  Vec pos;
  double rotation;
} Projectile;

typedef struct {
  Projectile head;
  struct Projectiles* tail;
  struct Projectiles* before;
} Projectiles;

Projectiles* create_projectile(Projectiles* projectiles, Vec pos, double angle)
  ;
Projectiles* colide_asteroids(Asteroids* asteroids, Projectiles* projectiles, Explosions* explosions)
  ;
Projectiles* update_projectiles(Projectiles* projectiles, uint win_width, uint win_height, float speed)
  ;
void render_projectiles(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
  ;

#endif // _PROJECTILES_H_
