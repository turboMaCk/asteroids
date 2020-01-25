#include <SDL2/SDL.h>
#include <stdbool.h>
#include "input.h"

Input Input_init()
{
  Input input = {0,0,false};
  return input;
}

bool Input_is_firing(Input* input) {
  return input->fire;
}

void Input_keyboard_handler(SDL_Event* event, Input* input)
{
  switch (event->type) {

  case SDL_KEYUP: {
    switch (event->key.keysym.sym) {
    case SDLK_a:
    case SDLK_LEFT: {
      input->rotation = 0;
    } break;
    case SDLK_d:
    case SDLK_RIGHT: {
      input->rotation = 0;
    } break;
    case SDLK_w:
    case SDLK_UP: {
      input->thrust = 0;
    } break;
    case SDLK_s:
    case SDLK_DOWN: {
      input->thrust = 0;
    } break;
    case SDLK_SPACE: {
      input->fire = false;
    }
    }
  }; break;

  case SDL_KEYDOWN: {
    switch (event->key.keysym.sym) {
    case SDLK_a:
    case SDLK_LEFT: {
      input->rotation = -1;
    } break;
    case SDLK_d:
    case SDLK_RIGHT: {
      input->rotation = 1;
    } break;
    case SDLK_w:
    case SDLK_UP: {
      input->thrust = 1;
    } break;
    case SDLK_s:
    case SDLK_DOWN: {
      input->thrust = -1;
    } break;
    case SDLK_SPACE: {
      input->fire = true;
    }
    }
  } break;
  }

}
