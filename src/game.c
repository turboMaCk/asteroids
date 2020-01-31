#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "window.h"
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
  // TODO: this might fail and should be handled
  TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 22);

  game->status = GameNotStarted;
  game->ship = Ship_init(ship_pos, ren);
  game->explosions = Explosions_init(ren);
  game->asteroids = Asteroids_init(ren);
  game->projectiles = NULL;
  game->projectile_texture = IMG_LoadTexture(ren, "images/spaceship.png");
  game->score = 0;
  game->ui_font = font;

  return game;
}

void Game_start(Game* game)
{
  game->status = GameRunning;
}

void Game_restart(Game* game, SDL_Window* win)
{
  int win_width;
  int win_height;

  SDL_GetWindowSize(win, &win_width, &win_height);
  Vec ship_position = { win_width/2, win_height/2 };
  game->ship.pos = ship_position;
  game->ship.vel = (Vec) {0,0};
  game->ship.rotation_mom = 0;
  game->ship.rotation = 0;
  game->score = 0;

  Projectiles_destroy(game->projectiles);
  game->projectiles = NULL;

  Explosions_destroy_all(game->explosions);
  Asteroids_destroy_all(game->asteroids);

  Asteroids_create_random(game->asteroids, win_width, win_height);
}

void Game_update(Game* game,
                 FpsCounter* fps,
                 Input input,
                 int win_width,
                 int win_height)
{
  // Ship
  Ship_update(&input, &game->ship, fps->speed, win_width, win_height);
  // Asteroids
  Asteroids_update(game->asteroids, fps->speed, win_width, win_height);

  // Projectiles
  game->projectiles = Projectiles_update(game->projectiles,
                                         win_width,
                                         win_height,
                                         fps->speed);
  game->projectiles = Collisions_projectile_asteroids(game->asteroids,
                                                      game->projectiles,
                                                      game->explosions,
                                                      &game->score,
                                                      win_width,
                                                      win_height);

  // Ship collision with asteroids
  Vec* asteroid_collision_position = Collisions_asteroids_circle(game->asteroids, game->ship.pos, 24);
  if (asteroid_collision_position) {
    Explosions_create(game->explosions, Explosions_generate_type(), *asteroid_collision_position);
    Explosions_create(game->explosions, ExplosionHuge, game->ship.pos);
    game->status = GameEnded;
  }
}

void Game_render_ui(const Game* game, SDL_Renderer* ren, int win_width)
{
  SDL_Color color = {255,255,255,255};

  // Initialize texture
  char text[32];
  snprintf(text, 10, "Score: %d", game->score);
  SDL_Surface* surf = TTF_RenderText_Blended(game->ui_font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);

  // render texture
  int width, height;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  SDL_Rect dest = {win_width - width - 32, 24, width, height};
  SDL_RenderCopy(ren, texture, NULL, &dest);
  SDL_DestroyTexture(texture);
}

void Game_render(const Game* game,
                 const FpsCounter* fps,
                 SDL_Renderer* ren,
                 int win_width,
                 int win_height)
{
  Projectiles_render(game->projectiles, game->projectile_texture, ren);
  Asteroids_render(game->asteroids, fps->keyframe, ren);
  if (game->status == GameRunning || game->status == GamePaused)
    Ship_render(&game->ship, ren);
  Explosions_render(game->explosions, fps->keyframe, ren);
  Game_render_ui(game, ren, win_width);
}

void Game_destory(Game *game)
{
  SDL_DestroyTexture(game->projectile_texture);
  TTF_CloseFont(game->ui_font);

  Ship_destroy(&game->ship);
  Explosions_destroy(game->explosions);
  Asteroids_destroy(game->asteroids);
  Projectiles_destroy(game->projectiles);
  free(game);
}

void Game_loop(Game* game, FpsCounter* fps, SDL_Renderer* ren, int* pwin_width, int* pwin_height)
{
  Input input = Input_init();

  while (game->status == GameRunning) {
    FPSC_update(fps);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // Handle game input
      Window_event_handler(&event, ren, pwin_width, pwin_height);
      Input_keyboard_handler(&event, &input);
      Input_controller_handler(&event, &input);

      switch (event.type) {
      case SDL_CONTROLLERBUTTONDOWN: {
        if (event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
          goto GAME_QUIT_REQUEST;
      } break;
      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          goto GAME_QUIT_REQUEST;
      } break;
      case SDL_QUIT: {
        GAME_QUIT_REQUEST:
        game->status = GamePaused;
      } break;
      }
    }

    if (fps->keyframe && Input_is_firing(&input)) {
      game->projectiles = Projectiles_create(game->projectiles,
                                             game->ship.pos,
                                             game->ship.vel,
                                             game->ship.rotation);
    }

    Game_update(game, fps, input, *pwin_width, *pwin_height);

    SDL_RenderClear(ren);
    Game_render(game, fps, ren, *pwin_width, *pwin_height);
    SDL_RenderPresent(ren);
  }
}
