#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>

#include "menu.h"

// This sort of crates a cyclic dependecy
#include "game.h"

Menu* Menu_init(SDL_Renderer *ren, Game* game)
{
  Menu* res = malloc(sizeof(Menu));

  char *title_text, *instructions_text;
  char score_text[64];

  snprintf(score_text, 64, "Score: %d", game->score);

  switch (game->status) {
  case GameRunning: {
    // This should not happen
    assert(true);
  }
  case GameEnded: {
    title_text = "Game Ended";
    instructions_text = "Press ENTER or Start for new game; Press ESC or Back to exit.";
  } break;
  case GameNotStarted:
  case GamePaused: {
    title_text = "Game Paused";
    instructions_text = "Press ENTER or Start to continue; Press ESC or Back to exit.";
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
    SDL_Color color = {255,255,255,255};
    TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 24);
    SDL_Surface* surf = TTF_RenderText_Blended(font, instructions_text, color);
    res->instructions = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
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
    int width, height;
    SDL_QueryTexture(menu->instructions, NULL, NULL, &width, &height);
    SDL_Rect dest = {win_width/2 - width/2, win_height/2 - height/2 + 110, width, height};
    SDL_RenderCopy(ren, menu->instructions, NULL, &dest);
  }
}

void Menu_destroy(Menu* menu)
{
  SDL_DestroyTexture(menu->title);

  free(menu);
}
