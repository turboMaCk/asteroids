#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <zxc.h>

const uint win_width = 1200;
const uint win_height = 800;

const double toRad = 0.01745329252;

int main()
{
  SDL_Window* win;
  SDL_Renderer* ren;
  bool running = true;
  double rotation = 0;
  double rotation_mom = 0;
  SDL_Point pos = {
                   .x = (win_width - 48)/2,
                   .y = (win_height -48)/2,
  };

  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("Asteroids",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         win_width,
                         win_height,
                         SDL_WINDOW_SHOWN);

  if (!win) {
    zxc_log_sdl_err("CreateWindow");
    SDL_Quit();
    return 1;
  }

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!ren) {
    zxc_log_sdl_err("CreateRenderer");
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  SDL_Texture* bg = zxc_load_texture("images/background.jpg", ren);

  SDL_Texture* ship = zxc_load_texture("images/spaceship.png", ren);

  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        running = false;
      } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT: {
          if (rotation_mom > -20)
            rotation_mom -= 2;
        } break;
        case SDLK_RIGHT: {
          if (rotation_mom < 20)
            rotation_mom += 2;
        } break;
        case SDLK_UP: {
          pos.x += 10 * sin(rotation * toRad);
          pos.y += -10 * cos(rotation * toRad);
        } break;
        } break;
      } break;
      }
    }

    rotation += rotation_mom;

    if (rotation_mom > 0) {
      rotation_mom -= 0.2;
    } else if (rotation_mom < 0) {
      rotation_mom += 0.2;
    } else {
      rotation_mom = 0;
    }

    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);

    SDL_Rect src = {
                    .x = 38,
                    .y = 38,
                    .w = 38,
                    .h = 38,
    };

    SDL_Rect dest = {
                     .x = pos.x,
                     .y = pos.y,
                     .w = 38,
                     .h = 38,
    };

    SDL_RenderCopyEx(ren, ship, &src, &dest, rotation, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(ren);
  }

  return 0;
}
