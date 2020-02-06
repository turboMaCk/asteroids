#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "window.h"
#include "ship.h"
#include "input.h"
#include "entities.h"

typedef struct {
  uint frame_count;
  uint last_time;
  float speed;
  uint time;
  uint fps_tick;
  bool keyframe;
} FpsCounter;

FpsCounter* FPSC_init()
  ;
void FPSC_destory(FpsCounter* counter)
  ;
void FPSC_update(FpsCounter* pcounter)
  ;

typedef enum {
              GameNotStarted,
              GameRunning,
              GameEnded,
              GamePaused,
} GameStatus;

typedef struct {
  GameStatus status;
  SDL_Texture* projectile_texture;
  Ship ship;
  Asteroids* asteroids;
  Explosions* explosions;
  Projectiles* projectiles;
  Uint32 score;
  TTF_Font* ui_font;
  SDL_Texture* bg_texture;
} Game;

Game* Game_init(SDL_Renderer* ren)
  ;
void Game_start(Game* game)
  ;
void Game_restart(Game* game, Window* window)
  ;
void Game_loop(Game* game,
               FpsCounter* fps_counter,
               SDL_Renderer* ren,
               Window* window)
  ;
void Game_update(Game* game,
                 FpsCounter* fps_counter,
                 Input input,
                 int win_width,
                 int win_height)
  ;
void Game_render(Game* game,
                 const FpsCounter* fps_counter,
                 SDL_Renderer* renderer,
                 int win_width,
                 int win_height)
  ;
void Game_destory(Game *game)
  ;

#endif
