#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <zxc.h>

#define SSIZE 48

const uint win_width = 1200;
const uint win_height = 800;

const double toRad = 0.01745329252;

typedef struct {
  double rotation;
  float thrust;
} Input;

int main()
{
  SDL_Window* win;
  SDL_Renderer* ren;
  bool running = true;
  double rotation = 0;
  double rotation_mom = 0;
  SDL_Point pos = { (win_width - SSIZE)/2, (win_height - SSIZE)/2 };
  Input input = { 0, 0 };
  Vec vel = { 0,0 };

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
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT: {
          input.rotation = 0;
        } break;
        case SDLK_RIGHT: {
          input.rotation = 0;
        } break;
        case SDLK_UP: {
          input.thrust = 0;
        } break;
        case SDLK_DOWN: {
          input.thrust = 0;
        } break;
        } break;
      } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT: {
          input.rotation = -1;
        } break;
        case SDLK_RIGHT: {
          input.rotation = 1;
        } break;
        case SDLK_UP: {
          input.thrust = 1;
        } break;
        case SDLK_DOWN: {
          input.thrust = -1;
        } break;
        } break;
      } break;
      } break;
    }

    // ROTATION
    if (input.rotation != 0) {
      rotation_mom += fabs(rotation_mom) < 5 ? input.rotation : 0;
    } else {
      // slowing rotation
      if (fabs(rotation_mom) > 0.2) {
        rotation_mom += rotation_mom > 0 ? -0.2 : 0.2;
      } else {
        rotation_mom = 0;
      }
    }
    rotation += rotation_mom;

    // VELOCITY
    if (input.thrust != 0) {
      Vec thrust_vec = {
                        .x = input.thrust * sin(rotation * toRad),
                        .y = -1 * input.thrust * cos(rotation * toRad),
      };

      vel = vec_add(vel, thrust_vec);
    } else {
      vel = vec_scale(0.95, vel);
    }

    // POSITION
    pos.x += (int) vel.x;
    pos.y += (int) vel.y;

    /* RENDER */

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
