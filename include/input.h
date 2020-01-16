#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>

typedef struct {
  double rotation;
  float thrust;
} Input;

void handle_keyboard(SDL_Event* event, Input* input);

#endif // _INPUT_H_
