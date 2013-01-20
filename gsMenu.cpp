#include <SDL/SDL.h>

#include "cCheckboxes.h"
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

    sFade = createSurf(SCR_W, SCR_H, screen);
    fresh(sFade, true);
    fader = new cFader(sFade, 500, 255, STATE_FADEOUT);

    buttons = new cButtonSet(fButton, true, B_MIN, 140, 200, 20, 50, 500, 50, DIR_LEFT, white, grey, screen->format);
    buttons->addB("Play");
    buttons->addB("The Story");
    buttons->addB("About");
    buttons->addB("Exit");

    nextState = STATE_NULL;
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

    if (buttons->gClicked() == 1) {
        // gm->setNextState(STATE_GAME);
        nextState = STATE_GAME;
    }

    if (buttons->gClicked() == 4) {
        // gm->setNextState(STATE_EXIT);
        nextState = STATE_EXIT;
    }

    if (nextState != STATE_NULL && fader->gState() == STATE_INVISIBLE) {
        buttons->moveOut();
        fader->fIn();
    }

    if (buttons->gState() == B_AWAY) {
        gm->setNextState(nextState);
    }

    buttons->logic();

    fader->logic();

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

    fader->render(screen, 0, 0);

    SDL_Flip(screen);

    return 0;
}
