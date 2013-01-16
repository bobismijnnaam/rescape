#ifndef CFIELD_H
#define CFIELD_H

#include <SDL/SDL.h>
#include <vector>

enum FieldMode {
    MODE_SHADOW = 0,
    MODE_DISPLAY
} ;

enum Furniture {
    FUR_NULL = 0,
    FUR_MAX = 2
} ;

enum CellState {
    STATE_UNVISITED = 0,
    STATE_UNOCCUPIED,
    STATE_OCCUPIED,
    STATE_TOOCCUPIEDLONG,
    STATE_TOOCCUPIEDSHORT,
    STATE_TOUNOCCUPIED,
} ;

class cCell {
public:
    cCell(int fX, int fY, SDL_Surface* dst);
    ~cCell();

    int logic();
    int render(FieldMode mode, SDL_Surface* dst);

    bool isFurniture();
    int sFurniture(bool occupy);
    int visit();
    int leave();

    bool dVis; // Dijkstra's Visited

private:
    CellState state;
    int visitTime;

    int x, y;

    bool furniture;
    Furniture furnitureID;

    SDL_Surface* sFade;
    int delay;
    int threshold;
} ;

class cField {
public:
    cField(SDL_Surface* dst); // int fx, int fy, int fw, int fh,
    ~cField();

    int logic();
    int render(SDL_Surface* dst);

    int populate(int rcb);
    int sMode(FieldMode fMode);
    int visit(int x, int y);
    int leave(int x, int y);

    bool isEmpty(int x, int y);
    int scan(int x, int y);

private:
    std::vector< std::vector<cCell*> > field;

    FieldMode mode;

    // int x, y, w, h;
} ;

#endif
