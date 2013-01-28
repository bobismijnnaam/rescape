#ifndef CUFO_H
#define CUFO_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "cParticles.h"

enum UfoStates {
    STATE_WAITING = 0,
    STATE_UFOIN,
    STATE_UFOBEAMING1,
    STATE_UFOBEAMING2,
    STATE_UFOOUT,
    STATE_UFOAWAY,
    STATE_UFODONE
} ;

enum ShrubberyStates {
    STATE_NORMAL = 0,
    STATE_SHRUBBING
} ;

class cUfo {
public:
    cUfo();
    ~cUfo();

    int events(SDL_Event* event);
    int logic();
    int render(SDL_Surface* dst);

    UfoStates gState();

private:
    SDL_Surface* sUfo;
    SDL_Surface* sRays;
    SDL_Surface* sShrubbery;

    Mix_Chunk* mUfoIn;
    Mix_Chunk* mUfoOut;
    Mix_Chunk* mUfoBeaming;

    int delay[7];

    UfoStates state;
    int aniTime;

    ShrubberyStates shrubState;

    cEmitter* p;
    cEmitter* pOut;
    cEmitter* pShrubbery;
    SDL_Surface* sParticle;
    SDL_Surface* sParticle2;
} ;

#endif
