#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
  double rotation;
  float thrust;
  uint count;
  bool fire;
} Input;

void Input_keyboard_handler(SDL_Event* event, Input* input)
  ;
Input Input_init()
  ;
bool Input_is_firing(Input *input)
  ;
SDL_GameController** Input_init_controllers()
  ;
void Input_destroy_controllers(SDL_GameController** controllers)
  ;
void Input_controller_handler(SDL_Event* event, Input* input)
  ;

#endif // _INPUT_H_
