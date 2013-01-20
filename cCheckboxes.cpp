#include "cCheckBoxes.h"

cCheckboxesOne::cCheckboxesOne(int x, int y, int fw, int fh, int fborder, int fd, bool fhorizontal, Uint32 fouter, Uint32 finner, TTF_Font* font, SDL_Color color, int dx, int dy) {
    sX = x;
    sY = y;
    w = fw;
    h = fh;
    border = fborder;
    d = fd;
    horizontal = fhorizontal;
    outer = fouter;
    inner = finner;
    f = font;
    clr = color;
    offx = dx;
    offy = dy;
}

cCheckboxesOne::~cCheckboxesOne() {
    if (boxes.size() > 0) {
        for (int i = 0; i < boxes.size(); i++) {
            delete boxes[i];
        }
    }
}

int cCheckboxesOne::events(SDL_Event* event) {
    for (int i = 0; i < boxes.size(); i++) {
        switch (boxes[i]->events(event)){
            case 2: // Box went on
                // Turn all other boxes off
                for (int j = 0; j < boxes.size(); j++) {
                    if (j != i) boxes[j]->s(false);
                }
                break;
            case 1: // Box went off
                // Turn it back on, because it's not allowed to have all off
                boxes[i]->s(true);
                break;
        }
    }

    return 0;
}

int cCheckboxesOne::render(SDL_Surface* dst) {
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i]->render(dst);
    }

    return 0;
}

int cCheckboxesOne::addBox(std::string caption) {
    int nx, ny;

    if (horizontal) {
        nx = sX + d * boxes.size();
        ny = sY;
    } else {
        nx = sX;
        ny = sY + d * boxes.size();
    }

    boxes.push_back(new cCheckbox(nx, ny, w, h, border, outer, inner, f, clr, caption, offx, offy));
    return 0;
}

int cCheckboxesOne::g() {
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i]->g()) return i+1;
    }

    return 0;
}

int cCheckboxesOne::s(int p, int selected) {
    boxes[p - 1]->s(selected);

    for (int i = 0; i < boxes.size(); i++) {
        if (i != p - 1) boxes[i]->s(false);
    }

    return 0;
}
