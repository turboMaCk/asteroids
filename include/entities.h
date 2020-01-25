#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <vec.h>

// ASTEROIDS

#define MAX_ASTEROIDS 64

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

Explosions* init_explosions(SDL_Renderer* ren)
  ;
void destroy_explosions(Explosions* explosions)
  ;
void create_explosion(Explosions* explosions, ExplosionType type, Vec pos)
  ;
void render_explosions(Explosions* explosions, bool keyframe, SDL_Renderer* ren)
  ;
ExplosionType generate_explosion_type()
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

Projectiles* create_projectile(Projectiles* projectiles, Vec pos, Vec vel, double angle)
  ;
Projectiles* update_projectiles(Projectiles* projectiles, uint win_width, uint win_height, float speed)
  ;
void render_projectiles(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
  ;
void destroy_projectiles(Projectiles* projectiles)
  ;

// Colisions

bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec, uint win_width, uint win_height)
  ;
// TODO: better name
Projectiles* colide_asteroids(Asteroids* asteroids, Projectiles* projectiles, Explosions* explosions, uint win_width, uint win_height)
  ;
bool circle_colide_with_asteroids(Asteroids* asteroids, Vec pos, uint r)
  ;

#endif // _ENTITIES_H_
