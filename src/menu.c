#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "menu.h"

// This sort of crates a cyclic dependecy
#include "game.h"

SDL_Texture* Menu_init_title(SDL_Renderer* ren, const char* text)
{
  TTF_Font* font = TTF_OpenFont("fonts/Monda-Regular.ttf", 48);
  SDL_Color color = {255,255,255,255};

  SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);

  return texture;
}

Menu* Menu_init(SDL_Renderer *ren, Game* game)
{
  Menu* res = malloc(sizeof(Menu));

  char* title_text;
  switch (game->status) {
  case GameEnded: {
    title_text = "Game Ended";
  } break;
  case GamePaused: {
    title_text = "Game Paused";
  } break;
  }

  res->title = Menu_init_title(ren, title_text);

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
}

void Menu_destroy(Menu* menu)
{
  SDL_DestroyTexture(menu->title);

  free(menu);
}
