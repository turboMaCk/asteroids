#include "projectiles.h"

Projectiles* create_projectile(Projectiles* projectiles, Vec pos, double angle)
{
  int id = projectiles == NULL ? 1 : projectiles->head.id + 1;
  Projectile proj = {id, pos, angle};

  Projectiles* new_projectiles = (Projectiles*) malloc(sizeof(Projectiles));

  new_projectiles->head = proj;
  new_projectiles->tail = (struct Projectiles*) projectiles;

  return new_projectiles;
}

Projectiles* colide_asteroids(Asteroids* asteroids, Projectiles* projectiles)
{
  Projectiles* new_head = NULL;
  Projectiles* prev = NULL;

  while (projectiles != NULL) {
    Projectiles* next = (Projectiles*) projectiles->tail;

    // Check if projectile is on screen
    bool did_colide = !projectile_colide_asteroids(asteroids, projectiles->head.pos);

    if (did_colide) {
      if (new_head == NULL) new_head = projectiles;
      if (prev != NULL) prev->tail = (struct Projectiles*) projectiles;

      /* create_explosion(explosions, SDL_Texture *texture, uint size, uint duration, Vec pos) */

      prev = projectiles;
    } else {
      if (next == NULL && prev != NULL) prev->tail = NULL;
      free(projectiles);
    }

    projectiles = next;
  }

  return new_head;
}

Projectiles* update_projectiles(Projectiles* projectiles, uint win_width, uint win_height, float speed)
{
  Projectiles* new_head = NULL;
  Projectiles* prev = NULL;

  while (projectiles != NULL) {
    Projectile* projectile = &projectiles->head;
    Projectiles* next = (Projectiles*) projectiles->tail;

    // calculate new position
    Vec thrust_vec = {
                        .x = (10 * sin(projectile->rotation * toRad)) / speed,
                        .y = (-10 * cos(projectile->rotation * toRad)) / speed,
    };
    projectile->pos = vec_add(projectile->pos, thrust_vec);

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

void render_projectiles(Projectiles* projectiles, SDL_Texture* texture, SDL_Renderer* ren)
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
