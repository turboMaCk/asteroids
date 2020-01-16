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
  bool destroyed;
} Explosion;

typedef struct {
  uint size;
  SDL_Texture* texture_a;
  SDL_Texture* texture_b;
  SDL_Texture* texture_c;
  Explosion arr[MAX_EXPLOSIONS];
} Explosions;

Explosions* init_explosions(SDL_Renderer* ren)
  ;
void destroy_explosions(Explosions* explosions)
  ;
void create_explosion(Explosions* explosions, Vec pos)
  ;
void render_explosions(Explosions* explosions, bool keyframe, SDL_Renderer* ren)
  ;

#endif // _EXLOSIONS_H_
