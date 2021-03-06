#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vec.h>
#include <deg.h>
#include <stdbool.h>

#include "input.h"
#include "ship.h"

Ship Ship_init(Vec pos, SDL_Renderer* ren)
{
  SDL_Texture* texture = IMG_LoadTexture(ren, "images/spaceship.png");
  SDL_Texture* fire_texture = IMG_LoadTexture(ren, "images/fire_red.png");
  double rotation = 0;
  double rotation_mom = 0;
  Vec vel = { 0,0 };

  Ship ship = {texture, fire_texture, 0, rotation, rotation_mom, pos, vel};
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

    int limit = 10;

    // smaller max speed and thrust on reverse
    if (input->thrust < 0) {
      thrust_vec = vec_scale(0.5, thrust_vec);
      limit = 5;
    }

    ship->vel = vec_limit(limit, vec_add(ship->vel, thrust_vec));
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
                    .y = 0,
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

void Ship_render_engines(Ship* ship, Input* input, SDL_Renderer* ren, bool keyframe)
{
  // render only when there is thrust forward
  if (input->thrust <= 0) return;

  // total number of frames
  uint frames = 16;

  uint x = (ship->tick)*32;

  SDL_Rect src = {
                  .x = x,
                  .y = 0,
                  .w = 32,
                  .h = 64,
  };

  // Left engine
  {
    SDL_Rect dest = {
                     .x = ship->pos.x - 28,
                     .y = ship->pos.y + 12,
                     .w = 32,
                     .h = 64,
    };

    SDL_Point center = {28, -12};

    SDL_RenderCopyEx(ren,
                     ship->fire_texture,
                     &src,
                     &dest,
                     ship->rotation,
                     &center,
                     SDL_FLIP_NONE);
  }

  // Right engine
  {
    SDL_Rect dest = {
                     .x = ship->pos.x - 4,
                     .y = ship->pos.y + 12,
                     .w = 32,
                     .h = 64,
    };

    SDL_Point center = {4, -12};

    SDL_RenderCopyEx(ren,
                     ship->fire_texture,
                     &src,
                     &dest,
                     ship->rotation,
                     &center,
                     SDL_FLIP_NONE);
  }

  if (keyframe) {
    ship->tick = ship->tick < frames ? ship->tick + 1 : 0;
  }
}

void Ship_destroy(Ship* ship)
{
  SDL_DestroyTexture(ship->texture);
}
