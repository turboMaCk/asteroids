#include <SDL2/SDL.h>
#include <stdbool.h>
#include "input.h"

Input Input_init()
{
  Input input = {0,0,0,false};
  return input;
}

bool Input_is_firing(Input* input) {
  bool res;
  if (input->fire && input->count >= 10) {
    input->count = 0;
    res = true;
  } else {
    input->count++;
    res = false;
  }

  return res;
}

void Input_keyboard_handler(SDL_Event* event, Input* input)
{
  switch (event->type) {

    // key pressed
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
      input->count++;
    }
    }
  } break;

    // key pulled
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
      input->count = 0;
    }
    }
  }; break;

  }
}

// Controllers

bool Input_init_controllers()
{
  SDL_GameController *controller = NULL;

  bool has_controller = false;
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i)) {
      char *mapping;
      SDL_Log("Index \'%i\' is a compatible controller, named \'%s\'", i, SDL_GameControllerNameForIndex(i));
      controller = SDL_GameControllerOpen(i);
      mapping = SDL_GameControllerMapping(controller);
      SDL_Log("Controller %i is mapped as \"%s\".", i, mapping);
      SDL_free(mapping);
      has_controller = true;
    } else {
      SDL_Log("Index \'%i\' is not a compatible controller.", i);
    }
  }

  return has_controller;
}

void Input_controller_handler(SDL_Event* event, Input* input)
{
  switch (event->type) {

  case SDL_CONTROLLERAXISMOTION: {
    // thrust
    if (event->caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
      input->thrust = ((float) (event->caxis.value / -3000))/10;
    }

    // rotation
    if (event->caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
      input->rotation = ((float) (event->caxis.value / 3000)) / 10;
    }

    printf("Thrust is %f, Rotation is %f\n", input->thrust, input->rotation);
  } break;


  case SDL_CONTROLLERBUTTONDOWN: {
    switch (event->cbutton.button) {

      // firing
    case SDL_CONTROLLER_BUTTON_A: {
      input->fire = true;
      input->count++;
    } break;

      // thrust
    case SDL_CONTROLLER_BUTTON_DPAD_UP: {
      input->thrust = 1;
    } break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN: {
      input->thrust = -1;
    } break;

      // rotation
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: {
      input->rotation = -1;
    } break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: {
      input->rotation = 1;
    } break;
    }
  } break;
  case SDL_CONTROLLERBUTTONUP: {
    switch (event->cbutton.button) {

      // firing
    case SDL_CONTROLLER_BUTTON_A: {
      input->fire = false;
      input->count = 0;
    } break;

      // thrust
    case SDL_CONTROLLER_BUTTON_DPAD_UP: {
      input->thrust = 0;
    } break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN: {
      input->thrust = 0;
    } break;

      // rotation
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: {
      input->rotation = 0;
    } break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: {
      input->rotation = 0;
    } break;
    }
  } break;
  }
}
