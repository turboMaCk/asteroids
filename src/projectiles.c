#include "entities.h"

#include <SDL2/SDL.h>
#include <deg.h>
#include <vec.h>
#include <stdbool.h>

Projectiles* Projectiles_create(Projectiles* projectiles, Vec pos, Vec vel, double angle)
{
  int id = projectiles == NULL ? 1 : projectiles->head.id + 1;
  Projectile proj = {id, pos, vel, angle};

  Projectiles* new_projectiles = (Projectiles*) malloc(sizeof(Projectiles));

  new_projectiles->head = proj;
  new_projectiles->tail = (struct Projectiles*) projectiles;

  return new_projectiles;
}

Projectiles* Projectiles_update(Projectiles* projectiles, uint win_width, uint win_height, float speed)
{
  Projectiles* new_head = NULL;
  Projectiles* prev = NULL;

  while (projectiles != NULL) {
    Projectile* projectile = &projectiles->head;
    Projectiles* next = (Projectiles*) projectiles->tail;

    // calculate new position
    Vec velocity = {
                    .x = 15 * sin(deg_to_radians(projectile->rotation)),
                    .y = -15 * cos(deg_to_radians(projectile->rotation)),
    };

    // Add initial velocity
    velocity = vec_add(velocity, projectile->vel);
    velocity = vec_scale(1/speed, velocity);
    projectile->pos = vec_add(projectile->pos, velocity);

    // Check if projectile is on screen
    if (projectile->pos.x > -10 &&
        projectile->pos.x < win_width + 10 &&
        projectile->pos.y > -10 &&
        projectile->pos.y < win_height + 10) {
      if (new_head == NULL) new_head = projectiles;
      if (prev != NULL) prev->tail = (struct Projectiles*) projectiles;

      prev = projectiles;
    } else {
      if (next == NULL && prev != NULL) prev->tail = NULL;
      free(projectiles);
    }

    projectiles = next;
  }

  return new_head;
}

void Projectiles_render(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
{
  while (projectiles != NULL) {
    Projectile projectile = projectiles->head;

    SDL_Rect src = {
                    .x = 3,
                    .y = 130,
                    .w = 5,
                    .h = 11
    };

    SDL_Rect dest = {
                    .x = projectile.pos.x - 2,
                    .y = projectile.pos.y - 5,
                    .w = 5,
                    .h = 11
    };

    SDL_RenderCopyEx(ren, texture, &src, &dest, projectile.rotation, NULL, SDL_FLIP_NONE);

    projectiles = (Projectiles*) projectiles->tail;
  }
}

void Projectiles_destroy(Projectiles* projectiles)
{
  while (projectiles) {
    Projectiles* next = (Projectiles *) projectiles->tail;
    free(projectiles);
    projectiles = next;
  }
}
