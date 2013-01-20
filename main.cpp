#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "globals.h"
#include "mtwist.h"

int init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    TTF_Init();

    screen = SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE);
    SDL_WM_SetCaption("Relentless Escape", NULL);

    mt_seed();

    gm = new cGameMan(STATE_INTRO);

    fHeadline = TTF_OpenFont("Media/Fonts/courier_noir.ttf", 50);
    fBigHeadline = TTF_OpenFont("Media/Fonts/courier_noir.ttf", 80);
    fButton = TTF_OpenFont("Media/Fonts/courier_noir.ttf", 40);
    fSmall = TTF_OpenFont("Media/Fonts/courier_noir.ttf", 20);
    fMedium = TTF_OpenFont("Media/Fonts/courier_noir.ttf", 30);

    white = SDL_MapRGB(screen->format, 255, 255, 255);
    grey = SDL_MapRGB(screen->format, 128, 128, 128);

    // console hack
    // freopen( "CON", "wt", stdout );
    // freopen( "CON", "wt", stderr );

    return 0;
}

int game() {
    while (gm->getState() != STATE_EXIT) {
        gm->currState->events();

        gm->currState->logic();

        gm->changeState();

        SDL_Delay(1);

        gm->currState->render();
    }

    return 0;
}

int quit() {
    delete gm;

    TTF_CloseFont(fHeadline);

    TTF_Quit();

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[]) {
    init();

    game();

    quit();

    return 0;
}
