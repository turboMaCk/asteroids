#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <SDL2/SDL.h>

#define WIN_WIDTH 1200;
#define WIN_HEIGHT 800;

void Window_event_handler(SDL_Event* event, SDL_Renderer* ren, int *win_width, int *win_height)
  ;

#endif
