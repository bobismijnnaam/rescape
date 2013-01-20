#include <SDL/SDL.h>

#include "cCheckboxes.h"
#include "functions.h"

cCheckbox::cCheckbox(int x, int y, int w, int h, int border, Uint32 couter, Uint32 cinner, TTF_Font* font, SDL_Color color, std::string fcaption, int dx, int dy) {
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

    offx = dx;
    offy = dy;

    oColor = couter;
    mColor = cinner;

    f = font;
    clr = color;
    caption = fcaption;

    sCaption = TTF_RenderText_Blended(f, caption.c_str(), clr);

    status = false;
}

cCheckbox::~cCheckbox() {
    SDL_FreeSurface(sCaption);
}

int cCheckbox::events(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONUP) {
        int mx = event->button.x;
        int my = event->button.y;
        if (mx > outer.x && mx < outer.x + outer.w && my > outer.y && my < outer.y + outer.h) {
            status = !status;
            if (status) {
                return 2;
            } else {
                return 1;
            }
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

    applySurface(sCaption, dst, outer.x + outer.w + offx, outer.y + offy);

    return 0;
}

bool cCheckbox::g() {
    return status;
}

int cCheckbox::s(bool selected) {
    status = selected;
    return 0;
}
