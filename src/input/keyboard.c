#include <SDL2/SDL.h>
#include "../input.h"

void handle_keyboard(SDL_Event* event, Input* input)
{
  switch (event->type) {
  case SDL_KEYUP: {
    switch (event->key.keysym.sym) {
    case SDLK_LEFT: {
      input->rotation = 0;
    } break;
    case SDLK_RIGHT: {
      input->rotation = 0;
    } break;
    case SDLK_UP: {
      input->thrust = 0;
    } break;
    case SDLK_DOWN: {
      input->thrust = 0;
    } break;
    } break;
    case SDL_KEYDOWN: {
      switch (event->key.keysym.sym) {
      case SDLK_LEFT: {
        input->rotation = -1;
      } break;
      case SDLK_RIGHT: {
        input->rotation = 1;
      } break;
      case SDLK_UP: {
        input->thrust = 1;
      } break;
      case SDLK_DOWN: {
        input->thrust = -1;
      } break;
      } break;
    } break;
    }; break;
  };
}
