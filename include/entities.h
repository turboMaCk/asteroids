#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <zxc.h>
#include <math.h>

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
  uint size;
  SDL_Texture* texture;
  SDL_Texture* texture_small;
  Asteroid asteroids[MAX_ASTEROIDS];
} Asteroids;

Asteroids* init_asteroids(SDL_Renderer* ren)
  ;
void destory_asteroids(Asteroids* asteroids)
  ;
void create_asteroid(Asteroids* asteroids, AsteroidType type, Vec pos)
  ;
void update_asteroids(Asteroids* asteroids, float speed, uint width, uint height)
  ;
void render_asteroids(Asteroids* asteroids, bool keyframe, SDL_Renderer* ren)
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
  uint size;
  uint duration;
  Vec pos;
} Explosion;

typedef struct {
  uint size;
  SDL_Texture* texture_a;
  SDL_Texture* texture_b;
  SDL_Texture* texture_c;
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

// Projectiles

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
Projectiles* update_projectiles(Projectiles* projectiles, uint win_width, uint win_height, float speed)
  ;
void render_projectiles(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
  ;
void destroy_projectiles(Projectiles* projectiles)
  ;

// Colisions

bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec)
  ;
// TODO: better name
Projectiles* colide_asteroids(Asteroids* asteroids, Projectiles* projectiles, Explosions* explosions)
  ;
bool circle_colide_with_asteroids(Asteroids* asteroids, Vec pos, uint r)
  ;

#endif // _ENTITIES_H_
