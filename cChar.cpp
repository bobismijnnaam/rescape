
#include "cChar.h"
#include "functions.h"

cChar::cChar() {
    x = 0;
    y = 0;
    tx = -1;
    ty = -1;

    animTime = 0;
    dir = RIGHT;
    state = STATE_WAIT;
    walked = false;

    sWait = loadImage("Media/Images/player.png");

    sWalk[0] = loadImage("Media/Images/player0.png");
    sWalk[1] = loadImage("Media/Images/player1.png");
    sWalk[2] = loadImage("Media/Images/player2.png");
    sWalk[3] = loadImage("Media/Images/player3.png");
    sWalk[4] = loadImage("Media/Images/player1black.png");

    clip[0].x = 0;
    clip[0].y = 0;
    clip[0].w = 40;
    clip[0].h = 80;

    clip[1].x = 40;
    clip[1].y = 0;
    clip[1].w = 40;
    clip[1].h = 80;

    clip[2].x = 80;
    clip[2].y = 0;
    clip[2].w = 40;
    clip[2].h = 80;

    clip[3].x = 120;
    clip[3].y = 0;
    clip[3].w = 40;
    clip[3].h = 80;
}

cChar::~cChar() {
    SDL_FreeSurface(sWait);
    SDL_FreeSurface(sWalk[0]);
    SDL_FreeSurface(sWalk[1]);
    SDL_FreeSurface(sWalk[2]);
    SDL_FreeSurface(sWalk[3]);
}

int cChar::events(SDL_Event* event) {
    int rval = 0;

    if (event->type == SDL_KEYDOWN) {
        if (state == STATE_WAIT) {
            switch (event->key.keysym.sym) {
                case SDLK_KP8: // Move up
                    if (y != 0) {
                        tx = x;
                        ty = y - 1;
                        state = STATE_WALK;
                    }

                    dir = UP;

                    break;
                case SDLK_KP9: // Move to up right
                    if (x != 14 && y != 0) {
                        tx = x + 1;
                        ty = y - 1;
                        state = STATE_WALK;
                    }

                    dir = UP;

                    break;
                case SDLK_KP6: // Move to right
                    if (x != 14) {
                        tx = x + 1;
                        ty = y;
                        state = STATE_WALK;
                    }

                    dir = RIGHT;

                    break;
                case SDLK_KP3: // Move to bottom right
                    if (x != 14 && y != 8) {
                        tx = x + 1;
                        ty = y + 1;
                        state = STATE_WALK;
                    }

                    dir = DOWN;

                    break;
                case SDLK_KP2: // Move to bottom
                    if (y != 8) {
                        tx = x;
                        ty = y + 1;
                        state = STATE_WALK;
                    }

                    dir = DOWN;

                    break;
                case SDLK_KP1: // Move to bottom left
                    if (x != 0 && y != 8) {
                        tx = x - 1;
                        ty = y + 1;
                        state = STATE_WALK;
                    }

                    dir = DOWN;

                    break;
                case SDLK_KP4: // Move to left
                    if (x != 0) {
                        tx = x - 1;
                        ty = y;
                        state = STATE_WALK;
                    }

                    dir = LEFT;

                    break;
                case SDLK_KP7: // Move to top left
                    if (x != 0 && y != 0) {
                        tx = x - 1;
                        ty = y - 1;
                        state = STATE_WALK;
                    }

                    dir = UP;

                    break;
                default:
                    // Not any keypresses that concern the character
                    break;
            }

            if (state == STATE_WALK) {
                animTime = SDL_GetTicks();
                walked = true;
                rval = 1;
            }
        }
    }

    return rval;
}

int cChar::logic(cField* field) {
    if (walked) {
        field->leave(x, y);
        field->visit(tx, ty);
        walked = false;
    }

    if (SDL_GetTicks() - animTime >= 500 && state == STATE_WALK) {
        state = STATE_WAIT;
        x = tx;
        y = ty;
    }

    if (SDL_GetTicks() - animTime >= 1000 && state == STATE_END1) {
        state = STATE_END2;
        animTime = SDL_GetTicks();
    }

    return 0;
}

int cChar::render(SDL_Surface* dst) {
    int rx, ry; // Render X, render Y
    int ax, ay, atx, aty; // Absolute X, absolute Y, absolute target X, absolute target Y
    float d = (SDL_GetTicks() - animTime) / (float)CHAR_DELAY; // How far we have progressed with walking, for LERP (0-1)
    int a = ((SDL_GetTicks() - animTime) % 400) / 100; // How far we have progressed with the animation (0-3)

    if (state == STATE_WAIT) {
        rx = x * 40 + 20;
        ry = y * 40 + 66 - 40;
        applyClipped(sWait, dst, rx, ry, &clip[dir]);
    } else if (state == STATE_WALK) {
        ax = x * 40 + 20;
        ay = y * 40 + 66 - 40;

        atx = tx * 40 + 20;
        aty = ty * 40 + 66 - 40;

        rx = atx * d + ax * (1 - d);
        ry = aty * d + ay * (1 - d);

        applyClipped(sWalk[dir], dst, rx, ry, &clip[a]);
    } else if (state == STATE_END1) {
        // HIDE
    } else if (state == STATE_END2) {
        d = (SDL_GetTicks() - animTime) / 4000.0;
        rx = 590 * (1 - d) + 710 * (d);
        ry = 380;
        applyClipped(sWalk[4], dst, rx, ry, &clip[a]);
    } else if (state == STATE_GONE) {
        // HIDE
    }

    return 0;
}

int cChar::exit() {
    animTime = SDL_GetTicks();
    state = STATE_END1;

    return 0;
}

int cChar::gX() {
    return x;
}

int cChar::gY() {
    return y;
}

CharState cChar::gS() {
    return state;
}
