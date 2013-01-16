#include <SDL/SDL.h>

#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    bgB = loadImage("Media/Images/bgMenu1.png");
    bgF = loadImage("Media/Images/bgMenu2.png");
    light = loadImage("Media/Images/light.png");

    lightState = true;
    lightTime = SDL_GetTicks();

    const SDL_PixelFormat* fmt = screen->format;
    sFade = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_W, SCR_H, SCR_BPP, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
    fresh(sFade, true);
    fadeTime = SDL_GetTicks();

    Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 grey = SDL_MapRGB(screen->format, 128, 128, 128);

    buttons = new cButtonSet(fButton, true, B_MIN, 140, 200, 20, 50, 500, 50, DIR_LEFT, white, grey, screen->format);
    buttons->addB("Play");
    buttons->addB("Help");
    buttons->addB("About");
    buttons->addB("Exit");
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bgF);
    SDL_FreeSurface(bgB);
    SDL_FreeSurface(light);
    SDL_FreeSurface(sFade);

    delete buttons;
}

int gsMenu::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        }

        buttons->handleEvents(&event);
    }

    return 0;
}

int gsMenu::logic() {
    if (SDL_GetTicks() - lightTime > 10000) {
        lightState = !lightState;
        lightTime = SDL_GetTicks();
    }

    if (SDL_GetTicks() - fadeTime >= 2000) {
        fadeTime = -1;
    }

    if (buttons->gClicked() == 1) {
        gm->setNextState(STATE_GAME);
    }

    if (buttons->gClicked() == 4) {
        gm->setNextState(STATE_EXIT);
    }

    buttons->logic();

    return 0;
}

int gsMenu::render() {
    fresh(screen, false);

    applySurface(bgB, screen, 0, 0);

    if (lightState) {
        applySurface(light, screen, 435 + (SDL_GetTicks() - lightTime) / 10000.0 * 185, 388);
    } else {
        applySurface(light, screen, 620 - (SDL_GetTicks() - lightTime) / 10000.0 * 185, 388);
    }

    applySurface(bgF, screen, 0, 0);

    buttons->render(screen);

    if (fadeTime > -1) {
        int d = SDL_GetTicks() - fadeTime;
        if (d <= 2000) SDL_SetAlpha(sFade, SDL_SRCALPHA, (2000 - d) / 2000.0 * 255);
        applySurface(sFade, screen, 0, 0);
    }

    SDL_Flip(screen);

    return 0;
}
