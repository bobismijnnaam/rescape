#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "gamestate.h"

extern SDL_Surface* screen;

extern cGameMan* gm;

extern TTF_Font* fHeadline;
extern TTF_Font* fBigHeadline;
extern TTF_Font* fButton;
extern TTF_Font* fSmall;
extern TTF_Font* fMedium;

extern Uint32 white;
extern Uint32 grey;

#endif
