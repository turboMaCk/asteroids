#ifndef _EXPLOSIONS_H_
#define _EXPLOSIONS_H_

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
  SDL_Texture* texture_a;
  SDL_Texture* texture_b;
  SDL_Texture* texture_c;
  uint size;
} Explosions;

Explosions* init_explosions(SDL_Renderer* ren)
  ;
void create_explosion(Explosions* explosions, Vec pos)
  ;
int render_explosion(Explosion* explosion, bool keyframe, SDL_Renderer* ren)
  ;
void destroy_explosion(Explosion* explosion)
  ;

#endif // _EXLOSIONS_H_