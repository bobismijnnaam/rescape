#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "gamestate.h"

SDL_Surface* screen = NULL;

cGameMan* gm = NULL;

TTF_Font* fHeadline = NULL;
TTF_Font* fButton;
