#include <stdbool.h>
#include "entities.h"

bool Collisions_asteroids_point(Asteroids* asteroids, Vec vec, uint win_width, uint win_height)
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
            Asteroids_create(asteroids, AsteroidSmall, asteroid->pos);
            num--;
          }

          // new large asteroid
          Asteroids_create_random(asteroids, win_width, win_height);
        }
        break;
      }
    }

    // next asteroid
    asteroid++;
  }

  return res;
}

bool Collisions_asteroids_circle(Asteroids* asteroids, Vec pos, uint r)
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

Projectiles* Collisions_projectile_asteroids(Asteroids* asteroids,
                                             Projectiles* projectiles,
                                             Explosions* explosions,
                                             uint win_width,
                                             uint win_height)
{
  Projectiles* new_head = NULL;
  Projectiles* prev = NULL;

  while (projectiles != NULL) {
    Projectiles* next = (Projectiles*) projectiles->tail;

    // Check if projectile is on screen
    Vec position = projectiles->head.pos;

    if (!Collisions_asteroids_point(asteroids, position, win_width, win_height)) {
      if (new_head == NULL) new_head = projectiles;
      if (prev != NULL) prev->tail = (struct Projectiles*) projectiles;

      prev = projectiles;
    } else {
      if (next == NULL && prev != NULL) prev->tail = NULL;
      free(projectiles);
      Explosions_create(explosions, Explosions_generate_type(), position);
    }

    projectiles = next;
  }

  return new_head;
}
