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

// INPUT

typedef struct {
  double rotation;
  float thrust;
} Input;

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

// SHIP

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

  Ship ship = {texture, rotation ,rotation_mom, pos, vel};
  return ship;
}

void update_ship(Input* input, Ship* ship, float speed)
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

  if (ship->pos.x > win_width) {
    ship->pos.x = -SSIZE;
  } else if (ship->pos.x < -SSIZE) {
    ship->pos.x = win_width;
  }

  if (ship->pos.y > win_height) {
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
                     .x = (int) ship->pos.x,
                     .y = (int) ship->pos.y,
                     .w = 38,
                     .h = 38,
    };
    SDL_RenderCopyEx(ren, ship->texture, &src, &dest, ship->rotation, NULL, SDL_FLIP_NONE);
}

// EXPLOSIONS

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

int render_explosion(Explosion* explosion, bool keyframe, SDL_Renderer* ren)
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

// ASTEROIDS

typedef struct {
  SDL_Texture* texture;
  uint radius;
  uint tick;
  uint frames;
  Vec pos;
  Vec vel;
} Asteroid;

Asteroid init_asteroid(SDL_Texture* texture) {
  Vec pos = {200,300};
  Vec vel = {(rand()%10)+1,(rand()%10)+1};

  // TODO: hardcoded radius
  Asteroid asteroid = {texture, 32, 0, 16, pos, vel};
  return asteroid;
}

void update_asteroid(Asteroid* asteroid, float speed)
{
  asteroid->pos = vec_add(asteroid->pos, vec_scale(1/speed, asteroid->vel));

  int min_limit = (int)asteroid->radius*-2;

  if (asteroid->pos.y > win_height) {
    asteroid->pos.y = min_limit;
  } else if (asteroid->pos.y < min_limit) {
    asteroid->pos.y = win_height;
  }

  if (asteroid->pos.x > win_width) {
    asteroid->pos.x = min_limit;
  } else if (asteroid->pos.x < min_limit) {
    asteroid->pos.x = win_width;
  }
}

void render_asteroid(Asteroid* asteroid, bool keyframe, SDL_Renderer* ren) {
  uint size = asteroid->radius*2;
  uint k = 5;
  uint x = (asteroid->tick/k)*size;
  SDL_Rect src = {
                  .x = x,
                  .y = 0,
                  .w = size,
                  .h = size,
  };

  SDL_Rect dest = {
                  .x = asteroid->pos.x,
                  .y = asteroid->pos.y,
                  .w = size,
                  .h = size,
  };

  if (keyframe) {
    asteroid->tick = asteroid->tick/k < asteroid->frames ? asteroid->tick + 1 : 0;
  }

  SDL_RenderCopy(ren, asteroid->texture, &src, &dest);
}

int main()
{
  SDL_Window* win;
  SDL_Renderer* ren;
  bool running = true;
  Input input = { 0, 0 };

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

  // Background
  SDL_Texture* bg = zxc_load_texture("images/background.jpg", ren);

  // Entities
  Vec ship_pos = { (win_width - SSIZE)/2, (win_height - SSIZE)/2 };
  Ship ship = init_ship(ship_pos, ren);
  Explosions explosions;

  // TODO: refactor
  SDL_Texture* explosion_a = zxc_load_texture("images/explosions/type_A.png", ren);
  SDL_Texture* explosion_b = zxc_load_texture("images/explosions/type_B.png", ren);
  SDL_Texture* explosion_c = zxc_load_texture("images/explosions/type_C.png", ren);

  Vec epos = {100,100};
  create_explosion(&explosions, explosion_a, 50, 30, epos);
  create_explosion(&explosions, explosion_b, 192, 64, epos);
  create_explosion(&explosions, explosion_c, 256, 48, epos);

  SDL_Texture* asteroid_texture = zxc_load_texture("images/rock.png", ren);
  Asteroid ast = init_asteroid(asteroid_texture);
  Asteroid ast2 = init_asteroid(asteroid_texture);

  // FPS meter
  uint frame_count = 0;
  uint last_time = SDL_GetTicks();
  float speed = 3;
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
      case SDL_KEYUP:
      case SDL_KEYDOWN: {
        handle_keyboard(&event, &input);
      } break;
      } break;
    }

    // UPDATE
    update_ship(&input, &ship, speed);
    update_asteroid(&ast, speed);
    update_asteroid(&ast2, speed);

    // RENDER
    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);

    for (uint i = 0; i < explosions.size; ++i) {
      if (render_explosion(&explosions.arr[i], keyframe, ren) == 0)
        destroy_explosion(&explosions.arr[i]);
    }

    render_ship(&ship, ren);

    render_asteroid(&ast, keyframe, ren);
    render_asteroid(&ast2, keyframe, ren);

    SDL_RenderPresent(ren);
  }

  return 0;
}
