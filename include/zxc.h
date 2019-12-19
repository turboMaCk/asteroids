#ifndef _ZXC_
#define _ZXC_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void zxc_log_sdl_err(const char* message) {
  fprintf(stderr, "SDL ERR %s: %s", message, SDL_GetError());
}

SDL_Texture *zxc_load_texture(const char *file, SDL_Renderer *ren) {
  SDL_Texture* texture = IMG_LoadTexture(ren, file);
  if (!texture) {
    zxc_log_sdl_err("IMG_LoadTexture");
  }

  return texture;
}

void zxc_render_texture(SDL_Texture *texture,
                    SDL_Renderer *ren,
                    int x, int y,
                    int w, int h)
{
  // Setup the destination rectancle
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;

  // Query the texture
  SDL_RenderCopy(ren, texture, NULL, &dst);
}

void zxc_render_texture_fill(SDL_Texture *texture, SDL_Renderer *ren)
{
  SDL_RenderCopy(ren, texture, NULL, NULL);
}

#endif
