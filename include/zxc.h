#ifndef _ZXC_
#define _ZXC_

#include <SDL2/SDL.h>
#include <SDL_image.h>

void zxc_log_sdl_err(const char* message)
{
  fprintf(stderr, "SDL ERR %s: %s", message, SDL_GetError());
}

SDL_Texture *zxc_load_texture(const char *file, SDL_Renderer* ren)
{
  SDL_Texture* texture = IMG_LoadTexture(ren, file);
  if (!texture) {
    zxc_log_sdl_err("IMG_LoadTexture");
  }

  return texture;
}

void zxc_render_texture(SDL_Texture* texture,
                    SDL_Renderer* ren,
                    int x, int y,
                    int w, int h)
{
  // Setup the destination rectancle
  SDL_Rect dst = {
                  .x = x,
                  .y = y,
                  .w = w,
                  .h = h,
  };

  // Query the texture
  SDL_RenderCopy(ren, texture, NULL, &dst);
}

void zxc_render_texture_fill(SDL_Texture* texture, SDL_Renderer* ren)
{
  SDL_RenderCopy(ren, texture, NULL, NULL);
}

typedef struct {
  double x;
  double y;
} Vec;

Vec vec_add(Vec a, Vec b)
{
  Vec res = { a.x + b.x, a.y + b.y };
  return res;
}

Vec vec_scale(float f, Vec vec)
{
  Vec res = { vec.x * f, vec.y * f };
  return res;
}

float gdc(int a, int b)
{
  float n1 = abs(a);
  float n2 = abs(b);

  while(n1!=n2) {
    if(n1 > n2)
      n1 -= n2;
    else
      n2 -= n1;
  }

  return n1;
}

Vec vec_limit(float f, Vec vec)
{
  float sum = fabs(vec.x) + fabs(vec.y);
  if (sum > f) {
    float k = f / sum;
    Vec res = { vec.x * k, vec.y * k };
    return res;
  } else {
    return vec;
  }
}

const double toRad = 0.01745329252;
#endif
