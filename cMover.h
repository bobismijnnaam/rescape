#ifndef CMOVER_H
#define CMOVER_H

#include <SDL/SDL.h>

enum MoverStates {
    STATE_INSCREEN = 0,
    STATE_OUTSCREEN,
    STATE_MOVINGIN,
    STATE_MOVINGOUT
} ;

class cMover {
public:
    cMover(int sX, int sY, int aX, int aY, SDL_Surface* Surf, int Delay);
    ~cMover();

    int logic();
    int render(SDL_Surface* dst);

    int mIn();
    int mOut();
    int toggle();
    int sState(MoverStates newState);

    MoverStates gState();

private:
    int sx, sy; // Screen X/Y
    int ax, ay; // Away X/Y

    SDL_Surface* surf;

    int startTime;
    int delay;
    MoverStates state;

} ;

#endif
