

#include "cMover.h"
#include "functions.h"

cMover::cMover(int sX, int sY, int aX, int aY, SDL_Surface* Surf, int Delay) {
    sx = sX;
    sy = sY;
    ax = aX;
    ay = aY;

    surf = Surf;
    delay = Delay;

    state = STATE_OUTSCREEN;
}

cMover::~cMover() {

}

int cMover::logic() {
    if (SDL_GetTicks() - startTime >= delay) {
        if (state == STATE_MOVINGIN) {
            state = STATE_INSCREEN;
        } else if (state == STATE_MOVINGOUT) {
            state = STATE_OUTSCREEN;
        }
    }

    return 0;
}

int cMover::render(SDL_Surface* dst) {
    float d = (SDL_GetTicks() - startTime) / (float)delay;

    switch (state) {
        case STATE_MOVINGIN:
            applySurface(surf, dst, sx * d + ax * (1 - d), sy * d + ay * (1 - d));
            break;
        case STATE_MOVINGOUT:
            applySurface(surf, dst, sx * (1 - d) + ax * d, sy * (1 - d) + ay * d);
            break;
        case STATE_INSCREEN:
            applySurface(surf, dst, sx, sy);
            break;
        case STATE_OUTSCREEN:
            applySurface(surf, dst, ax, ay);
            break;
    }

    return 0;
}

int cMover::mIn() {
    state = STATE_MOVINGIN;
    startTime = SDL_GetTicks();

    return 0;
}

int cMover::mOut() {
    state = STATE_MOVINGOUT;
    startTime = SDL_GetTicks();

    return 0;
}

int cMover::toggle() {
    switch (state) {
        case STATE_MOVINGIN:
            state = STATE_MOVINGOUT;
            startTime = SDL_GetTicks() - (delay - (SDL_GetTicks() - startTime));
            break;
        case STATE_MOVINGOUT:
            state = STATE_MOVINGIN;
            startTime = SDL_GetTicks() - (delay - (SDL_GetTicks() - startTime));
            break;
        case STATE_INSCREEN:
            this->mOut();
            break;
        case STATE_OUTSCREEN:
            this->mIn();
            break;
    }

    return 0;
}

int cMover::sState(MoverStates newState) {
    state = newState;
    startTime = SDL_GetTicks();

    return 0;
}

MoverStates cMover::gState() {
    return state;
}
