#include <SDL/SDL.h>

#include "cField.h"
#include "functions.h"

// Constructor
cCell::cCell(int fX, int fY, SDL_Surface* dst) {
    x = fX;
    y = fY;

    dVis = false;
    furniture = false;
    state = STATE_UNVISITED;
    visitTime = 0;

    sFade = createSurf(40, 40, dst);
    SDL_FillRect(sFade, NULL, SDL_MapRGB(dst->format, 0, 0, 0));
    SDL_SetAlpha(sFade, SDL_SRCALPHA, 255);
    delay = 500;
    threshold = 100;

    if ((x == 0 && y == 0) || (x == 14 && y == 8)) {
        delay = 3000;
    }

    sFurniture = NULL;
}

// Destructor
cCell::~cCell() {
    SDL_FreeSurface(sFade);
}

// Logic loop
int cCell::logic() {
    int d = SDL_GetTicks() - visitTime;

    switch (state) {
        case STATE_UNVISITED:
            // Let it beeeee, let it be
            break;
        case STATE_UNOCCUPIED:
            // Let it beeee, let it be
            break;
        case STATE_OCCUPIED:
            // Let it beeee, let it be
            break;
        case STATE_TOOCCUPIEDLONG:
            if (d > delay) {
                state = STATE_OCCUPIED;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, 0);
            } else {
                int a = (1 - d / (float(delay))) * 255;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, a);
            }
            break;
        case STATE_TOOCCUPIEDSHORT:
            if (d > delay) {
                state = STATE_OCCUPIED;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, 0);
            } else {
                int a = (1 - d / (float(delay))) * threshold;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, a);
            }
            break;
        case STATE_TOUNOCCUPIED:
            if (d > delay) {
                state = STATE_UNOCCUPIED;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, threshold);
            } else {
                int a = (d / (float(delay))) * threshold;
                SDL_SetAlpha(sFade, SDL_SRCALPHA, a);
            }
            break;
    }

    return 0;
}

// Render loop
int cCell::render(FieldMode mode, SDL_Surface* dst) {
    if (mode == MODE_SHADOW) {
        // Render fog & maybe furniture
        if (sFurniture != NULL) {
            applySurface(sFurniture, dst, 20 + x * 40, 66 + y * 40);
        }

        applySurface(sFade, dst, 20 + x * 40, 66 + y * 40);
    } else if (mode == MODE_DISPLAY) {
        // Render furniture zonder fog
        if (sFurniture != NULL) {
            applySurface(sFurniture, dst, 20 + x * 40, 66 + y * 40);
        }
    }

    return 0;
}

// Checkes whether or not the cell has furniture
bool cCell::isFurniture() {
    return furniture;
}

// Sets the cell to contain furniture
// What pieces it contains is randomly decided here
int cCell::setFurn(bool occupy, SDL_Surface* furSurf) {
    if (occupy) {
        furniture = true;
        sFurniture = furSurf;
    } else {
        furniture = false;
        sFurniture = NULL;
    }

    return 0;
}

int cCell::visit() {
    switch (state) {
        case STATE_UNVISITED:
            state = STATE_TOOCCUPIEDLONG;
            visitTime = SDL_GetTicks();
            break;
        case STATE_UNOCCUPIED:
        default:
            state = STATE_TOOCCUPIEDSHORT;
            visitTime = SDL_GetTicks();
            break;
    }

    return 0;
}

int cCell::leave() {
    state = STATE_TOUNOCCUPIED;
    visitTime = SDL_GetTicks();

    return 0;
}

int cCell::gX() {
    return x;
}

int cCell::gY() {
    return y;
}
