#include <SDL2/SDL.h>
#include <zxc.h>

#include "input.h"

#define SSIZE 48 // size of ship sprite

typedef struct {
  SDL_Texture* texture;
  double rotation;
  double rotation_mom;
  Vec pos;
  Vec vel;
} Ship;

Ship init_ship(Vec pos, SDL_Renderer* ren)
{
  SDL_Texture* texture = zxc_load_texture("images/spaceship.png", ren);
  double rotation = 0;
  double rotation_mom = 0;
  Vec vel = { 0,0 };

  Ship ship = {texture, rotation, rotation_mom, pos, vel};
  return ship;
}

void update_ship(Input* input, Ship* ship, float speed, uint win_width, uint win_height)
{
  // ROTATION
  if (input->rotation != 0) {
    ship->rotation_mom += fabs(ship->rotation_mom) < 5 ? input->rotation/speed : 0;
  } else {
    // slowing rotation
    ship->rotation_mom += ship->rotation_mom > 0 ? -0.05/speed : 0.05/speed;
  }
  ship->rotation += ship->rotation_mom/speed;

  // VELOCITY
  if (fabs(input->thrust) > 0) {
    Vec thrust_vec = {
                      .x = (input->thrust * sin(ship->rotation * toRad)) / speed,
                      .y = (-1 * input->thrust * cos(ship->rotation * toRad)) / speed,
    };

    ship->vel = vec_limit(20, vec_add(ship->vel, thrust_vec));
  } else {
    ship->vel = vec_scale(1 - (0.01 / speed), ship->vel);
  }

  // POSITION
  ship->pos.x += ship->vel.x/speed;
  ship->pos.y += ship->vel.y/speed;

  if (ship->pos.x > win_width + SSIZE) {
    ship->pos.x = -SSIZE;
  } else if (ship->pos.x < -SSIZE) {
    ship->pos.x = win_width;
  }

  if (ship->pos.y > win_height + SSIZE) {
    ship->pos.y = -SSIZE;
  } else if (ship->pos.y < -SSIZE) {
    ship->pos.y = win_height;
  }
}

void render_ship(Ship* ship, SDL_Renderer* ren)
{
    SDL_Rect src = {
                    .x = 38,
                    .y = 38,
                    .w = 38,
                    .h = 38,
    };

    SDL_Rect dest = {
                     .x = (int) ship->pos.x - 19,
                     .y = (int) ship->pos.y - 19,
                     .w = 38,
                     .h = 38,
    };
    SDL_RenderCopyEx(ren, ship->texture, &src, &dest, ship->rotation, NULL, SDL_FLIP_NONE);
}