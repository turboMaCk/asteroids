#include "entities.h"
#include <SDL_image.h>

Explosions* Explosions_init(SDL_Renderer* ren)
{
  Explosions* res = malloc(sizeof(Explosions));
  res->size = 0;

  // textures;
  res->texture_small = IMG_LoadTexture(ren, "images/explosions/type_A.png");
  res->texture_big = IMG_LoadTexture(ren, "images/explosions/type_B.png");
  res->texture_huge = IMG_LoadTexture(ren, "images/explosions/type_C.png");

  return res;
}

void Explosions_destroy(Explosions* explosions)
{
  SDL_DestroyTexture(explosions->texture_small);
  SDL_DestroyTexture(explosions->texture_big);
  SDL_DestroyTexture(explosions->texture_huge);

  free(explosions);
}

/* NEW Explosion is always created
   If there is not available slot for new explosion
   new explosion is assigned to first available slot
   and rest of the array is reorganized
   from start to end
*/
void Explosions_create(Explosions* explosions, ExplosionType type, Vec pos)
{
  // Configure based on texture
  uint radius, duration;

  switch(type) {
  case ExplosionSmall: {
    radius = 25;
    duration = 30;
  } break;
  case ExplosionBig: {
    radius = 96;
    duration = 64;
  } break;
  case ExplosionHuge: {
    radius = 128;
    duration = 48;
  } break;
  default: {
    fprintf(stderr,
            "Unknown ExplosionType: %d", type);
    exit(-1);
  }
  }

  uint loc = explosions->size;
  Explosion explosion = {
                         .destroyed = false,
                         .type = type,
                         .tick = 0,
                         .radius = radius,
                         .duration = duration,
                         .pos = pos
  };

  // THIS MIGHT OVERWRITE SOME EXPLOSIONS DATA
  if (loc >= MAX_EXPLOSIONS) {
    // put new item to first place so it's guaranteed to fit
    explosions->arr[0] = explosion;
    explosions->size = 1;

    // copy the rest in place
    uint current_index = 1;
    for (uint i = 1; i < MAX_EXPLOSIONS; ++i) {
      if (!explosions->arr[i].destroyed) {
        explosions->arr[current_index] = explosions->arr[i];
        explosions->arr[i].destroyed = true;
        explosions->size++;
        current_index++;
      }
    }

  } else {
    explosions->arr[loc] = explosion;
    explosions->size++;
  }
}

ExplosionType Explosions_generate_type()
{
  ExplosionType explosion_type;
  int rand_val = rand() % 2 + 1;
  switch (rand_val) {
  case 1: {
    explosion_type = ExplosionBig;
  } break;
  case 2: {
    explosion_type = ExplosionHuge;
  } break;
  default: {
    fprintf(stderr, "Default case in explosion generation reached for %d\n", rand_val);
    explosion_type = ExplosionBig;
  }
  }

  return explosion_type;
}

SDL_Texture* Explosions_get_texture(Explosions* explosions, ExplosionType type)
{
  switch (type) {
  case ExplosionSmall: {
    return explosions->texture_small;
  } break;
  case ExplosionBig: {
    return explosions->texture_big;
  } break;
  case ExplosionHuge: {
    return explosions->texture_huge;
  } break;
  default: {
    fprintf(stderr, "Unknown ExplosionType: %d", type);
    exit(-1);
  }
  }
}

void Explosions_render(Explosions* explosions, bool keyframe, SDL_Renderer* ren)
{
  for (uint i = 0; i < explosions->size; ++i) {
    Explosion* explosion = &explosions->arr[i];

    if (!explosion->destroyed) {
      uint d = explosion->radius * 2;
      int x = explosion->tick * d;
      SDL_Rect src = {x, 0, d, d};

      SDL_Rect dest = {
                       .x = explosion->pos.x - explosion->radius,
                       .y = explosion->pos.y - explosion->radius,
                       .w = d,
                       .h = d,
      };

      SDL_RenderCopy(ren, Explosions_get_texture(explosions, explosion->type), &src, &dest);

      if (keyframe) {
        if (explosion->tick < explosion->duration) {
          explosion->tick += 1;
        }
        else if (explosion->duration == explosion->tick) {
          explosion->destroyed=true;
        }
        else {
          explosion->tick = 0;
        }
      }
    }
  }
}

void Explosions_destroy_all(Explosions* explosions)
{
  for (int i = 0; i < MAX_EXPLOSIONS; ++i) {
    explosions->size = 0;
    explosions->arr[i].destroyed = true;
  }
}
