#include <stdbool.h>
#include "entities.h"

Vec* Collisions_asteroids_point(Asteroids* asteroids, Vec vec, uint win_width, uint win_height)
{
  Asteroid* asteroid = asteroids->asteroids;
  Vec* res = NULL;

  for (uint i = 0; i < asteroids->size; i++) {
    // skip destroyed
    if (!asteroid->destroyed) {
      Vec apos = asteroid->pos;
      uint r = asteroid->radius;

      // test collision
      if ((powf(apos.x - vec.x, 2) + powf(apos.y - vec.y, 2)) <= r*r ) {
        asteroid->destroyed = true;

        // We need to alocate new position because subsequent `Asteroids_crate`
        // Call might overwrite the asteroids data with new which would cause
        // pointer to point to incorrect value.
        // This new memory must be freed after usage.
        // TODO: think about nicer / easier to use way to solve this.
        res = malloc(sizeof(Vec));
        *res = asteroid->pos;

        // create new asteroids
        if (asteroid->type == AsteroidLarge) {
          int num = rand() % 3 + 1;

          // Small pieces from large asteroid
          while (num) {
            // TODO: this can potentially destoroy the `res` pointer???
            Asteroids_create(asteroids, AsteroidSmall, asteroid->pos);
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

Vec* Collisions_asteroids_circle(Asteroids* asteroids, Vec pos, uint r)
{
  Asteroid* asteroid = asteroids->asteroids;
  Vec* res = NULL;

  for (uint i = 0; i < asteroids->size; i++) {
    if (!asteroid->destroyed) {
      uint dx = asteroid->pos.x - pos.x;
      uint dy = asteroid->pos.y - pos.y;

      double distance = sqrt((double) (dx * dx + dy * dy));

      if (distance < r + asteroid->radius) {
        asteroid->destroyed = true;
        // TODO: probably in this case we don't need to do the same thing
        // as in `Collisions_asteroids_point`
        // but only because this code is used for collision with ship
        // this is not super robust design but should work.
        res = &asteroid->pos;
        break;
      }
    }
    asteroid++;
  }

  return res;
}

Projectiles* Collisions_projectile_asteroids(Asteroids* asteroids,
                                             Projectiles* projectiles,
                                             Explosions* explosions,
                                             Uint32* score,
                                             uint win_width,
                                             uint win_height)
{
  Projectiles* new_head = NULL;
  Projectiles* prev = NULL;

  while (projectiles != NULL) {
    Projectiles* next = (Projectiles*) projectiles->tail;

    // Check if projectile is on screen
    Vec* collision_point = Collisions_asteroids_point(asteroids, projectiles->head.pos, win_width, win_height);
    if (!collision_point) {
      if (new_head == NULL) new_head = projectiles;
      if (prev != NULL) prev->tail = (struct Projectiles*) projectiles;

      prev = projectiles;
    } else {
      if (next == NULL && prev != NULL) prev->tail = NULL;
      free(projectiles);
      Explosions_create(explosions, Explosions_generate_type(), *collision_point);
      // Here we're freeing the position copy.
      // see comment in `Collisions_asteroids_point` for details
      free(collision_point);

      // increase score
      ++*score;
    }

    projectiles = next;
  }

  return new_head;
}
