#include <SDL2/SDL.h>
#include "background.h"

void Background_putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = pixel;
    break;

  case 2:
    *(Uint16 *)p = pixel;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;

  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

SDL_Texture* Background_generate(SDL_Renderer* ren, int width, int height)
{
  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

  if (!surface) {
    fprintf(stderr, "SDL surface creating error %s", SDL_GetError());
    exit(1);
  }

  for (int i = 0; i < (width * height) / 1000; i++) {
    int ran = random() % 0xcf;
    int x = random() % width;
    int y = random() % height;
    int r = ran + (random() % 48);
    int g = ran + (random() % 48);
    int b = ran + (random() % 48);

    Background_putpixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);

  return texture;
}
