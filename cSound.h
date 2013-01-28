#ifndef CSOUND_H
#define CSOUND_H

#include <SDL/SDL.h>

class cSound {
public:
    cSound(bool On);
    ~cSound();

    int events(SDL_Event* event);
    int render(SDL_Surface* dst);

    bool g();

private:
    bool on;
    SDL_Surface* sSound;
    SDL_Rect clip[2];

} ;

#endif
