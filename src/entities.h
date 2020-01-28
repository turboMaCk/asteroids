#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <vec.h>

// ASTEROIDS

#define MAX_ASTEROIDS 3

typedef enum {
              AsteroidSmall,
              AsteroidLarge
} AsteroidType;

typedef struct {
  bool destroyed;
  AsteroidType type;
  uint radius;
  uint tick;
  uint frames;
  Vec pos;
  Vec vel;
} Asteroid;

typedef struct {
  SDL_Texture* texture;
  SDL_Texture* texture_small;
  uint size;
  Asteroid asteroids[MAX_ASTEROIDS];
} Asteroids;

Asteroids* Asteroids_init(SDL_Renderer* ren)
  ;
void Asteroids_destroy(Asteroids* asteroids)
  ;
void Asteroids_create(Asteroids* asteroids, AsteroidType type, Vec pos)
  ;
void Asteroids_update(Asteroids* asteroids, float speed, uint width, uint height)
  ;
void Asteroids_render(Asteroids* asteroids, bool keyframe, SDL_Renderer* ren)
  ;
void Asteroids_create_random(Asteroids* asteroids, uint win_width, uint win_height)
  ;

// Explosions

#define MAX_EXPLOSIONS 32

typedef enum {
              ExplosionSmall,
              ExplosionBig,
              ExplosionHuge,
} ExplosionType;

typedef struct {
  bool destroyed;
  ExplosionType type;
  uint tick;
  uint radius;
  uint duration;
  Vec pos;
} Explosion;

typedef struct {
  uint size;
  SDL_Texture* texture_small;
  SDL_Texture* texture_big;
  SDL_Texture* texture_huge;
  Explosion arr[MAX_EXPLOSIONS];
} Explosions;

Explosions* Explosions_init(SDL_Renderer* ren)
  ;
void Explosions_destroy(Explosions* explosions)
  ;
void Explosions_create(Explosions* explosions, ExplosionType type, Vec pos)
  ;
void Explosions_render(Explosions* explosions, bool keyframe, SDL_Renderer* ren)
  ;
ExplosionType Explosions_generate_type()
  ;

// Projectiles

typedef struct {
  int id;
  Vec pos;
  Vec vel;
  double rotation;
} Projectile;

typedef struct {
  Projectile head;
  struct Projectiles* tail;
  struct Projectiles* before;
} Projectiles;

Projectiles* Projectiles_create(Projectiles* projectiles, Vec pos, Vec vel, double angle)
  ;
Projectiles* Projectiles_update(Projectiles* projectiles, uint win_width, uint win_height, float speed)
  ;
void Projectiles_render(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
  ;
void Projectiles_destroy(Projectiles* projectiles)
  ;

// Colisions

Vec* Collisions_asteroids_point(Asteroids* asteroids,
                                Vec vec,
                                uint win_width,
                                uint win_height)
  ;
Vec* Collisions_asteroids_circle(Asteroids* asteroids, Vec pos, uint r)
  ;
Projectiles* Collisions_projectile_asteroids(Asteroids* asteroids,
                                             Projectiles* projectiles,
                                             Explosions* explosions,
                                             Uint32* score,
                                             uint win_width,
                                             uint win_height)
  ;

#endif // _ENTITIES_H_
