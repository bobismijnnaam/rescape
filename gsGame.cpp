#include <cmath>
#include <SDL/SDL_ttf.h>
#include <sstream>
#include <string>

#include "cChar.h"
#include "constants.h"
#include "functions.h"
#include "globals.h"
#include "gsGame.h"

gsGame::gsGame() {
    sBG = loadImage("Media/Images/bgGame.png");

    diff = gm->diff * 10;
    steps = 0;

    field = new cField(screen);
    field->sMode(MODE_SHADOW);

    player = new cChar();

    detector = new cDetector();

    sPauseFade = loadImage("Media/Images/pauseinstructions.png");
    sPauseFade = SDL_DisplayFormat(sPauseFade);

    sStateFade = createSurf(SCR_W, SCR_H, screen);
    fresh(sStateFade, true);

    sEnterFade = loadImage("Media/Images/instructions.png"); // SDL_ConvertSurface(sPauseFade, sPauseFade->format, sPauseFade->flags);
    sEnterFade = SDL_DisplayFormat(sEnterFade);

    SDL_Color clrGrey = {128, 128, 128};
    SDL_Color clrBlack = {0, 0, 0};

    pauseFader = new cFader(sPauseFade, 500, 128, STATE_INVISIBLE);
    stateFader = new cFader(sStateFade, 500, 255, STATE_FADEOUT);
    enterFader = new cFader(sEnterFade, 500, 128, STATE_VISIBLE);

    stepCount = new cStepCount(fSmall, fMedium);
    gameTimer = new cGameTimer(fSmall, fMedium);

    buttons = new cButtonSet(fButton, true, B_AWAY, 185, 80, 0, 50, 500, 0, DIR_LEFT, white, grey, screen->format);
    buttons->addB("Resume");
    buttons->addB("Restart");
    buttons->addB("Back to main menu");
    buttons->addB("Exit");

    buttonsEnd = new cButtonSet(fButton, false, B_AWAY, 70, 450, 160, 0, 500, 0, DIR_DOWN, white, grey, screen->format);
    buttonsEnd->addB("Retry");
    buttonsEnd->addB("Show");
    buttonsEnd->addB("Main menu");

    sKP = loadImage("Media/Images/numpad.png");
    sKPCaption = TTF_RenderText_Blended(fSmall, "Controls", clrGrey);
    sKPSub = TTF_RenderText_Blended(fSmall, "Numpad", clrGrey);

    sBlinds = createSurf(634, SCR_H, screen);
    fresh(sBlinds, true);
    blind = false;

    sEndFade = createSurf(634, SCR_H, screen);
    fresh(sEndFade, true);
    endFader = new cFader(sEndFade, 2000, 255, STATE_INVISIBLE);

    sCaughtLight = loadImage("Media/Images/shade.png");
    caughtLight = false;

    state = STATE_NEWGAME;
    nextState = STATE_NULL;

    endTrigger = false;
}

gsGame::~gsGame() {
    SDL_FreeSurface(sPauseFade);
    SDL_FreeSurface(sStateFade);
    SDL_FreeSurface(sEnterFade);
    SDL_FreeSurface(sEndFade);
    SDL_FreeSurface(sBlinds);
    SDL_FreeSurface(sBG);
    SDL_FreeSurface(sKP);
    SDL_FreeSurface(sKPCaption);
    SDL_FreeSurface(sKPSub);

    delete field;
    delete player;
    delete detector;
    delete pauseFader;
    delete enterFader;
    delete stateFader;
    delete buttons;
    delete buttonsEnd;
    delete stepCount;
    delete gameTimer;
}

int gsGame::events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gm->setNextState(STATE_EXIT);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_r) {
                // gm->setNextState(STATE_GAME);
            } else if (event.key.keysym.sym == SDLK_SPACE) {
                // Empty
            }
        }

        switch (state) {
            case STATE_NEWGAME:
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_KP6:
                            field->populate(POS_R, diff);
                            state = STATE_INGAME;
                            break;
                        case SDLK_KP3:
                            field->populate(POS_C, diff);
                            state = STATE_INGAME;
                            break;
                        case SDLK_KP2:
                            field->populate(POS_B, diff);
                            state = STATE_INGAME;
                            break;
                    }

                    if (state == STATE_INGAME) {
                        enterFader->fOut();
                        stepCount->step();
                        gameTimer->start();
                    }
                }

                player->events(&event);

                break;
            case STATE_INGAME:
                if (player->events(&event) == 1) {
                    stepCount->step();
                }

                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        state = STATE_PAUSED;
                        pauseFader->fIn();
                        buttons->moveIn();
                        gameTimer->pause();
                    }
                }

                break;
            case STATE_PAUSED:
                buttons->handleEvents(&event);
                break;
            case STATE_LOSE:
            case STATE_WIN:
            case STATE_SHOW:
                buttonsEnd->handleEvents(&event);
                break;
        }
    }

    return 0;
}

int gsGame::logic() {
    field->logic();
    player->logic(field);

    SDL_Color white = {255, 255, 255};
    int c = 100;
    SDL_Color grey = {c, c, c};

    if (detector->logic(field, player) == 9 && state == STATE_INGAME) {
        gameTimer->stop();
        state = STATE_LOSE;
        blind = true;

        endScreen(sEndFade, "You got caught!", gameTimer->g(), stepCount->g(), fBigHeadline, fMedium, white, grey);
        endScreen(sCaughtLight, "You got caught!", gameTimer->g(), stepCount->g(), fBigHeadline, fMedium, white, grey);

        endFader->fIn();
    }

    if (player->gX() == 14 && player->gY() == 8 && state == STATE_INGAME) {
        state = STATE_WIN;
        player->exit();
        gameTimer->stop();
        detector->sState(STATE_NOISE);
        blind = true;

        endScreen(sEndFade, "You escaped!", gameTimer->g(), stepCount->g(), fBigHeadline, fMedium, white, grey);
        endFader->fIn();
    }

    if (state == STATE_PAUSED) { // Pause Menu
        switch (buttons->gPressed()) {
            case 1: // Resume
                state = STATE_INGAME;
                pauseFader->fOut();
                gameTimer->start();
                buttons->moveOut();
                break;
            case 2: // Restart
                nextState = STATE_GAME;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
            case 3: // Back to main
                nextState = STATE_MENU;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
            case 4: // Quit
                nextState = STATE_EXIT;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
        }
    } else if (state == STATE_LOSE || state == STATE_WIN || state == STATE_SHOW) { // Win/Lose/Show Menu
        switch (buttonsEnd->gPressed()) {
            case 1: // Retry
                nextState = STATE_GAME;
                stateFader->fIn();
                buttonsEnd->moveOut();
                break;
            case 2: // Display Room
                state = STATE_SHOW;
                field->sMode(MODE_DISPLAY);
                break;
            case 3: // Back to Main
                nextState = STATE_MENU;
                stateFader->fIn();
                buttonsEnd->moveOut();
                break;
        }
    }

    endFader->logic();

    if (state != STATE_SHOW) {
        if (endFader->gState() == STATE_VISIBLE && !endTrigger) {
            buttonsEnd->moveIn();
            endTrigger = true;
        }

        if (endFader->gState() == STATE_VISIBLE && state == STATE_LOSE) {
            caughtLight = true;
            blind = false;
            endFader->sState(STATE_INVISIBLE);
        }
    }

    buttons->logic();

    buttonsEnd->logic();

    pauseFader->logic();
    enterFader->logic();
    stateFader->logic();

    if (stateFader->gState() == STATE_VISIBLE) {
        gm->setNextState(nextState);
    }

    return 0;
}

int gsGame::render() {
    fresh(screen, true);

    // Game Layer
    applySurface(sBG, screen, 0, 0);

    field->render(screen);

    if (state != STATE_SHOW && state != STATE_LOSE && state != STATE_WIN) player->render(screen);

    int y = 130;

    applySurface(sKP, screen, 643, y - 108);

    applySurface(sKPCaption, screen, 656, y - 120);

    applySurface(sKPSub, screen, 659, y - 10);

    detector->render(screen, 640, y + 90);

    stepCount->render(screen, 650, y + 70);

    gameTimer->render(screen, 650, y + 20);

    if (state != STATE_SHOW) {
        if (blind) applySurface(sBlinds, screen, 0, 0);
        if (caughtLight) applySurface(sCaughtLight, screen, 0, 0);

        endFader->render(screen, 0, 0);
    }

    // Pause layer
    pauseFader->render(screen, 0, 0);

    enterFader->render(screen, 0, 0);

    buttons->render(screen);

    buttonsEnd->render(screen);

    // Top layer
    stateFader->render(screen, 0, 0);

    SDL_Flip(screen);

    return 0;
}

/* Code to remove shadow with shadow on
else if(event.type == SDL_MOUSEBUTTONDOWN) {
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
*/

/*
    // Change overlay based on game state
    // What did this do again?
    switch (state) {
        case STATE_NEWGAME:
        case STATE_INGAME:
            break;
        case STATE_PAUSED:
            break;
        case STATE_LOSE:
            break;
        case STATE_WIN:
            break;
        case STATE_SHOW:
            break;
    }
*/
