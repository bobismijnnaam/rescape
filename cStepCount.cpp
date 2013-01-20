#include <sstream>

#include "cStepCount.h"
#include "functions.h"

cStepCount::cStepCount(TTF_Font* captionFont, TTF_Font* countFont) {
    fCaption = captionFont;
    fCount = countFont;
    count = 0;

    SDL_Color clrGrey = {128, 128, 128};
    sCaption = TTF_RenderText_Blended(fCaption, "Steps", clrGrey);

    std::stringstream ss;
    ss << count;

    SDL_Color clrWhite = {255, 255, 255};
    sCount = TTF_RenderText_Blended(fCount, ss.str().c_str(), clrWhite);
}

cStepCount::~cStepCount() {
    SDL_FreeSurface(sCaption);
    SDL_FreeSurface(sCount);
}

int cStepCount::render(SDL_Surface* dst, int x, int y) {
    applySurface(sCaption, dst, x, y);
    applySurface(sCount, dst, x, y + sCaption->h + 5);
    return 0;
}

int cStepCount::step() {
    ++count;

    std::stringstream ss;
    ss << count;

    SDL_FreeSurface(sCount);

    SDL_Color clrWhite = {255, 255, 255};
    sCount = TTF_RenderText_Blended(fCount, ss.str().c_str(), clrWhite);

    return 0;
}
