#ifndef CFADER_H
#define CFADER_H

#include <SDL/SDL.h>

enum FaderStates {
    STATE_VISIBLE = 0,
    STATE_INVISIBLE,
    STATE_FADEIN,
    STATE_FADEOUT
} ;

class cFader {
public:
    cFader(SDL_Surface* surf, int time, int alpha, FaderStates startState);
    ~cFader();

    int fIn();
    int fOut();

    int setDelay(int time);
    int setSurf(SDL_Surface* surf);
    int setAlpha(int alpha);

    FaderStates gState();

    int logic();
    int render(SDL_Surface* dst, int x, int y);

private:
    int fadeTime;
    FaderStates state;
    SDL_Surface* sFade;
    int tFade;
    int targetAlpha;
} ;

#endif
