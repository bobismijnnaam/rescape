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

    diff = 40;
    steps = 0;

    field = new cField(screen);
    field->sMode(MODE_SHADOW);

    player = new cChar();

    detector = new cDetector();

    sPauseFade = createSurf(SCR_W, SCR_H, screen);
    fresh(sPauseFade, true);
    sStateFade = SDL_ConvertSurface(sPauseFade, sPauseFade->format, sPauseFade->flags);
    sEnterFade = SDL_ConvertSurface(sPauseFade, sPauseFade->format, sPauseFade->flags);

    SDL_Color clrGrey = {128, 128, 128};
    SDL_Color clrBlack = {0, 0, 0};

    SDL_Surface* sPauseHeader = TTF_RenderText_Shaded(fBigHeadline, "Paused", clrGrey, clrBlack);
    applySurface(sPauseHeader, sPauseFade, 250, 10);
    SDL_FreeSurface(sPauseHeader);

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

    sKP = loadImage("Media/Images/numpad.png");
    sKPCaption = TTF_RenderText_Blended(fSmall, "Controls", clrGrey);
    sKPSub = TTF_RenderText_Blended(fSmall, "Numpad", clrGrey);

    state = STATE_NEWGAME;
    nextState = STATE_NULL;
}

gsGame::~gsGame() {
    SDL_FreeSurface(sPauseFade);
    SDL_FreeSurface(sStateFade);
    SDL_FreeSurface(sEnterFade);
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
                gm->setNextState(STATE_GAME);
            } else if (event.key.keysym.sym == SDLK_SPACE) {
                gameTimer->start();
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
                break;
            case STATE_WIN:
                break;
            case STATE_SHOW:
                break;
        }
    }

    return 0;
}

int gsGame::logic() {
    field->logic();

    player->logic(field);

    if (detector->logic(field, player) == 9 && state == STATE_INGAME) {
        gameTimer->stop();
        state = STATE_LOSE;
    }

    if (player->gX() == 14 && player->gY() == 8 && state == STATE_INGAME) {
        state = STATE_WIN;
    }

    if (state == STATE_PAUSED) {
        switch (buttons->gPressed()) {
            case 1:
                state = STATE_INGAME;
                pauseFader->fOut();
                gameTimer->start();
                buttons->moveOut();
                break;
            case 2:
                nextState = STATE_GAME;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
            case 3:
                nextState = STATE_MENU;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
            case 4:
                nextState = STATE_EXIT;
                pauseFader->fOut();
                stateFader->fIn();
                buttons->moveOut();
                break;
        }
    }

    if (stateFader->gState() == STATE_VISIBLE) {
        gm->setNextState(nextState);
    }

    buttons->logic();

    pauseFader->logic();
    enterFader->logic();
    stateFader->logic();

    return 0;
}

int gsGame::render() {
    fresh(screen, true);

    // Game Layer
    applySurface(sBG, screen, 0, 0);

    field->render(screen);

    player->render(screen);

    int y = 130;

    applySurface(sKP, screen, 643, y - 108);

    applySurface(sKPCaption, screen, 656, y - 120);

    applySurface(sKPSub, screen, 659, y - 10);

    detector->render(screen, 640, y + 90);

    stepCount->render(screen, 650, y + 70);

    gameTimer->render(screen, 650, y + 20);

    // Pause layer
    pauseFader->render(screen, 0, 0);

    buttons->render(screen);

    enterFader->render(screen, 0, 0);

    // Top layer
    stateFader->render(screen, 0, 0);

    // Change overlay based on game state
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
