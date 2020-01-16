#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <zxc.h>
#include <time.h>

#include "input.h"
#include "ship.h"
#include "projectiles.h"
#include "asteroids.h"
#include "explosions.h"

const int win_width = 1200;
const int win_height = 800;

// SHIP

int main()
{
  // seed random
  srand ( time(NULL) );

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
  Explosions* explosions = init_explosions(ren);
  Asteroids* asteroids = init_asteroids();

  // TODO: refactor
  SDL_Texture* asteroid_texture = zxc_load_texture("images/rock.png", ren);

  create_asteroid(asteroid_texture, asteroids);
  create_asteroid(asteroid_texture, asteroids);
  create_asteroid(asteroid_texture, asteroids);
  create_asteroid(asteroid_texture, asteroids);
  create_asteroid(asteroid_texture, asteroids);

  // TODO: refactor
  SDL_Texture* proj_texture = zxc_load_texture("images/spaceship.png", ren);
  Projectiles* projectiles = NULL;

  // FPS meter
  uint frame_count = 0;
  uint last_time = SDL_GetTicks();
  float speed = 3;
  uint time;
  bool keyframe = true;
  uint fps_tick = 0;

  while (running) {
    ++frame_count;
    time = SDL_GetTicks();

    if (last_time + 10 <= time) {
      last_time = time;
      speed = ((float) frame_count)/0.6;

      fps_tick++;
      // LOG FPS
      if (fps_tick == 100) {
        printf("FPS: %d\n", (frame_count * 60));
        fps_tick = 0;
      }

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
        switch (event.key.keysym.sym) {
        case SDLK_SPACE: {
          projectiles = create_projectile(projectiles, ship.pos, ship.rotation);
          } break;
        }
      } break;
      } break;
    }

    // UPDATE
    update_ship(&input, &ship, speed, win_width, win_height);

    for (uint i = 0; i < asteroids->size; ++i) {
      update_asteroid(&asteroids->asteroids[i], speed, win_width, win_height);
    }

    projectiles = update_projectiles(projectiles, win_width, win_height, speed);
    projectiles = colide_asteroids(asteroids, projectiles, explosions);

    // RENDER
    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);

    for (uint i = 0; i < explosions->size; ++i) {
      if (render_explosion(&explosions->arr[i], keyframe, ren) == 0)
        destroy_explosion(&explosions->arr[i]);
    }

    render_projectiles(projectiles, proj_texture, ren);

    render_ship(&ship, ren);

    for (uint i = 0; i < asteroids->size; ++i) {
        render_asteroid(&asteroids->asteroids[i], keyframe, ren);
    }

    SDL_RenderPresent(ren);
  }

  return 0;
}
