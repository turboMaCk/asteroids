#ifndef _ASTEROIDS_H_
#define _ASTEROIDS_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <zxc.h>
#include <math.h>

#define MAX_ASTEROIDS 64

typedef struct {
  SDL_Texture* texture;
  uint radius;
  uint tick;
  uint frames;
  bool destroyed;
  Vec pos;
  Vec vel;
} Asteroid;

typedef struct {
  uint size;
  SDL_Texture* texture;
  Asteroid asteroids[MAX_ASTEROIDS];
} Asteroids;

Asteroids* init_asteroids(SDL_Renderer* ren)
  ;
void destory_asteroids(Asteroids* asteroids)
  ;
void create_asteroid(Asteroids* asteroids, Vec pos)
  ;
void update_asteroids(Asteroids* asteroids, float speed, uint width, uint height)
  ;
void render_asteroids(Asteroids* asteroids, bool keyframe, SDL_Renderer* ren)
  ;
bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec)
  ;
bool circle_colide_with_asteroids(Asteroids* asteroids, Vec pos, uint r)
  ;

#endif
