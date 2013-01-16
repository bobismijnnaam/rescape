#ifndef GSGAME_H
#define GSGAME_H

#include <SDL/SDL.h>

#include "cField.h"
#include "gamestate.h"

enum IngameStates {
    STATE_INGAME = 0,
    STATE_PAUSED,
    STATE_LOSE,
    STATE_WIN,
    STATE_SHOW
};

class gsGame : public cGameState {
public:
    gsGame();
    ~gsGame();

    int events();
    int logic();
    int render();

private:
    SDL_Surface* sBG;
    cField* field;
} ;

#endif
