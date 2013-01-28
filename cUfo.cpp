#include "mtwist.h"

#include "constants.h"
#include "cUfo.h"
#include "functions.h"
#include "globals.h"

cUfo::cUfo() {
    sUfo = loadImage("Media/Images/ufo.png");
    sRays = loadImage("Media/Images/rays.png");
    sShrubbery = loadImage("Media/Images/shrubbery.png");

    mUfoIn = Mix_LoadWAV("Media/Sounds/ufoin.wav");
    mUfoOut = Mix_LoadWAV("Media/Sounds/ufoout.wav");
    mUfoBeaming = Mix_LoadWAV("Media/Sounds/beamingup.wav");

    state = STATE_WAITING;
    aniTime = 0;

    shrubState = STATE_NORMAL;

    delay[STATE_WAITING] = 100;
    delay[STATE_UFOIN] = 7487;
    delay[STATE_UFOBEAMING1] = 2500; // Length of sound sampless
    delay[STATE_UFOBEAMING2] = 1530;
    delay[STATE_UFOOUT] = 7487;
    delay[STATE_UFOAWAY] = 5000;
    delay[STATE_UFODONE] = 100;

    sParticle = loadImage("Media/Images/exhaust.png");
    sParticle2 = createSurf(2, 2, screen);
    fresh(sParticle2, true);

    p = new cEmitter(-10, -5, -30, 30, 0.8, 500, 1000, sParticle);
    pOut = new cEmitter(-80, -70, -20, 20, 0.8, 15000, 30000, sParticle);
    pShrubbery = new cEmitter(-20, -19, 100, 120, 2, 1000, 1500, sParticle2);
}

cUfo::~cUfo() {
    SDL_FreeSurface(sUfo);
    SDL_FreeSurface(sRays);
    SDL_FreeSurface(sShrubbery);
    SDL_FreeSurface(sParticle);
    SDL_FreeSurface(sParticle2);

    delete p;
    delete pOut;
    delete pShrubbery;

    Mix_FreeChunk(mUfoIn);
    Mix_FreeChunk(mUfoOut);
    Mix_FreeChunk(mUfoBeaming);
}

int cUfo::events(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONUP) {
        int mX = event->button.x;
        int mY = event->button.y;
        if (mX > 319 && mX < 319 + sShrubbery->w && mY > 383 && mY < 383 + sShrubbery->h && state == STATE_WAITING) {
            state = STATE_UFOIN;
            Mix_PlayChannel(-1, mUfoIn, 0);
            aniTime = SDL_GetTicks();
            p->line(0 - sUfo->w, 160, 298, 160, 7487, 20);
        }
    } else if (event->type == SDL_MOUSEMOTION) {
        int mX = event->button.x;
        int mY = event->button.y;
        if (mX > 319 && mX < 319 + sShrubbery->w && mY > 383 && mY < 383 + sShrubbery->h && state == STATE_WAITING) {
            shrubState = STATE_SHRUBBING;
        } else {
            shrubState = STATE_NORMAL;
        }
    }

    return 0;
}

int cUfo::logic() {
    int retVal = 0;

    int d = SDL_GetTicks() - aniTime;
    int dl = delay[state];

    switch (state) {
        case STATE_WAITING:
            // Do nothing
            d = 0;
            break;
        case STATE_UFOIN:
            if (d >= dl) {
                state = STATE_UFOBEAMING1;
                aniTime = SDL_GetTicks();
                Mix_PlayChannel(-1, mUfoBeaming, 0);
                retVal = REQ_STOPMUS;
            }

            break;
        case STATE_UFOBEAMING1:
            if (d >= dl) {
                aniTime = SDL_GetTicks();
                state = STATE_UFOBEAMING2;
            }

            break;
        case STATE_UFOBEAMING2:
            if (d >= dl) {
                state = STATE_UFOOUT;
                aniTime = SDL_GetTicks();
                Mix_PlayChannel(-1, mUfoOut, 0);
            }

            break;
        case STATE_UFOOUT:
            if (SDL_GetTicks() - aniTime >= 3000) {
                state = STATE_UFOAWAY;
                aniTime = SDL_GetTicks();
            }

            break;
        case STATE_UFOAWAY:
            if (d >= dl) {
                state = STATE_UFODONE;
                retVal = REQ_STARTMUS;
            }

            break;
        case STATE_UFODONE:
            // Do nothing
            break;
    }

    p->logic();
    pOut->logic();
    pShrubbery->logic();

    return retVal;
}

int cUfo::render(SDL_Surface* dst) {
    float d = (SDL_GetTicks() - aniTime) / (float)delay[state];
    int ad = ((SDL_GetTicks() - aniTime) % 200) / 50;

    switch (ad) {
        case 0:
            ad = -2;
            break;
        case 1:
            ad = 0;
            break;
        case 2:
            ad = 2;
            break;
        case 3:
            ad = 0;
            break;
    }

    switch (state) {
        case STATE_WAITING:
            if (shrubState == STATE_NORMAL) {
                applySurface(sShrubbery, dst, 319, 383);
            } else if (shrubState == STATE_SHRUBBING) {
                applySurface(sShrubbery, dst, 319 + ad, 383);
            }
            break;
        case STATE_UFOIN:
            applySurface(sUfo, dst, (298 + sUfo->w) * d - sUfo->w, 131);
            applySurface(sShrubbery, dst, 319, 383);
            break;
        case STATE_UFOBEAMING1:
            applySurface(sShrubbery, dst, 319 + ad, 383);
            applySurface(sRays, dst, 301, 162);
            applySurface(sUfo, dst, 298, 131);
            break;
        case STATE_UFOBEAMING2:
            applySurface(sShrubbery, dst, 319, 383 * (1 - d) + 162 * d);
            applySurface(sRays, dst, 301, 162);
            applySurface(sUfo, dst, 298, 131);

            if (mt_lrand() % 4 == 0) {
                pShrubbery->emit(319 + mt_lrand() % sShrubbery->w, (383 * (1 - d) + 162 * d) + 20);
            }
            break;
        case STATE_UFOOUT:
            d = d + 0.7;
            d = d * d * d * d * d * d * d * d * d * d * d * d * d * d * d * d;
            applySurface(sUfo, dst, 298 * (1 - d) + 750 * d, 131);
            pOut->emit(298 * (1 - d) + 750 * d, 160);
            break;
        case STATE_UFOAWAY:
        case STATE_UFODONE:
            // Shrubbery is gone now lol
            // FIND ME ONE
            break;
    }

    p->render(dst);
    pOut->render(dst);
    pShrubbery->render(dst);

    return 0;
}

UfoStates cUfo::gState() {
    return state;
}
