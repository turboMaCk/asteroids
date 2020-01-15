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

Asteroids* init_asteroids() {
  Asteroids* asteroids = malloc(sizeof(Asteroids));
  asteroids->size = 0;

  return asteroids;
}

void destory_asteroyds(Asteroids* asteroids) {
  free(asteroids);
}

void create_asteroid(SDL_Texture* texture, Asteroids* asteroids) {
  Vec pos = {200,300};
  int vx = (rand()%10) + 1;
  int vy = (rand()%10) + 1;
  Vec vel = {vx,vy};

  // TODO: hardcoded radius
  Asteroid asteroid = {texture, 32, 0, 16, false, pos, vel};

  if (asteroids->size < MAX_ASTEROIDS) {
    asteroids->asteroids[asteroids->size++] = asteroid;
  } else {
    // TODO: clearing code
  }
}

void update_asteroid(Asteroid* asteroid, float speed, uint width, uint height)
{
  // skip if asteroid is destroyed
  if (asteroid->destroyed) return;

  asteroid->pos = vec_add(asteroid->pos, vec_scale(1/speed, asteroid->vel));

  int min_limit = (int)asteroid->radius*-2;

  if (asteroid->pos.y > height) {
    asteroid->pos.y = min_limit;
  } else if (asteroid->pos.y < min_limit) {
    asteroid->pos.y = height;
  }

  if (asteroid->pos.x > width) {
    asteroid->pos.x = min_limit;
  } else if (asteroid->pos.x < min_limit) {
    asteroid->pos.x = width;
  }
}

void render_asteroid(Asteroid* asteroid, bool keyframe, SDL_Renderer* ren) {
  if (asteroid->destroyed == true) return;
  uint size = asteroid->radius*2;
  uint k = 5;
  uint x = (asteroid->tick/k)*size;
  SDL_Rect src = {
                  .x = x,
                  .y = 0,
                  .w = size,
                  .h = size,
  };

  SDL_Rect dest = {
                  .x = asteroid->pos.x,
                  .y = asteroid->pos.y,
                  .w = size,
                  .h = size,
  };

  if (keyframe) {
    asteroid->tick = asteroid->tick/k < asteroid->frames ? asteroid->tick + 1 : 0;
  }

  SDL_RenderCopy(ren, asteroid->texture, &src, &dest);
}


bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec)
{
  Asteroid *asteroid = asteroids->asteroids;

  for (uint i = 0; i < asteroids->size; i++) {
    // skip destroyed
    if (asteroid->destroyed != true) {
      Vec apos = asteroid->pos;
      uint r = asteroid->radius;

      // test collision
      if ((powf(apos.x - vec.x, 2) + powf(apos.y - vec.y, 2)) <= r*r ) {
        asteroid->destroyed = true;
        return true;
      }
    }

    // next asteroid
    asteroid++;
  }

  return false;
}
