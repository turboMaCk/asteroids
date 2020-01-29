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

void Menu_valigh_lines(SDL_Renderer* ren, uint clines, SDL_Texture* textures[], int win_width, int win_height)
{
  int space = 10;
  int total_height = 0;
  SDL_Rect rects[clines];

  // calculate height
  for (uint i = 0; i < clines; ++i) {
    int w, h;
    SDL_QueryTexture(textures[i], NULL, NULL, &w, &h);
    total_height += h;
    if (i + 1 < clines) total_height += space;
    SDL_Rect rect = {0,0, w,h};
    rects[i] = rect;
  }

  int top = (win_height - total_height) / 2;

  // Assign positions
  for (uint i = 0; i < clines; ++i) {
    rects[i].y = top;
    rects[i].x = win_width / 2 - rects[i].w / 2;
    top += rects[i].h + space;

    // render
    SDL_RenderCopy(ren, textures[i], NULL, &rects[i]);
  }
}

void Menu_render(Menu* menu, SDL_Renderer* ren, int win_width, int win_height)
{
  int clines = 2 + INST_LINES;
  SDL_Texture* lines[] = {
                                menu->title,
                                menu->score,
                                menu->instructions[0],
                                menu->instructions[1],
  };
  Menu_valigh_lines(ren, clines, lines, win_width, win_height);
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
