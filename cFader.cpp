

#include "cFader.h"
#include "functions.h"

cFader::cFader(SDL_Surface* surf, int time, int alpha, FaderStates startState) {
    sFade = surf;
    tFade = time;
    targetAlpha = alpha;

    state = startState;
    fadeTime = SDL_GetTicks();

    switch (state) {
        case STATE_VISIBLE:
            SDL_SetAlpha(sFade, SDL_SRCALPHA, targetAlpha);
            break;
        case STATE_INVISIBLE:
            SDL_SetAlpha(sFade, SDL_SRCALPHA, 0);
            break;
        case STATE_FADEIN:
            SDL_SetAlpha(sFade, SDL_SRCALPHA, 0);
            break;
        case STATE_FADEOUT:
            SDL_SetAlpha(sFade, SDL_SRCALPHA, targetAlpha);
            break;
    }
}

cFader::~cFader() {
    // Nothing to clean up! :D:D
}

int cFader::fIn() {
    state = STATE_FADEIN;
    fadeTime = SDL_GetTicks();

    return 0;
}

int cFader::fOut() {
    state = STATE_FADEOUT;
    fadeTime = SDL_GetTicks();

    return 0;
}

int cFader::setDelay(int time) {
    tFade = time;
    return 0;
}

int cFader::setSurf(SDL_Surface* surf) {
    sFade = surf;
    return 0;
}

int cFader::setAlpha(int alpha) {
    targetAlpha = alpha;
}

FaderStates cFader::gState() {
    return state;
}

int cFader::logic() {
    int a;

    if (state == STATE_FADEIN || state == STATE_FADEOUT) {
        if (SDL_GetTicks() - fadeTime >= tFade) {
            if (state == STATE_FADEIN) {
                state = STATE_VISIBLE;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, targetAlpha);
            } else if (state == STATE_FADEOUT) {
                state = STATE_INVISIBLE;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, 0);
            }
        } else {
            if (state == STATE_FADEIN) a = ((SDL_GetTicks() - fadeTime) / (float)tFade) * targetAlpha;
            if (state == STATE_FADEOUT) a =  (1 - ((SDL_GetTicks()- fadeTime) / (float)tFade)) * targetAlpha;
            SDL_SetAlpha(sFade, SDL_SRCALPHA, a);
        }
    }

    return 0;
}

int cFader::render(SDL_Surface* dst, int x, int y) {
    if (state == STATE_FADEIN || state == STATE_FADEOUT || state == STATE_VISIBLE) {
        applySurface(sFade, dst, x, y);
    }

    return 0;
}
