#ifndef CGAMETIMER_H
#define CGAMETIMER_H

#include <SDL/SDL_ttf.h>

enum GameTimerStates {
    STATE_RESET = 0,
    STATE_STOPPED,
    STATE_RUNNING,
    STATE_TPAUSED
} ;

class cGameTimer {
public:
    cGameTimer(TTF_Font* captionFont, TTF_Font* timeFont);
    ~cGameTimer();

    int render(SDL_Surface* dst, int x, int y);
    int start();
    int stop();
    int pause();

    int g();

private:
    int startTime;
    int pausedTime;
    int endTime;

    GameTimerStates state;

    TTF_Font* fCaption;
    TTF_Font* fTime;
    SDL_Surface* sCaption;
} ;

#endif
