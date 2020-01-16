#include "explosions.h"

Explosions* init_explosions(SDL_Renderer* ren)
{
  Explosions* res = malloc(sizeof(Explosions));
  res->size = 0;

  {
    // textures;
    SDL_Texture* tex_a = zxc_load_texture("images/explosions/type_A.png", ren);
    SDL_Texture* tex_b = zxc_load_texture("images/explosions/type_B.png", ren);
    SDL_Texture* tex_c = zxc_load_texture("images/explosions/type_C.png", ren);


    res->texture_a = tex_a;
    res->texture_b = tex_b;
    res->texture_c = tex_c;
  }

  return res;
}

  /* create_explosion(explosions, explosion_a, 50, 30, epos); */
  /* create_explosion(explosions, explosion_b, 192, 64, epos); */
  /* create_explosion(explosions, explosion_c, 256, 48, epos); */

void create_explosion(Explosions* explosions, Vec pos)
{
  // Configure based on texture
  SDL_Texture* texture;
  uint size, duration;

  //TODO: fix
  if (0) {
    size = 50;
    duration = 30;
    texture = explosions->texture_a;
  } else if (2) {
    size = 192;
    duration = 64;
    texture = explosions->texture_b;
  } else {
    size = 192;
    duration = 64;
    texture = explosions->texture_c;
  }

  uint loc = explosions->size;

  if (loc >= sizeof(explosions->arr)/sizeof(explosions->arr[0])) {

    // put new item to first place so it's guaranteed to fit
    Explosion expl = {texture, 0, size, duration, pos};
    explosions->arr[0] = expl;
    uint new_explosions_size = 1;

    // copy the rest in reverse order
    uint current_index = 1;
    for (uint i = MAX_EXPLOSIONS - 1; i > 0; --i) {

      // THIS MIGHT OVERWRITE SOME EXPLOSIONS DATA
      if (explosions->arr[i].texture != NULL) {
        explosions->arr[current_index] = explosions->arr[i];
        new_explosions_size++;
        current_index++;
      }
    }
    explosions->size = new_explosions_size;
  } else {
    Explosion explosion = {texture, 0, size, duration, pos};
    explosions->arr[loc] = explosion;
    explosions->size++;
  }
}

int render_explosion(Explosion* explosion, bool keyframe, SDL_Renderer* ren)
{
  if (explosion->texture == NULL) return 0;

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
    if (explosion->tick < explosion->duration ) {
        explosion->tick += 1;
    } else {
        explosion->tick = 0;
    }
  }

  return explosion->duration - explosion->tick;
}

void destroy_explosion(Explosion* explosion)
{
  explosion->texture = NULL;
}