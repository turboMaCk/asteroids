#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "input.h"
#include "ship.h"
#include "entities.h"

#define WIN_WIDTH 1200;
#define WIN_HEIGHT 800;

// SHIP

int main()
{
  int win_width = WIN_WIDTH;
  int win_height = WIN_HEIGHT;

  // seed random
  srand(time(NULL));

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
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
  Asteroids* asteroids = init_asteroids(ren);

  Vec pos = {200,200};
  create_asteroid(asteroids, AsteroidLarge, pos);

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
    frame_count++;
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
        case SDL_WINDOWEVENT: {
          switch (event.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:  {
            win_width = event.window.data1;
            win_height = event.window.data2;
            // we should use ints here!
            int scale = win_width / WIN_WIDTH;

            // scale can be at least 1
            scale = scale < 1 ? 1 : scale;
            win_height /= scale;
            win_width /= scale;

            if (SDL_RenderSetScale(ren, scale, scale)) {
              zxc_log_sdl_err("SetScale");
            }
          } break;
          };
        } break;
      } break;
      } break;
    }

    // UPDATE
    update_ship(&input, &ship, speed, win_width, win_height);
    update_asteroids(asteroids, speed, win_width, win_height);

    projectiles = update_projectiles(projectiles, win_width, win_height, speed);
    projectiles = colide_asteroids(asteroids, projectiles, explosions, win_width, win_height);

    // Check player & asteroids collision

    if (circle_colide_with_asteroids(asteroids, ship.pos, 24)) {
      printf("Game over\n");
      create_explosion(explosions, ExplosionHuge, ship.pos);
    }

    // RENDER
    SDL_RenderClear(ren);
    zxc_render_texture_fill(bg, ren);

    render_projectiles(projectiles, proj_texture, ren);
    render_asteroids(asteroids, keyframe, ren);
    render_ship(&ship, ren);
    render_explosions(explosions, keyframe, ren);

    SDL_RenderPresent(ren);
  }

  // Cleanup

  // textures
  SDL_DestroyTexture(proj_texture);
  SDL_DestroyTexture(bg);

  // game
  destroy_ship(&ship);
  destroy_explosions(explosions);
  destory_asteroids(asteroids);
  destroy_projectiles(projectiles);

  // SDL stuff
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
