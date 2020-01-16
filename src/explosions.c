#include "explosions.h"

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

/* NEW Explosion is always created
   If there is not available slot for new explosion
   new explosion is assigned to first available slot
   and rest of the array is reorganized
   from start to end
 */
void create_explosion(Explosions* explosions, Vec pos)
{
  // Configure based on texture
  SDL_Texture* texture;
  uint size, duration;

  //TODO: add enum to pick explosion version
  // small one
  if (0) {
    size = 50;
    duration = 30;
    texture = explosions->texture_a;
  } else if (2) {
    size = 192;
    duration = 64;
    texture = explosions->texture_b;
  } else {
    size = 256;
    duration = 48;
    texture = explosions->texture_c;
  }

  uint loc = explosions->size;
  Explosion explosion = {texture, 0, size, duration, pos, false};

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

void render_explosion(Explosion* explosion, bool keyframe, SDL_Renderer* ren)
{
  if (explosion->destroyed) return;

  int x = explosion->tick * explosion->size;
  SDL_Rect src = {x, 0, explosion->size, explosion->size};

  SDL_Rect dest = {
                   .x = explosion->position.x - explosion->size/2,
                   .y = explosion->position.y - explosion->size/2,
                   .w = explosion->size,
                   .h = explosion->size
  };

  SDL_RenderCopy(ren, explosion->texture, &src, &dest);

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
