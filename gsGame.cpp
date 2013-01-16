#include <cmath>

#include "functions.h"
#include "globals.h"
#include "gsGame.h"

gsGame::gsGame() {
    sBG = loadImage("Media/Images/bgGame.png");

    field = new cField(screen);
    field->sMode(MODE_SHADOW);
}

gsGame::~gsGame() {
    SDL_FreeSurface(sBG);
    delete field;
}

int gsGame::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN) {
            // Space
        } else if(event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.x >= 20 && event.button.x < 620 && event.button.y >= 66 && event.button.y < 426) {
                int ix = floor((event.button.x - 20) / 600.0 * 15);
                int iy = floor((event.button.y - 66) / 360.0 * 9);

                if (event.button.button == SDL_BUTTON_LEFT) {
                    field->visit(ix, iy);
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    field->leave(ix, iy);
                }
            }
        }
    }

    return 0;
}

int gsGame::logic() {
    field->logic();
    return 0;
}

int gsGame::render() {
    fresh(screen, true);

    applySurface(sBG, screen, 0, 0);

    field->render(screen);

    SDL_Flip(screen);

    return 0;
}
