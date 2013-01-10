#ifndef GSMENU_H
#define GSMENU_H

#include <SDL/SDL.h>

#include "gamestate.h"

class gsMenu : public cGameState {
public:
    gsMenu();
    ~gsMenu();

    int events();
    int logic();
    int render();

private:
    SDL_Surface* bg;

} ;

#endif
