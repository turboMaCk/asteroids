#include <SDL2/SDL.h>
#include <zxc.h>
#include <stdbool.h>

typedef struct {
  int id;
  Vec pos;
  double rotation;
} Projectile;

typedef struct {
  Projectile head;
  struct Projectiles* tail;
} Projectiles;

Projectiles* create_projectile(Projectiles* projectiles, Vec pos, double angle) {
  int id = projectiles == NULL ? 1 : projectiles->head.id + 1;
  Projectile proj = {id, pos, angle};

  Projectiles* new_projectiles = (Projectiles*) malloc(sizeof(Projectiles));

  new_projectiles->head = proj;
  new_projectiles->tail = (struct Projectiles*) projectiles;

  return new_projectiles;
}

Projectiles* update_projectiles(Projectiles* projectiles, uint win_width, uint win_height, float speed)
{
  Projectiles *result = NULL, **curr = &result;

  while (projectiles != NULL) {
    Projectile projectile = projectiles->head;
    Vec thrust_vec = {
                        .x = (15 * sin(projectile.rotation * toRad)) / speed,
                        .y = (-15 * cos(projectile.rotation * toRad)) / speed,
    };

    bool doClean = false;
    if (projectile.pos.x > -10 &&
        projectile.pos.x < win_width &&
        projectile.pos.y > -10 &&
        projectile.pos.y < win_height) {
      *curr = (Projectiles*) malloc(sizeof(Projectiles));
      (*curr)->head = projectile;
      (*curr)->head.pos = vec_add(projectile.pos, thrust_vec);
      (*curr)->tail = NULL;
      curr = (Projectiles**) &((*curr)->tail);
    } else {
      doClean = true;
    }

    projectiles = (Projectiles*) projectiles->tail;

    if (doClean) {
      free(projectiles);
    }
  }

  return result;
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
                    .x = projectile.pos.x,
                    .y = projectile.pos.y,
                    .w = 5,
                    .h = 11
    };

    SDL_RenderCopyEx(ren, texture, &src, &dest, projectile.rotation, NULL, SDL_FLIP_NONE);

    projectiles = (Projectiles*) projectiles->tail;
  }
}
