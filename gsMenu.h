#ifndef GSMENU_H
#define GSMENU_H

#include <SDL/SDL.h>

#include "buttons.h"
#include "gamestate.h"

class gsMenu : public cGameState {
public:
    gsMenu();
    ~gsMenu();

    int events();
    int logic();
    int render();

private:
    SDL_Surface* bgF;
    SDL_Surface* bgB;
    SDL_Surface* light;
    SDL_Surface* sFade;

    bool lightState;
    int lightTime;

    int fadeTime;

    cButtonSet* buttons;
} ;

#endif
