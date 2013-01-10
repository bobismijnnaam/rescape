#ifndef GSINTRO_H
#define GSINTRO_H

#include "gamestate.h"
#include "knightslogo.h"

class gsIntro : public cGameState {
public:
    gsIntro();
    ~gsIntro();

    int events();
    int logic();
    int render();

private:
    cKnightsLogo* logo;

    SDL_Surface* sOgam;
    SDL_Surface* sSdl;
    SDL_Surface* sFade;

    int startTime;
    int fadeTime;
} ;

#endif
