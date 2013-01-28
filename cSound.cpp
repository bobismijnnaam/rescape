#include <SDL/SDL_mixer.h>

#include "cSound.h"
#include "functions.h"

cSound::cSound(bool On) {
    sSound = loadImage("Media/Images/sound.png");
    on = On;

    for (int i = 0; i < 2; i++) {
        clip[i].x = i * 30;
        clip[i].y = 0;
        clip[i].w = 30;
        clip[i].h = 30;
    }

    if (on) {
        Mix_Volume(-1, 128);
        Mix_VolumeMusic(128);
    } else {
        Mix_Volume(-1, 0);
        Mix_VolumeMusic(0);
    }
}

cSound::~cSound() {
    SDL_FreeSurface(sSound);
}

int cSound::events(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONUP) {
        int mX = event->button.x;
        int mY = event->button.y;

        if (mX > 46 && mX < 76 && mY > 431 && mY < 461) {
            on = !on;

            if (on) {
                Mix_Volume(-1, 128);
                Mix_VolumeMusic(128);
            } else {
                Mix_Volume(-1, 0);
                Mix_VolumeMusic(0);
            }
        }
    }

    return 0;
}

int cSound::render(SDL_Surface* dst) {
    if (on) {
        applyClipped(sSound, dst, 46, 431, &clip[0]);
    } else {
        applyClipped(sSound, dst, 46, 431, &clip[1]);
    }

    return 0;
}

bool cSound::g() {
    return on;
}
