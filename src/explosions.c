#include "entities.h"

Explosions* init_explosions(SDL_Renderer* ren)
{
  Explosions* res = malloc(sizeof(Explosions));
  res->size = 0;

  // textures;
  SDL_Texture* tex_a = zxc_load_texture("images/explosions/type_A.png", ren);
  SDL_Texture* tex_b = zxc_load_texture("images/explosions/type_B.png", ren);
  SDL_Texture* tex_c = zxc_load_texture("images/explosions/type_C.png", ren);

  res->texture_a = tex_a;
  res->texture_b = tex_b;
  res->texture_c = tex_c;

  return res;
}

void destroy_explosions(Explosions* explosions)
{
  SDL_DestroyTexture(explosions->texture_a);
  SDL_DestroyTexture(explosions->texture_b);
  SDL_DestroyTexture(explosions->texture_c);

  free(explosions);
}

/* NEW Explosion is always created
   If there is not available slot for new explosion
   new explosion is assigned to first available slot
   and rest of the array is reorganized
   from start to end
*/
void create_explosion(Explosions* explosions, ExplosionType type, Vec pos)
{
  /* // Configure based on texture */
  /* SDL_Texture* texture; */
  uint size, duration;

  switch(type) {
  case ExplosionSmall: {
    size = 50;
    duration = 30;
  } break;
  case ExplosionBig: {
    size = 192;
    duration = 64;
  } break;
  case ExplosionHuge: {
    size = 256;
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
                         .size = size,
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

SDL_Texture* get_explosion_texture(Explosions* explosions, ExplosionType type)
{
  switch (type) {
  case ExplosionSmall: {
    return explosions->texture_a;
  } break;
  case ExplosionBig: {
    return explosions->texture_b;
  } break;
  case ExplosionHuge: {
    return explosions->texture_c;
  } break;
  default: {
    fprintf(stderr, "Unknown ExplosionType: %d", type);
    exit(-1);
  }
  }
}

void render_explosions(Explosions* explosions, bool keyframe, SDL_Renderer* ren)
{
  for (uint i = 0; i < explosions->size; ++i) {
    Explosion* explosion = &explosions->arr[i];

    if (!explosion->destroyed) {
      int x = explosion->tick * explosion->size;
      SDL_Rect src = {x, 0, explosion->size, explosion->size};

      SDL_Rect dest = {
                       .x = explosion->pos.x - explosion->size/2,
                       .y = explosion->pos.y - explosion->size/2,
                       .w = explosion->size,
                       .h = explosion->size
      };

      SDL_RenderCopy(ren, get_explosion_texture(explosions, explosion->type), &src, &dest);

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
