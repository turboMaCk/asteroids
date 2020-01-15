#include <SDL2/SDL.h>
#include <zxc.h>
#include <stdbool.h>

#define MAX_EXPLOSIONS 32

typedef struct {
  SDL_Texture* texture;
  uint tick;
  uint size;
  uint duration;
  Vec position;
} Explosion;

typedef struct {
  Explosion arr[MAX_EXPLOSIONS];
  uint size;
} Explosions;

void create_explosion(Explosions* explosions, SDL_Texture* texture, uint size, uint duration, Vec pos)
{
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
