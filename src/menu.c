#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>

#include "menu.h"

// This sort of crates a cyclic dependecy
#include "game.h"

#define INST_LINES 2

Menu* Menu_init(SDL_Renderer *ren, Game* game)
{
  Menu* res = malloc(sizeof(Menu));

  char *title_text, *instructions[2];
  char score_text[64];

  snprintf(score_text, 64, "Score: %d", game->score);

  switch (game->status) {
  case GameRunning: {
    // This should not happen
    assert(true);
  }
  case GameEnded: {
    title_text = "Game Ended";
    instructions[0] = "Press ENTER or Start for new game.";
    instructions[1] = "Press ESC or Back to exit.";
  } break;
  case GameNotStarted:
  case GamePaused: {
    title_text = "Game Paused";
    instructions[0] = "Press ENTER or Start to continue.";
    instructions[1] = "Press ESC or Back to exit.";
  } break;
  }

  SDL_Color color = {255,255,255,255};

  // Title
  {
    TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 48);
    SDL_Surface* surf = TTF_RenderText_Blended(font, title_text, color);
    res->title = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }


  // Score
  {
    TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 32);
    SDL_Surface* surf = TTF_RenderText_Blended(font, score_text, color);
    res->score = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }

  // Instructions
  {
    TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 24);

    SDL_Texture **textures = malloc(sizeof(SDL_Texture*)*2);
    res->instructions = textures;
    for (int i = 0; i < INST_LINES; ++i) {
      SDL_Surface* surf = TTF_RenderText_Blended(font, instructions[i], color);
      *textures = SDL_CreateTextureFromSurface(ren, surf);
      SDL_FreeSurface(surf);
      ++textures;
    }

    TTF_CloseFont(font);
  }

  return res;
}

void Menu_render(Menu* menu, SDL_Renderer* ren, int win_width, int win_height)
{
  // Render title
  {
    int width, height;
    SDL_QueryTexture(menu->title, NULL, NULL, &width, &height);
    SDL_Rect dest = {win_width/2 - width/2, win_height/2 - height/2, width, height};
    SDL_RenderCopy(ren, menu->title, NULL, &dest);
  }

  // Render Score
  {
    int width, height;
    SDL_QueryTexture(menu->score, NULL, NULL, &width, &height);
    SDL_Rect dest = {win_width/2 - width/2, win_height/2 - height/2 + 60, width, height};
    SDL_RenderCopy(ren, menu->score, NULL, &dest);
  }

  // Render Instructions
  {
    for (int i = 0; i < INST_LINES; ++i) {
      int width, height;
      SDL_QueryTexture(menu->instructions[i], NULL, NULL, &width, &height);
      SDL_Rect dest = {win_width/2 - width/2, win_height/2 - height/2 + 110 + (40*i), width, height};
      SDL_RenderCopy(ren, menu->instructions[i], NULL, &dest);
    }
  }
}

void Menu_destroy(Menu* menu)
{
  SDL_DestroyTexture(menu->title);
  SDL_DestroyTexture(menu->score);

  for (int i = 0; i < INST_LINES; ++i) {
    SDL_DestroyTexture(menu->instructions[i]);
  }

  free(menu->instructions);
  free(menu);
}
