#include <SDL/SDL.h>

#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    bg = loadImage("Media/Images/bgMenu.png");
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bg);
}

int gsMenu::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        }
    }

    return 0;
}

int gsMenu::logic() {

    return 0;
}

int gsMenu::render() {
    fresh(screen, false);

    applySurface(bg, screen, 0, 0);

    SDL_Flip(screen);

    return 0;
}
