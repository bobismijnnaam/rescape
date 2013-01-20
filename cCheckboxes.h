#ifndef CCHECKBOXES_H
#define CCHECKBOXES_H

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

class cCheckbox {
public:
    cCheckbox(int x, int y, int w, int h, int border, Uint32 couter, Uint32 cinner, TTF_Font* font, SDL_Color color, std::string fcaption, int dx, int dy);
    ~cCheckbox();

    int events(SDL_Event* event);
    int render(SDL_Surface* dst);

    bool g();
    int s(bool selected);

private:
    SDL_Rect outer;
    SDL_Rect middle;
    SDL_Rect inner;

    int offx;
    int offy;

    Uint32 oColor;
    Uint32 mColor;

    TTF_Font* f;
    SDL_Color clr;
    std::string caption;
    SDL_Surface* sCaption;

    bool status;
} ;

class cCheckboxesOne {
public:
    cCheckboxesOne(int x, int y, int fw, int fh, int fborder, int fd, bool fhorizontal, Uint32 fouter, Uint32 finner, TTF_Font* font, SDL_Color color, int dx, int dy);
    ~cCheckboxesOne();

    int events(SDL_Event* event);
    int render(SDL_Surface* dst);

    int addBox(std::string caption);

    int g();
    int s(int p, int selected);

private:
    std::vector<cCheckbox*> boxes;

    int sX, sY, w, h, border, d;
    bool horizontal;
    Uint32 outer, inner;

    TTF_Font* f;
    SDL_Color clr;
    int offx, offy;

} ;

class cCheckboxesMultiple {
    // Ooit
} ;

#endif
