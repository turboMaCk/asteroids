#include "entities.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include <vec.h>

Asteroids* init_asteroids(SDL_Renderer* ren)
{
  SDL_Texture* texture_small = IMG_LoadTexture(ren, "images/rock_small.png");
  SDL_Texture* texture = IMG_LoadTexture(ren, "images/rock.png");
  Asteroids* asteroids = malloc(sizeof(Asteroids));
  asteroids->size = 0;
  asteroids->texture = texture;
  asteroids->texture_small = texture_small;

  return asteroids;
}

void destory_asteroids(Asteroids* asteroids)
{
  SDL_DestroyTexture(asteroids->texture);
  SDL_DestroyTexture(asteroids->texture_small);

  free(asteroids);
}

/* NEW Asteroids migght not be created
   if there is no empty slot for it
*/
void create_asteroid(Asteroids* asteroids, AsteroidType type, Vec pos)
{
  int vx = (rand()%20) - 9;
  int vy = (rand()%20) - 9;
  Vec vel = {vx,vy};
  // TODO: radius is same for small and big
  uint radius = 32;

  uint loc = asteroids->size;
  Asteroid asteroid = {
                       .destroyed = false,
                       .type = type,
                       .radius = radius,
                       .tick = 0,
                       .frames = 16,
                       .pos = pos,
                       .vel = vel
  };

  // THIS MIGHT OVERWRITE SOME EXPLOSIONS DATA
  if (asteroids->size >= MAX_ASTEROIDS) {
    // reaorganize the array
    uint current_index = 0;
    for (uint i = 0; i < MAX_ASTEROIDS; ++i) {
      if (!asteroids->asteroids[i].destroyed) {
        if (i != current_index) {
          asteroids->asteroids[current_index] = asteroids->asteroids[i];
          asteroids->asteroids[i].destroyed = true;
        }

        current_index++;
      }
    }

    // Add new if there is a slot
    if (current_index + 1 < MAX_ASTEROIDS) {
      asteroids->asteroids[current_index] = asteroid;
      current_index++;
    }

    asteroids->size = current_index;
  } else {
    asteroids->asteroids[loc] = asteroid;
    asteroids->size++;
  }
}

void create_random_asteroid(Asteroids* asteroids, uint win_width, uint win_height)
{
  int x = rand() % win_width + 1;
  uint y = rand() % win_height + 1;

  if (x / win_width <= y / win_height) {
    x = 0;
  } else {
    y = 0;
  }

  Vec pos = {x,y};

  create_asteroid(asteroids, AsteroidLarge, pos);
}

void update_asteroids(Asteroids* asteroids, float speed, uint width, uint height)
{
  for (uint i = 0; i < asteroids->size; ++i) {
    Asteroid* asteroid = &asteroids->asteroids[i];
    // skip if asteroid is destroyed
    if (!asteroid->destroyed) {
      asteroid->pos = vec_add(asteroid->pos, vec_scale(1/speed, asteroid->vel));

      int min_limit = - (int) asteroid->radius;

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
  }
}


SDL_Texture* get_asteroid_texture(Asteroids* asteroids, AsteroidType type)
{
  SDL_Texture *texture;

  switch (type) {
  case AsteroidLarge: {
    texture = asteroids->texture;
  } break;
  case AsteroidSmall: {
    texture = asteroids->texture_small;
  } break;
  default:
    fprintf(stderr, "Unknown steroid type %d", type);
    exit(-1);
  }

  return texture;
}

void render_asteroids(Asteroids* asteroids, bool keyframe, SDL_Renderer* ren)
{
  for (uint i = 0; i < asteroids->size; ++i) {
    Asteroid* asteroid = &asteroids->asteroids[i];

    if (!asteroid->destroyed) {
      uint size = asteroid->radius*2;
      SDL_Rect src;
      uint k = 5;

      uint x = (asteroid->tick/k)*size;
      src = (SDL_Rect) {
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

      SDL_RenderCopy(ren, get_asteroid_texture(asteroids, asteroid->type), &src, &dest);
    }
  }
}

bool projectile_colide_asteroids(Asteroids* asteroids, Vec vec, uint win_width, uint win_height)
{
  Asteroid* asteroid = asteroids->asteroids;
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

        // create new asteroids
        if (asteroid->type == AsteroidLarge) {
          int num = rand() % 3 + 1;

          // Small pieces from large asteroid
          while (num) {
            create_asteroid(asteroids, AsteroidSmall, asteroid->pos);
            num--;
          }

          // new large asteroid
          create_random_asteroid(asteroids, win_width, win_height);
        }
        break;
      }
    }

    // next asteroid
    asteroid++;
  }

  return res;
}

bool circle_colide_with_asteroids(Asteroids* asteroids, Vec pos, uint r)
{
  Asteroid* asteroid = asteroids->asteroids;
  for (uint i = 0; i < asteroids->size; i++) {
    if (!asteroid->destroyed) {
      uint dx = asteroid->pos.x - pos.x;
      uint dy = asteroid->pos.y - pos.y;

      double distance = sqrt((double) (dx * dx + dy * dy));

      if (distance < r + asteroid->radius) {
        asteroid->destroyed = true;
        return true;
      }
    }
    asteroid++;
  }

  return false;
}
