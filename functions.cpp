#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <string>
#include <sstream>

#include "functions.h"
#include "globals.h"

int endScreen(SDL_Surface* s, std::string caption, int first, int second, TTF_Font* fB, TTF_Font* fS, SDL_Color clrF, SDL_Color clrS) {
    SDL_Surface* sCaption;
    SDL_Surface* sCFirst;
    SDL_Surface* sCSecond;
    SDL_Surface* sFirst;
    SDL_Surface* sSecond;

    std::stringstream ss1;
    std::stringstream ss2;

    std::string strFirst;
    std::string strSecond;

    ss1 << first;
    strFirst = ss1.str();

    ss2 << second;
    strSecond = ss2.str();

    sCaption = TTF_RenderText_Blended(fB, caption.c_str(), clrF);

    sCFirst = TTF_RenderText_Blended(fS, "Time:", clrS);
    sCSecond = TTF_RenderText_Blended(fS, "Steps:", clrS);

    sFirst = TTF_RenderText_Blended(fS, strFirst.c_str(), clrF);
    sSecond = TTF_RenderText_Blended(fS, strSecond.c_str(), clrF);

    applySurface(sCaption, s, (s->w - sCaption->w) / 2, 200);

    int x1 = 230;
    int x2 = 430;
    int y = 300;

    applySurface(sCFirst, s, x1 - sCFirst->w, y);
    applySurface(sFirst, s, x1 + 5, y);

    applySurface(sCSecond, s, x2 - sCSecond->w, y);
    applySurface(sSecond, s, x2 + 5, y);

    SDL_FreeSurface(sCaption);
    SDL_FreeSurface(sCFirst);
    SDL_FreeSurface(sCSecond);
    SDL_FreeSurface(sFirst);
    SDL_FreeSurface(sSecond);

    return 0;
}

SDL_Surface* createSurf(int w, int h, SDL_Surface* dst) {
    SDL_Surface* dummy;
    const SDL_PixelFormat* fmt = dst->format;

    dummy = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

    return dummy;
}

int fresh(SDL_Surface* dst, bool black) {
    int c;

    if (black) {
        c = 0;
    } else {
        c = 255;
    }

    SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, c, c, c));

    return 0;
}

bool inRangeEx (int p, int min, int max) {
    if ((p < max) && (p > min)) {
        return true;
    }

    return false;
}

int applyClipped(SDL_Surface* src, SDL_Surface* dst, int x, int y, SDL_Rect* clip) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, clip, dst, &tRect);
}

int applySurface(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
    SDL_Rect tRect;
    int t = 0;

    tRect.x = x;
    tRect.y = y;

    t = SDL_BlitSurface(src, NULL, dst, &tRect);

    return t;
}

bool pointInRect(int x, int y, SDL_Rect r) {
    if (x >= r.x && x <= r.x + r.w) {
        if (y >= r.y && y <= r.y + r.h) {
            return true;
        }
    }

    return false;
}

SDL_Surface* loadImage(std::string src) {
    SDL_Surface* loaded = NULL;
    SDL_Surface* converted = NULL;

    loaded = IMG_Load(src.c_str());

    if (loaded != NULL) {
        converted = SDL_DisplayFormatAlpha(loaded);
        SDL_FreeSurface(loaded);
    }

    return converted;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
