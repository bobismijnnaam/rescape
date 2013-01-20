#ifndef CCHAR_H
#define CCHAR_H

#include <SDL/SDL.h>

#include "cField.h"

const int CHAR_DELAY = 500;

enum CharState {
    STATE_WAIT = 0,
    STATE_WALK
} ;

enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
} ;

class cChar {
public:
    cChar();
    ~cChar();

    int events(SDL_Event* event);
    int logic(cField* field);
    int render(SDL_Surface* dst);

    int gX();
    int gY();

private:
    int x, y;
    int tx, ty;
    int animTime;
    bool walked;

    SDL_Surface* sWait;
    SDL_Surface* sWalk[4];

    SDL_Rect clip[4];

    CharState state;
    Direction dir;

} ;

#endif
