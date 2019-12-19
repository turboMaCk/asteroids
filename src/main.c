#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

const uint win_width = 1280;
const uint win_height = 720;

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
    printf("Can't create window");
    SDL_Quit();
    return 1;
  }

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!ren) {
    printf("Can't create renderer");
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

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
    SDL_RenderPresent(ren);
  }

  return 0;
}
