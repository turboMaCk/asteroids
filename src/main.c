#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <zxc.h>

#define SSIZE 48

const int win_width = 1200;
const int win_height = 800;

const double toRad = 0.01745329252;

typedef struct {
  double rotation;
  float thrust;
} Input;

typedef struct {
  SDL_Texture* texture;
  uint tick;
  Vec position;
} Explosion;

int update_explosion(Explosion* explosion, SDL_Renderer* ren) {
  if (explosion->texture == NULL) return 0;

  int x = (explosion->tick / 100) * 50;
  SDL_Rect src = {x,0, 50, 50};

  SDL_Rect dest = {explosion->position.x, explosion->position.y, 50, 50};

  SDL_RenderCopy(ren, explosion->texture, &src, &dest);

  if (explosion->tick < 2000) {
    explosion->tick += 1;
  } else {
    explosion->tick = 0;
  }

  return 2000 - explosion->tick;
}

void destroy_explosion(Explosion* explosion) {
  explosion->texture = NULL;
}

int main()
{
  SDL_Window* win;
  SDL_Renderer* ren;
  bool running = true;
  double rotation = 0;
  double rotation_mom = 0;
  Vec pos = { (win_width - SSIZE)/2, (win_height - SSIZE)/2 };
  Input input = { 0, 0 };
  Vec vel = { 0,0 };

  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("Asteroids",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         win_width,
                         win_height,
                         SDL_WINDOW_SHOWN);

  if (!win) {
    zxc_log_sdl_err("CreateWindow");
    SDL_Quit();
    return 1;
  }

  // Renderer
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  if (!ren) {
    zxc_log_sdl_err("CreateRenderer");
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  SDL_Texture* bg = zxc_load_texture("images/background.jpg", ren);
  SDL_Texture* ship = zxc_load_texture("images/spaceship.png", ren);

  SDL_Texture* explosion_a = zxc_load_texture("images/explosions/type_A.png", ren);
  SDL_Texture* explosion_b = zxc_load_texture("images/explosions/type_B.png", ren);


  Explosion explosions[32] = {{explosion_a, 0, {100,100}},
                              {explosion_b, 0, {200,50}},
  };

  // FPS meter
  Uint32 frame_count = 0;
  Uint32 last_time = SDL_GetTicks();
  float speed = 100;
  Uint32 time;

  while (running) {
    ++frame_count;
    time = SDL_GetTicks();

    if (last_time + 1000 <= time) {
      last_time = time;
      speed = ((float) frame_count)/60;
      SDL_Log("FPS: %d speed: %f\n", frame_count, speed);
      frame_count = 0;
    }

    SDL_Event event;

    // HANDLE EVENTS
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        running = false;
      } break;
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT: {
          input.rotation = 0;
        } break;
        case SDLK_RIGHT: {
          input.rotation = 0;
        } break;
        case SDLK_UP: {
          input.thrust = 0;
        } break;
        case SDLK_DOWN: {
          input.thrust = 0;
        } break;
        } break;
      } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT: {
          input.rotation = -1;
        } break;
        case SDLK_RIGHT: {
          input.rotation = 1;
        } break;
        case SDLK_UP: {
          input.thrust = 1;
        } break;
        case SDLK_DOWN: {
          input.thrust = -1;
        } break;
        } break;
      } break;
      } break;
    }

    // ROTATION
    if (input.rotation != 0) {
      rotation_mom += fabs(rotation_mom) < 5 ? input.rotation/speed : 0;
    } else {
      // slowing rotation
     rotation_mom += rotation_mom > 0 ? -0.05/speed : 0.05/speed;
    }
    rotation += rotation_mom/speed;

    // VELOCITY
    if (fabs(input.thrust) > 0) {
      Vec thrust_vec = {
                        .x = (input.thrust * sin(rotation * toRad)) / speed,
                        .y = (-1 * input.thrust * cos(rotation * toRad)) / speed,
      };

      vel = vec_limit(20, vec_add(vel, thrust_vec));
    } else {
      vel = vec_scale(1 - (0.01 / speed), vel);
    }

    // POSITION
    pos.x += vel.x/speed;
    pos.y += vel.y/speed;

    if (pos.x > win_width) {
      pos.x = -SSIZE;
    } else if (pos.x < -SSIZE) {
      pos.x = win_width;
    }

    if (pos.y > win_height) {
      pos.y = -SSIZE;
    } else if (pos.y < -SSIZE) {
      pos.y = win_height;
    }

    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);

    SDL_Rect src = {
                    .x = 38,
                    .y = 38,
                    .w = 38,
                    .h = 38,
    };

    SDL_Rect dest = {
                     .x = (int) pos.x,
                     .y = (int) pos.y,
                     .w = 38,
                     .h = 38,
    };

    for (uint i = 0; i < sizeof(explosions)/sizeof(explosions[0]); ++i) {
      if (update_explosion(&explosions[i], ren) == 0) {
        destroy_explosion(&explosions[i]);
      }
    }

    SDL_RenderCopyEx(ren, ship, &src, &dest, rotation, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(ren);
  }

  return 0;
}
