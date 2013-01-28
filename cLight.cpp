#include "mtwist.h"

#include "cLight.h"
#include "constants.h"
#include "functions.h"

cLight::cLight() {
    light = loadImage("Media/Images/light.png");
    crash = Mix_LoadWAV("Media/Sounds/crash.wav");
    lightswitch = Mix_LoadWAV("Media/Sounds/lightswitch.wav");

    absTime = SDL_GetTicks();
    animTime = SDL_GetTicks();

    state = STATE_MOVINGRIGHT;

    delay = mt_lrand() % 15000 + 15000;
}

cLight::~cLight() {
    SDL_FreeSurface(light);
    Mix_FreeChunk(crash);
    Mix_FreeChunk(lightswitch);
}

int cLight::logic(Mix_Music* msc) {
    int retVal = 0;

    if (state != STATE_CRASHED && state != STATE_DONE) {
        if (SDL_GetTicks() - absTime > delay) {
            state = STATE_CRASHED;
            animTime = SDL_GetTicks();
            Mix_PlayChannel(-1, crash, 0);
            retVal = REQ_STOPMUS;
        } else {
            if (SDL_GetTicks() - animTime >= 10000) {
                if (state == STATE_MOVINGLEFT) {
                    state = STATE_MOVINGRIGHT;
                } else {
                    state = STATE_MOVINGLEFT;
                }

                animTime = SDL_GetTicks();
            }
        }
    } else if (state == STATE_CRASHED) {
        if (SDL_GetTicks() - animTime >= 3000) {
            Mix_PlayChannel(-1, lightswitch, 0);
            state = STATE_DONE;
            animTime = SDL_GetTicks();
        }
    } else if (state == STATE_DONE) {
        if (SDL_GetTicks() - animTime >= 4000) {
            retVal = REQ_STARTMUS;
        }
    }

    return retVal;
}

int cLight::render(SDL_Surface* dst) {
    if (state != STATE_CRASHED && state != STATE_DONE) {
        float d = (SDL_GetTicks() - animTime) / 10000.0;

        if (state == STATE_MOVINGLEFT) {
            applySurface(light, dst, 435 * d + 620 * (1 - d), 388);
        } else {
            applySurface(light, dst, 435 * (1 - d) + 620 * d, 388);
        }
    } else if (state == STATE_DONE) {
        SDL_Rect t;
        t.x = 455;
        t.y = 320;
        t.w = 30;
        t.h = t.w;

        SDL_FillRect(dst, &t, SDL_MapRGB(dst->format, 255, 255, 255));
    }

    return 0;
}

LightStates cLight::gState() {
    return state;
}
