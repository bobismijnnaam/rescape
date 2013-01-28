#ifndef GSGAME_H
#define GSGAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "buttons.h"
#include "cChar.h"
#include "cDetector.h"
#include "cFader.h"
#include "cField.h"
#include "cGameTimer.h"
#include "cStepCount.h"
#include "gamestate.h"

enum IngameStates {
    STATE_INGAME = 0,
    STATE_NEWGAME,
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
    cField* field;
    cChar* player;
    cDetector* detector;
    cStepCount* stepCount;
    cGameTimer* gameTimer;

    cFader* pauseFader;
    cFader* enterFader;
    cFader* stateFader;
    cFader* endFader;

    cButtonSet* buttons;
    cButtonSet* buttonsEnd;

    IngameStates state;
    GameStates nextState;
    int diff;
    int steps;

    SDL_Surface* sPauseFade;
    SDL_Surface* sStateFade;
    SDL_Surface* sEnterFade;
    SDL_Surface* sEndFade;

    SDL_Surface* sBlinds;
    bool blind;

    SDL_Surface* sCaughtLight;
    bool caughtLight;

    SDL_Surface* sBG;
    SDL_Surface* sKP;
    SDL_Surface* sKPCaption;
    SDL_Surface* sKPSub;

    bool endTrigger;

    Mix_Chunk* mCrash;
    Mix_Chunk* mLightswitch;
    Mix_Chunk* mExit;
} ;

#endif
