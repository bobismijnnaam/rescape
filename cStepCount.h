#ifndef CSTEPCOUNT_H
#define CSTEPCOUNT_H

#include <SDL/SDL_ttf.h>

class cStepCount {
public:
    cStepCount(TTF_Font* captionFont, TTF_Font* countFont);
    ~cStepCount();

    int render(SDL_Surface* dst, int x, int y);
    int step();

    int g();

private:
    int count;
    SDL_Surface* sCaption;
    SDL_Surface* sCount;
    TTF_Font* fCaption;
    TTF_Font* fCount;

} ;

#endif
