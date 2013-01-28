#ifndef GSMENU_H
#define GSMENU_H

#include <SDL/SDL.h>

#include "buttons.h"
#include "cCheckboxes.h"
#include "cFader.h"
#include "cLight.h"
#include "cMover.h"
#include "cParticles.h"
#include "cSound.h"
#include "cUfo.h"
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
    SDL_Surface* sFade;
    SDL_Surface* sDiff;
    SDL_Surface* sStory;
    SDL_Surface* sAbout;
    SDL_Surface* sParticle;

    Mix_Music* mCricket;

    bool lightState;
    int lightTime;

    GameStates nextState;

    cButtonSet* buttons;

    cCheckboxesOne* cb;

    cFader* fader;

    cMover* story;
    cMover* about;

    cLight* light;
    cUfo* ufo;
    cSound* sound;

    cEmitter* p;
} ;

#endif
