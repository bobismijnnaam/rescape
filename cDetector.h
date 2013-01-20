#ifndef CDETECTOR_H
#define CDETECTOR_H

#include "cChar.h"
#include "cField.h"

enum Detectorstate {
    STATE_DETECT = 0,
    STATE_NOISE
} ;

class cDetector {
public:
    cDetector();
    ~cDetector();

    int logic(cField* field, cChar* player);
    int render(SDL_Surface* dst, int x, int y);

    int sState(Detectorstate newState);

private:
    SDL_Surface* sDetector[9];
    SDL_Surface* sNoise;

    SDL_Rect clip[5];
    int noiseTime;
    int count;

    Detectorstate state;
} ;

#endif
