#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <zxc.h>

// size of ship sprite
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
  uint size;
  uint duration;
  Vec position;
} Explosion;

typedef struct {
  Explosion arr[32];
  uint size;
} Explosions;


void create_explosion(Explosions* explosions, SDL_Texture* texture, uint size, uint duration, Vec pos)
{
  uint loc = explosions->size;

  if (loc >= sizeof(explosions->arr)/sizeof(explosions->arr[0])) {

    // put new item to first place so it's guaranteed to fit
    Explosion expl = {texture, 0, size, duration, pos};
    explosions->arr[0] = expl;
    uint new_explosions_size = 1;

    // copy the rest in reverse order
    uint current_index = 1;
    for (uint i = sizeof(explosions->arr)/sizeof(explosions->arr[0]) - 1; i > 0; --i) {

      // THIS MIGHT OVERWRITE SOME EXPLOSIONS DATA
      if (explosions->arr[i].texture != NULL) {
        explosions->arr[current_index] = explosions->arr[i];
        new_explosions_size++;
        current_index++;
      }
    }
    explosions->size = new_explosions_size;
  } else {
    Explosion explosion = {texture, 0, size, duration, pos};
    explosions->arr[loc] = explosion;
    explosions->size++;
  }
}

int update_explosion(Explosion* explosion, SDL_Renderer* ren, bool keyframe)
{
  if (explosion->texture == NULL) return 0;

  int x = explosion->tick * explosion->size;
  SDL_Rect src = {x, 0, explosion->size, explosion->size};

  SDL_Rect dest = {
                   .x = explosion->position.x,
                   .y = explosion->position.y,
                   .w = explosion->size,
                   .h = explosion->size
  };

  SDL_RenderCopy(ren, explosion->texture, &src, &dest);

  if (keyframe) {
    if (explosion->tick < explosion->duration ) {
        explosion->tick += 1;
    } else {
        explosion->tick = 0;
    }
  }

  return explosion->duration - explosion->tick;
}

void destroy_explosion(Explosion* explosion)
{
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
  SDL_Texture* explosion_c = zxc_load_texture("images/explosions/type_C.png", ren);

  Explosions explosions;

  Vec epos = {100,100};
  create_explosion(&explosions, explosion_a, 50, 30, epos);
  create_explosion(&explosions, explosion_b, 192, 64, epos);
  create_explosion(&explosions, explosion_c, 256, 48, epos);

  // FPS meter
  uint frame_count = 0;
  uint last_time = SDL_GetTicks();
  float speed = 10;
  uint time;
  bool keyframe = true;

  while (running) {
    ++frame_count;
    time = SDL_GetTicks();

    if (last_time + 1000 <= time) {
      last_time = time;
      speed = ((float) frame_count)/60;
      SDL_Log("FPS: %d speed: %f\n", frame_count, speed);
      frame_count = 0;
    }

    // determine keyframes
    if (frame_count % (int) speed == 0) {
      keyframe = true;
    } else {
      keyframe = false;
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

    for (uint i = 0; i < explosions.size; ++i) {
      if (update_explosion(&explosions.arr[i], ren, keyframe) == 0)
        destroy_explosion(&explosions.arr[i]);
    }

    SDL_RenderCopyEx(ren, ship, &src, &dest, rotation, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(ren);
  }

  return 0;
}
