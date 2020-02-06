#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vec.h>
#include <deg.h>

#include "input.h"
#include "ship.h"

Ship Ship_init(Vec pos, SDL_Renderer* ren)
{
  SDL_Texture* texture = IMG_LoadTexture(ren, "images/spaceship.png");
  double rotation = 0;
  double rotation_mom = 0;
  Vec vel = { 0,0 };

  Ship ship = {texture, rotation, rotation_mom, pos, vel};
  return ship;
}

void Ship_update(Input* input, Ship* ship, float speed, uint win_width, uint win_height)
{
  // ROTATION
  if (input->rotation != 0) {
    ship->rotation_mom += fabs(ship->rotation_mom) < 5 ? input->rotation/speed/4 : 0;
  } else {
    // slowing rotation
    ship->rotation_mom += ship->rotation_mom > 0 ? -0.2/speed : 0.2/speed;
  }
  ship->rotation += ship->rotation_mom/speed;

  float rad = deg_to_radians(ship->rotation);
  // VELOCITY
  if (fabs(input->thrust) > 0) {
    Vec thrust_vec = {
                      .x = (input->thrust * sin(rad)) / speed,
                      .y = (-1 * input->thrust * cos(rad)) / speed,
    };

    ship->vel = vec_limit(10, vec_add(ship->vel, thrust_vec));
  } else {
    ship->vel = vec_scale(1 - (0.01 / speed), ship->vel);
  }

  // POSITION
  ship->pos.x += ship->vel.x/speed;
  ship->pos.y += ship->vel.y/speed;

  if (ship->pos.x > win_width + SSIZE/2) {
    ship->pos.x = -SSIZE;
  } else if (ship->pos.x < -SSIZE) {
    ship->pos.x = win_width;
  }

  if (ship->pos.y > win_height + SSIZE/2) {
    ship->pos.y = -SSIZE/2;
  } else if (ship->pos.y < -SSIZE/2) {
    ship->pos.y = win_height;
  }
}

void Ship_render(const Ship* ship, SDL_Renderer* ren)
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

void Ship_destroy(Ship* ship)
{
  SDL_DestroyTexture(ship->texture);
}
