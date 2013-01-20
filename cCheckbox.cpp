#include <SDL/SDL.h>

#include "cCheckboxes.h"
#include "functions.h"

cCheckbox::cCheckbox(int x, int y, int w, int h, int border, Uint32 couter, Uint32 cinner, TTF_Font* font, SDL_Color color, std::string fcaption) {
    outer.x = x;
    outer.y = y;
    outer.w = w;
    outer.h = h;

    middle.x = x + border;
    middle.y = y + border;
    middle.w = w - border - border;
    middle.h = h - border - border;

    inner.x = x + border + border;
    inner.y = y + border + border;
    inner.w = w - border - border - border - border;
    inner.h = h - border - border - border - border;

    oColor = couter;
    mColor = cinner;

    f = font;
    clr = color;
    caption = fcaption;

    status = false;
}

cCheckbox::~cCheckbox() {
    SDL_FreeSurface(sCaption);
}

int cCheckbox::events(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (pointInRect(event->button.x, event->button.y, outer)) {
            status != status;
        }
    }

    return 0;
}

int cCheckbox::render(SDL_Surface* dst) {
    SDL_FillRect(dst, &outer, oColor);
    SDL_FillRect(dst, &middle, mColor);

    if (status) {
        SDL_FillRect(dst, &inner, oColor);
    }

    applySurface(sCaption, dst, outer.x + outer.w, outer.y);

    return 0;
}

bool cCheckbox::g() {
    return status;
}

int cCheckbox::s(bool selected) {
    status = selected;
    return 0;
}
