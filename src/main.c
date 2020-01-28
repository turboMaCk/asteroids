#include <stdio.h>
#include <SDL2/SDL.h>

#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "window.h"
#include "input.h"
#include "ship.h"
#include "entities.h"
#include "game.h"
#include "countdown.h"

typedef enum {StateMenu, StateGame, StatePause} State;

void run_loop(Game* game,
              FpsCounter* fps,
              SDL_Window* win,
              SDL_Renderer* ren)
{
  int win_width, win_height;
  SDL_GetWindowSize(win, &win_width, &win_height);

  Game_start(game, win);

  bool quit = false;
  bool playing = false;

  Countdown countdown = Countdown_init(ren);

  while (!quit) {
    FPSC_update(fps);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // Handle game input
      Window_event_handler(&event, ren, &win_width, &win_height);

      switch (event.type) {
      case SDL_QUIT: {
        quit = true;
      } break;
      }
    }

    // render game
    if (playing) {
      Game_loop(game, fps, ren, &win_width, &win_height);
      Countdown_restart(&countdown);
      playing = false;
    } else {
      SDL_RenderClear(ren);
      Game_render(game, fps, ren, win_width, win_height);
      if (Countdown_render(&countdown, ren, win_width, win_height)) {
        playing = true;
      }
      SDL_RenderPresent(ren);
    }
  }

  Contdown_destroy(&countdown);
}

int main()
{
  int win_width = WIN_WIDTH;
  int win_height = WIN_HEIGHT;

  // seed random
  srand(time(NULL));

  SDL_Window* win;
  SDL_Renderer* ren;

  // Init SDL
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

  // Init TTF
  if (TTF_Init() != 0){
    SDL_Log("Error during TTF initialization");
    SDL_Quit();
    return 1;
  }

  win = SDL_CreateWindow("Asteroids",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         win_width,
                         win_height,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!win) {
    SDL_Log("CreateWindow Error: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // update window size
  SDL_GetWindowSize(win, &win_width, &win_height);

  // Renderer
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  if (!ren) {
    SDL_Log("Create Renderer error: %s", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  FpsCounter* fps = FPSC_init();
  Game* game = Game_init(ren);
  SDL_GameController** controllers = Input_init_controllers();

  run_loop(game, fps, win, ren);

  // Cleanup
  FPSC_destory(fps);
  Game_destory(game);
  Input_destroy_controllers(controllers);

  // SDL stuff
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
