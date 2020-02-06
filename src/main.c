#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "window.h"
#include "input.h"
#include "ship.h"
#include "entities.h"
#include "game.h"
#include "countdown.h"
#include "menu.h"

typedef enum {StateMenu, StateGame, StatePause} State;

void run_loop(Game* game,
              FpsCounter* fps,
              Window* win,
              SDL_Renderer* ren)
{
  bool quit = false;
  Countdown countdown = Countdown_init(ren);
  Menu* menu = NULL;

  // game needs to be restarted later.
  bool initialized = false;

  while (!quit) {
    FPSC_update(fps);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // Handle game input
      Window_event_handler(&event, ren, &win->width, &win->height);

      // TODO add controller
      switch (event.type) {
      case SDL_CONTROLLERBUTTONDOWN: {
        switch (event.cbutton.button) {
        case SDL_CONTROLLER_BUTTON_BACK: {
          goto MAIN_QUIT_REQUEST;
        } break;
        case SDL_CONTROLLER_BUTTON_START: {
          goto MAIN_CONTINUE_REQUEST;
        } break;
        }
      } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: {
          goto MAIN_QUIT_REQUEST;
        } break;
        case SDLK_RETURN2:
        case SDLK_RETURN: {
          MAIN_CONTINUE_REQUEST:
          if (game->status == GameEnded)
            Game_restart(game, win->sdl_win);

          game->status = GameNotStarted;
        } break;
        }
      } break;
      case SDL_QUIT: {
        MAIN_QUIT_REQUEST:
        if (game->status == GameNotStarted) {
          if (menu) Menu_destroy(menu);
          menu = Menu_init(ren, game);
          game->status = GamePaused;
        } else {
          quit = true;
        }
      } break;
      }
    }
    switch (game->status) {
    case GameNotStarted: {
      SDL_RenderClear(ren);
      Game_render(game, fps, ren, win->width, win->height);
      if (Countdown_render(&countdown, ren, win->width, win->height)) {
        if (!initialized) {
          Game_restart(game, win->sdl_win);
          initialized = true;
        }
        Game_start(game);
      }
      SDL_RenderPresent(ren);
    } break;
    case GameRunning: {
      Game_loop(game, fps, ren, &win->width, &win->height);
      if (menu) Menu_destroy(menu);
      menu = Menu_init(ren, game);
      // countdown restarted
      Countdown_restart(&countdown);
    } break;
    case GamePaused: {
      SDL_RenderClear(ren);
      Game_render(game, fps, ren, win->width, win->height);
      Menu_render(menu, ren, win->width, win->height);
      SDL_RenderPresent(ren);
    } break;
    case GameEnded: {
      SDL_RenderClear(ren);
      Game_render(game, fps, ren, win->width, win->height);
      Menu_render(menu, ren, win->width, win->height);
      SDL_RenderPresent(ren);
    } break;
    default:
      assert(true);
    }
  }

  if (menu) Menu_destroy(menu);
  Contdown_destroy(&countdown);
}

int main(int argc, char** args)
{
  SDL_WindowFlags win_flag = SDL_WINDOW_RESIZABLE;
  SDL_Window* sdl_win;
  SDL_Renderer* ren;
  Window window;

  for (int i = 0; i < argc; ++i) {
    if (strncmp("-fullscreen", args[i], 11) == 0) win_flag = SDL_WINDOW_FULLSCREEN;
  }

  // seed random
  srand(time(NULL));

  // Init SDL
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

  // Init TTF
  if (TTF_Init() != 0){
    SDL_Log("Error during TTF initialization");
    SDL_Quit();
    return 1;
  }

  sdl_win = SDL_CreateWindow("Asteroids",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         WIN_WIDTH,
                         WIN_HEIGHT,
                         SDL_WINDOW_SHOWN | win_flag);

  if (!sdl_win) {
    SDL_Log("CreateWindow Error: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  window = Window_init(sdl_win);

  // Renderer
  ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

  if (!ren) {
    SDL_Log("Create Renderer error: %s", SDL_GetError());
    SDL_DestroyWindow(sdl_win);
    SDL_Quit();
    return 1;
  }

  FpsCounter* fps = FPSC_init();
  Game* game = Game_init(ren);
  SDL_GameController** controllers = Input_init_controllers();

  run_loop(game, fps, &window, ren);

  // Cleanup
  FPSC_destory(fps);
  Game_destory(game);
  Input_destroy_controllers(controllers);

  // SDL stuff
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(sdl_win);
  SDL_Quit();
  return 0;
}
