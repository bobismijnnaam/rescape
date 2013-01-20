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

    Uint32 outer = SDL_MapRGB(screen->format, 80, 80, 80);
    Uint32 inner = SDL_MapRGB(screen->format, 0, 0, 0);
    SDL_Color cWhite = {255, 255, 255};
    cb = new cCheckboxesOne(270, 437, 20, 20, 3, 110, true, outer, inner, fSmall, cWhite, 6, 3);

    cb->addBox("Easy");
    cb->addBox("Medium");
    cb->addBox("Hard");
    cb->addBox("Knightmare");

    cb->s(gm->diff, true);

    sDiff = TTF_RenderText_Blended(fSmall, "Difficulty:", cWhite);

    nextState = STATE_NULL;
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bgF);
    SDL_FreeSurface(bgB);
    SDL_FreeSurface(light);
    SDL_FreeSurface(sFade);
    SDL_FreeSurface(sDiff);

    delete buttons;

    delete cb;
}

int gsMenu::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        }

        buttons->handleEvents(&event);

        cb->events(&event);
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
        gm->diff = cb->g();
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

    applySurface(sDiff, screen, 150, 440);

    cb->render(screen);

    fader->render(screen, 0, 0);

    SDL_Flip(screen);

    return 0;
}
