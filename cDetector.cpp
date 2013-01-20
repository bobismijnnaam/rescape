

#include "cDetector.h"
#include "functions.h"

cDetector::cDetector() {
    sDetector[0] = loadImage("Media/Images/detector0.png");
    sDetector[1] = loadImage("Media/Images/detector1.png");
    sDetector[2] = loadImage("Media/Images/detector2.png");
    sDetector[3] = loadImage("Media/Images/detector3.png");
    sDetector[4] = loadImage("Media/Images/detector4.png");
    sDetector[5] = loadImage("Media/Images/detector5.png");
    sDetector[6] = loadImage("Media/Images/detector6.png");
    sDetector[7] = loadImage("Media/Images/detector7.png");
    sDetector[8] = loadImage("Media/Images/detector8.png");

    sNoise = loadImage("Media/Images/detectorn.png");

    count = 0;
    state = STATE_DETECT;

    for (int i = 0; i < 5; ++i) {
        clip[i].x = i * 100;
        clip[i].y = 0;
        clip[i].w = 100;
        clip[i].h = 130;
    }
}

cDetector::~cDetector() {
    for (int i = 0; i < 9; ++i) {
        SDL_FreeSurface(sDetector[i]);
    }

    SDL_FreeSurface(sNoise);
}

int cDetector::logic(cField* field, cChar* player) {
    count = field->scan(player->gX(), player->gY());

    if (count == 9 && state != STATE_NOISE) {
        state = STATE_NOISE;
        return 9;
    } else if (count == 9 && state == STATE_NOISE) {
        return 1;
    }

    return 0;
}

int cDetector::render(SDL_Surface* dst, int x, int y) {
    if (state == STATE_DETECT) {
        applySurface(sDetector[count], dst, x, y);
    } else if (state == STATE_NOISE) {
        int c = ((SDL_GetTicks() - noiseTime) % 150) / 30;
        applyClipped(sNoise, dst, x, y, &clip[c]);
    }

    return 0;
}

int cDetector::sState(Detectorstate newState) {
    state = newState;

    if (state == STATE_NOISE) {
        noiseTime = SDL_GetTicks();
    }

    return 0;
}
