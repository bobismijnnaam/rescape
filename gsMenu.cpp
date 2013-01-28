#include <SDL/SDL.h>

#include "cCheckboxes.h"
#include "constants.h"
#include "cParticles.h"
#include "functions.h"
#include "globals.h"
#include "gsMenu.h"

gsMenu::gsMenu() {
    bgB = loadImage("Media/Images/bgMenu1.png");
    bgF = loadImage("Media/Images/bgMenu2.png");

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

    sStory = loadImage("Media/Images/story.png");
    story = new cMover(336, 122, SCR_W, 122, sStory, 500);

    sAbout = loadImage("Media/Images/about.png");
    about = new cMover(336, 122, SCR_W, 122, sAbout, 500);

    mCricket = Mix_LoadMUS("Media/Sounds/cricket.wav");
    Mix_PlayMusic(mCricket, -1);

    light = new cLight();
    ufo = new cUfo();
    sound = new cSound(gm->snd);

    sParticle = createSurf(20, 20, screen);
    fresh(sParticle, false);

    p = new cEmitter(-50, 50, -50, 50, 0.5, 5000, 7500, sParticle);
}

gsMenu::~gsMenu() {
    SDL_FreeSurface(bgF);
    SDL_FreeSurface(bgB);
    SDL_FreeSurface(sFade);
    SDL_FreeSurface(sDiff);
    SDL_FreeSurface(sStory);
    SDL_FreeSurface(sAbout);
    SDL_FreeSurface(sParticle);

    Mix_FreeMusic(mCricket);

    delete buttons;

    delete cb;

    delete story;
    delete about;

    delete light;
    delete ufo;
    delete sound;

    delete p;
}

int gsMenu::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            // p->pulse(event.button.x, event.button.y, 2, 10);
            // p->line(event.button.x, event.button.y, event.button.x + 500, event.button.y, 1000, 1);
        }

        buttons->handleEvents(&event);

        ufo->events(&event);

        cb->events(&event);

        sound->events(&event);
    }

    return 0;
}

int gsMenu::logic() {
    if (buttons->gPressed() == 1) {
        // gm->setNextState(STATE_GAME);
        nextState = STATE_GAME;
        gm->diff = cb->g();

        if (story->gState() == STATE_INSCREEN || story->gState() == STATE_MOVINGIN) {
            story->toggle();
        }

        if (about->gState() == STATE_INSCREEN || about->gState() == STATE_MOVINGIN) {
            about->toggle();
        }
    }

    if (buttons->gPressed() == 2) {
        story->toggle();

        if (about->gState() == STATE_INSCREEN || about->gState() == STATE_MOVINGIN) {
            about->toggle();
        }
    }

    if (buttons->gPressed() == 3) {
        about->toggle();

        if (story->gState() == STATE_INSCREEN || story->gState() == STATE_MOVINGIN) {
            story->toggle();
        }
    }

    if (buttons->gClicked() == 4) {
        // gm->setNextState(STATE_EXIT);
        nextState = STATE_EXIT;

        if (story->gState() == STATE_INSCREEN || story->gState() == STATE_MOVINGIN) {
            story->toggle();
        }

        if (about->gState() == STATE_INSCREEN || about->gState() == STATE_MOVINGIN) {
            about->toggle();
        }
    }

    if (nextState != STATE_NULL && fader->gState() == STATE_INVISIBLE) {
        buttons->moveOut();
        fader->fIn();
    }

    if (buttons->gState() == B_AWAY) {
        gm->setNextState(nextState);
    }

    switch (light->logic(mCricket)) {
        case REQ_STARTMUS:
            if ((ufo->gState() == STATE_UFODONE || ufo->gState() == STATE_WAITING) && Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(mCricket, 0);
            }

            break;
        case REQ_STOPMUS:
            Mix_HaltMusic();

            break;
    }

    switch (ufo->logic()) {
        case REQ_STARTMUS:
            if (light->gState() != STATE_CRASHED && Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(mCricket, 0);
            }

            break;
        case REQ_STOPMUS:
            Mix_HaltMusic();

            break;
    }

    p->logic();

    buttons->logic();

    fader->logic();

    story->logic();
    about->logic();

    gm->snd = sound->g();

    return 0;
}

int gsMenu::render() {
    fresh(screen, false);

    applySurface(bgB, screen, 0, 0);

    light->render(screen);

    applySurface(bgF, screen, 0, 0);

    buttons->render(screen);

    ufo->render(screen);

    p->render(screen);

    applySurface(sDiff, screen, 150, 440);

    cb->render(screen);

    story->render(screen);
    about->render(screen);

    sound->render(screen);

    fader->render(screen, 0, 0);

    SDL_Flip(screen);

    return 0;
}
