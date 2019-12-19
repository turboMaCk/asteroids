#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <zxc.h>

const uint win_width = 1200;
const uint win_height = 800;

int main() {
  SDL_Window* win;
  SDL_Renderer* ren;
  bool running = true;

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

  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        running = false;
      } break;
      }
    }

    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);
    SDL_RenderPresent(ren);
  }

  return 0;
}
