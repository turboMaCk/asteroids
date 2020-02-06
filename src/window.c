#include <SDL2/SDL.h>
#include "window.h"

Window Window_init(SDL_Window* win)
{
  int width, height;
  SDL_GetWindowSize(win, &width, &height);

  Window res = {width, height, width/WIN_WIDTH, win};
  return res;
}

// TODO improve
void Window_event_handler(SDL_Event* event, SDL_Renderer* ren, int *win_width, int *win_height)
{
  // Event type must be window
  if (event->type != SDL_WINDOWEVENT) return;

  if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
    *win_width = event->window.data1;
    *win_height = event->window.data2;
    // we should use ints here!
    int scale = *win_width / 1200;

    // scale can be at least 1
    scale = scale < 1 ? 1 : scale;
    *win_height /= scale;
    *win_width /= scale;

    // Error when scaling
    if (SDL_RenderSetScale(ren, scale, scale)) {
      SDL_Log("SetScale Error %s", SDL_GetError());
    }
  }
}
