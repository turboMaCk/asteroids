#include <stdio.h>
#include <SDL2/SDL.h>

#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "input.h"
#include "ship.h"
#include "entities.h"

#include "game.h"

#define WIN_WIDTH 1200;
#define WIN_HEIGHT 800;

void run_loop(Game* game, FpsCounter* fps, SDL_Window* win, SDL_Renderer* ren)
{
  int win_width, win_height;
  Input input = { 0, 0 };
  Game_start(game, win);
  SDL_GetWindowSize(win, &win_width, &win_height);

  while (game->running) {
    FPSC_update(fps);

    // HANDLE EVENTS
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        game->running = false;
      } break;
      case SDL_KEYUP:
      case SDL_KEYDOWN: {
        handle_keyboard(&event, &input);
        switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
          game->projectiles = create_projectile(game->projectiles,
                                                game->ship.pos,
                                                game->ship.vel,
                                                game->ship.rotation);
          } break;
        }
        case SDL_WINDOWEVENT: {
          switch (event.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:  {
            win_width = event.window.data1;
            win_height = event.window.data2;
            // we should use ints here!
            int scale = win_width / WIN_WIDTH;

            // scale can be at least 1
            scale = scale < 1 ? 1 : scale;
            win_height /= scale;
            win_width /= scale;

            if (SDL_RenderSetScale(ren, scale, scale)) {
              zxc_log_sdl_err("SetScale");
            }
          } break;
          };
        } break;
      } break;
      } break;
    }

    Game_update(game, fps, input, win_width, win_height);
    Game_render(game, fps, ren);
  }
}

int main()
{
  int win_width = WIN_WIDTH;
  int win_height = WIN_HEIGHT;

  // seed random
  srand(time(NULL));

  SDL_Window* win;
  SDL_Renderer* ren;

  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("Asteroids",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         win_width,
                         win_height,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!win) {
    zxc_log_sdl_err("CreateWindow");
    SDL_Quit();
    return 1;
  }

  // update window size
  SDL_GetWindowSize(win, &win_width, &win_height);

  // Renderer
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  if (!ren) {
    zxc_log_sdl_err("CreateRenderer");
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  FpsCounter* fps = FPSC_init();
  Game* game = Game_init(ren);

  run_loop(game, fps, win, ren);

  // Cleanup
  FPSC_destory(fps);
  Game_destory(game);

  // SDL stuff
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
