#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
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

typedef struct {
  SDL_Texture* projectile_texture;
  Ship ship;
  Asteroids* asteroids;
  Explosions* explosions;
  Projectiles* projectiles;
} Game;

Game* Game_init(SDL_Renderer* ren)
  ;
void Game_start(Game* game, SDL_Window* window)
  ;
void Game_loop(Game* game,
               FpsCounter* fps_counter,
               SDL_Renderer* ren,
               int* pwin_width,
               int* pwin_height)
  ;
void Game_update(Game* game,
                 FpsCounter* fps_counter,
                 Input input,
                 int win_width,
                 int win_height)
  ;
void Game_render(const Game* game,
                 const FpsCounter* fps_counter,
                 SDL_Renderer* renderer)
  ;
void Game_destory(Game *game)
  ;

#endif
