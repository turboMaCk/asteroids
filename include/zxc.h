#ifndef _ZXC_H_
#define _ZXC_H_

#include <SDL2/SDL.h>
#include <SDL_image.h>

// SDL stuff

void zxc_log_sdl_err(const char* message)
  ;
SDL_Texture *zxc_load_texture(const char *file, SDL_Renderer* ren)
  ;
void zxc_render_texture(SDL_Texture* texture,
                    SDL_Renderer* ren,
                    int x, int y,
                    int w, int h)
  ;
void zxc_render_texture_fill(SDL_Texture* texture, SDL_Renderer* ren)
  ;

// Other

float gdc(int a, int b)
  ;

#define toRad 0.01745329252

#endif // _ZXC_H
