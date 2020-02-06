#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <SDL2/SDL.h>

// Default dimensions of window
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

typedef struct {
  int width;
  int height;
  int scale;
  SDL_Window* sdl_win;
} Window;

Window Window_init(SDL_Window* window);

void Window_event_handler(Window* window, SDL_Event* event, SDL_Renderer* ren)
  ;

#endif
