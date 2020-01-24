#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include <zxc.h>
#include "ship.h"
#include "entities.h"
#include "input.h"
#include "game.h"

FpsCounter* FPSC_init()
{
  FpsCounter* counter = malloc(sizeof(FpsCounter));

  counter->frame_count = 0;
  counter->last_time = SDL_GetTicks();
  counter->speed = 3;
  counter->time = 0;
  counter->fps_tick = 0;
  counter->keyframe = true;

  return counter;
}

void FPSC_destory(FpsCounter* counter)
{
  free(counter);
}

void FPSC_update(FpsCounter* c)
{
  c->frame_count++;
  c->time = SDL_GetTicks();

  // Calculate speed and FPS
  if (c->last_time + 10 <= c->time) {
    c->last_time = c->time;
    c->speed = ((float) c->frame_count)/0.6;
    c->fps_tick++;

    // LOG FPS
    if (c->fps_tick == 100) {
      printf("FPS: %d\n", (c->frame_count * 60));
      c->fps_tick = 0;
    }

    c->frame_count = 0;
  }

  // Resolve keyframes (for animations)
  if (c->frame_count % (int) c->speed == 0) {
    c->keyframe = true;
  } else {
    c->keyframe = false;
  }
}

Game* Game_init(SDL_Renderer* ren)
{
  Vec ship_pos = {0,0};
  Game* game = malloc(sizeof(Game));

  game->running = false;
  game->ship = init_ship(ship_pos, ren);
  game->explosions = init_explosions(ren);
  game->asteroids = init_asteroids(ren);
  game->projectiles = NULL;
  game->projectile_texture = zxc_load_texture("images/spaceship.png", ren);

  return game;
}

void Game_start(Game* game, SDL_Window* win)
{
  int win_width;
  int win_height;

  SDL_GetWindowSize(win, &win_width, &win_height);
  Vec ship_position = { win_width-SSIZE/2, win_height-SSIZE/2};

  create_random_asteroid(game->asteroids, win_width, win_height);
  game->ship.pos = ship_position;
  game->running = true;
}

// TODO: return end game state
void Game_update(Game* game,
                 FpsCounter* fps,
                 Input input,
                 int win_width,
                 int win_height)
{
  // Ship
  update_ship(&input, &game->ship, fps->speed, win_width, win_height);
  // Asteroids
  update_asteroids(game->asteroids, fps->speed, win_width, win_height);

  // Projectiles
  game->projectiles = update_projectiles(game->projectiles,
                                         win_width,
                                         win_height,
                                         fps->speed);
  game->projectiles = colide_asteroids(game->asteroids,
                                       game->projectiles,
                                       game->explosions,
                                       win_width,
                                       win_height);

  // Ship collision with asteroids
  if (circle_colide_with_asteroids(game->asteroids, game->ship.pos, 24)) {
    printf("Game over\n");
    create_explosion(game->explosions, ExplosionHuge, game->ship.pos);
  }
}

void Game_render(Game* game, FpsCounter* fps, SDL_Renderer* ren)
{
  SDL_RenderClear(ren);

  render_projectiles(game->projectiles, game->projectile_texture, ren);
  render_asteroids(game->asteroids, fps->keyframe, ren);
  render_ship(&game->ship, ren);
  render_explosions(game->explosions, fps->keyframe, ren);

  SDL_RenderPresent(ren);
}

void Game_destory(Game *game)
{
  SDL_DestroyTexture(game->projectile_texture);

  destroy_ship(&game->ship);
  destroy_explosions(game->explosions);
  destory_asteroids(game->asteroids);
  destroy_projectiles(game->projectiles);
}
