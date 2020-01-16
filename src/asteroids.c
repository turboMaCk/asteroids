#include "asteroids.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <zxc.h>
#include <math.h>

Asteroids* init_asteroids(SDL_Renderer* ren)
{
  SDL_Texture* texture = zxc_load_texture("images/rock.png", ren);
  Asteroids* asteroids = malloc(sizeof(Asteroids));
  asteroids->size = 0;
  asteroids->texture = texture;

  return asteroids;
}

void destory_asteroids(Asteroids* asteroids)
{
  free(asteroids);
}

void create_asteroid(Asteroids* asteroids, Vec pos)
{
  int vx = (rand()%10) + 1;
  int vy = (rand()%10) + 1;
  Vec vel = {vx,vy};

  uint loc = asteroids->size;
  // TODO: hardcoded radius
  Asteroid asteroid = {asteroids->texture, 32, 0, 16, false, pos, vel};

  // THIS MIGHT OVERWRITE SOME EXPLOSIONS DATA
  if (asteroids->size >= MAX_ASTEROIDS) {
    //put new item to first place
    asteroids->asteroids[0] = asteroid;
    asteroids->size = 1;

    // copy the rest
    uint current_index = 1;
    for (uint i = 1; i < MAX_ASTEROIDS; ++i) {
      if (!asteroids->asteroids[i].destroyed) {
        asteroids->asteroids[current_index] = asteroids->asteroids[i];
        asteroids->asteroids[i].destroyed = true;
        asteroids->size++;
        current_index++;
      }
    }
  } else {
    asteroids->asteroids[loc] = asteroid;
    asteroids->size++;
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

void render_asteroid(Asteroid* asteroid, bool keyframe, SDL_Renderer* ren)
{
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
  bool res = false;

  for (uint i = 0; i < asteroids->size; i++) {
    // skip destroyed
    if (!asteroid->destroyed) {
      Vec apos = asteroid->pos;
      uint r = asteroid->radius;

      // test collision
      if ((powf(apos.x - vec.x, 2) + powf(apos.y - vec.y, 2)) <= r*r ) {
        asteroid->destroyed = true;
        res = true;
        break;
      }
    }

    // next asteroid
    asteroid++;
  }

  return res;
}
