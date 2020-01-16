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
  Asteroid asteroids[MAX_ASTEROIDS];
} Asteroids;

Asteroids* init_asteroids()
  ;
void destory_asteroids(Asteroids* asteroids)
  ;
void create_asteroid(SDL_Texture* texture, Asteroids* asteroids)
  ;
void update_asteroid(Asteroid* asteroid, float speed, uint width, uint height)
  ;
void render_asteroid(Asteroid* asteroid, bool keyframe, SDL_Renderer* ren)
  ;
bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec)
  ;

#endif
