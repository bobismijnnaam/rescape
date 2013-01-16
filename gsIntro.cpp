

#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsIntro.h"
#include "knightslogo.h"

gsIntro::gsIntro() {
    logo = new cKnightsLogo(true, true, 16);

    sOgam = loadImage("Media/Images/1gam.png");
    sSdl = loadImage("Media/Images/sdl.gif");

    const SDL_PixelFormat* fmt = screen->format;
    sFade = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_W, SCR_H, SCR_BPP, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
    fresh(sFade, true);

    startTime = SDL_GetTicks();
    fadeTime = -1;
}

gsIntro::~gsIntro() {
    delete logo;
    SDL_FreeSurface(sOgam);
    SDL_FreeSurface(sSdl);
    SDL_FreeSurface(sFade);
}

int gsIntro::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
            gm->setNextState(STATE_MENU);
        }
    }

    return 0;
}

int gsIntro::logic() {
    logo->logic();

    if (SDL_GetTicks() - startTime > 3000 && fadeTime < 0) {
        fadeTime = SDL_GetTicks();
    }

    if (fadeTime > 0) {
        if (SDL_GetTicks() - fadeTime > 2000) {
            gm->setNextState(STATE_MENU);
        }
    }

    return 0;
}

int gsIntro::render() {
    fresh(screen, true);

    logo->render(screen, 260, 110);

    applySurface(sSdl, screen, 60, 200);
    applySurface(sOgam, screen, 560, 170);

    int d = SDL_GetTicks() - fadeTime;
    if (fadeTime > 0 && d <= 2000) {
        SDL_SetAlpha(sFade, SDL_SRCALPHA, d / 2000.0 * 255);
        applySurface(sFade, screen, 0, 0);
    } else if ( fadeTime > 0 && d > 2000) {
        SDL_SetAlpha(sFade, SDL_SRCALPHA, 255);
        applySurface(sFade, screen, 0, 0);
    }

    SDL_Flip(screen);

    return 0;
}
