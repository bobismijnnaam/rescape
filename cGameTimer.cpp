#include <SDL/SDL_ttf.h>
#include <sstream>

#include "cGameTimer.h"
#include "functions.h"

cGameTimer::cGameTimer(TTF_Font* captionFont, TTF_Font* timeFont) {
    fCaption = captionFont;
    fTime = timeFont;

    SDL_Color clrGrey = {128, 128, 128};
    sCaption = TTF_RenderText_Blended(fCaption, "Time", clrGrey);

    state = STATE_RESET;
}

cGameTimer::~cGameTimer() {
    SDL_FreeSurface(sCaption);
}

int cGameTimer::render(SDL_Surface* dst, int x, int y) {
    SDL_Surface* sTime;
    int tTime;

    switch (state) {
        case STATE_RESET:
            tTime = 0;
            break;
        case STATE_STOPPED:
            tTime = (endTime - startTime) / 1000;
            break;
        case STATE_RUNNING:
            tTime = (SDL_GetTicks() - startTime) / 1000;
            break;
        case STATE_TPAUSED:
            tTime = (pausedTime - startTime) / 1000;
            break;
    }

    std::stringstream ss;
    ss << tTime;

    SDL_Color clrWhite = {255, 255, 255};
    sTime = TTF_RenderText_Blended(fTime, ss.str().c_str(), clrWhite);

    applySurface(sCaption, dst, x, y);
    applySurface(sTime, dst, x, y + sCaption->h + 5);

    SDL_FreeSurface(sTime);

    return 0;
}

int cGameTimer::start() {
    switch (state) {
        case STATE_RESET:
        case STATE_STOPPED:
            startTime = SDL_GetTicks();
            state = STATE_RUNNING;
            break;
        case STATE_RUNNING:
            break;
        case STATE_TPAUSED:
            startTime = SDL_GetTicks() - (pausedTime - startTime);
            state = STATE_RUNNING;
            break;
    }

    return 0;
}

int cGameTimer::stop() {
    switch (state) {
        case STATE_RESET:
        case STATE_STOPPED:
            state = STATE_RESET;
            break;
        case STATE_RUNNING:
            state = STATE_STOPPED;
            endTime = SDL_GetTicks();
            break;
        case STATE_TPAUSED:
            state = STATE_STOPPED;
            endTime = pausedTime;
            break;
    }

    return 0;
}

int cGameTimer::pause() {
    switch (state) {
        case STATE_RESET:
        case STATE_STOPPED:
            break;
        case STATE_RUNNING:
            state = STATE_TPAUSED;
            pausedTime = SDL_GetTicks();
            break;
        case STATE_TPAUSED:
            break;
    }

    return 0;
}

int cGameTimer::g() {
    int tTime;

    switch (state) {
        case STATE_RESET:
            tTime = 0;
            break;
        case STATE_STOPPED:
            tTime = (endTime - startTime) / 1000;
            break;
        case STATE_RUNNING:
            tTime = (SDL_GetTicks() - startTime) / 1000;
            break;
        case STATE_TPAUSED:
            tTime = (pausedTime - startTime) / 1000;
            break;
    }

    return tTime;
}
