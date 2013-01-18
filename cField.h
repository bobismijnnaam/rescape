#ifndef CFIELD_H
#define CFIELD_H

#include <SDL/SDL.h>
#include <vector>

const int POS_R = 0;
const int POS_C = 1;
const int POS_B = 2;

enum FieldMode {
    MODE_SHADOW = 0,
    MODE_DISPLAY
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
    int setFurn(bool occupy, SDL_Surface* furSurf);
    int visit();
    int leave();

    int gX();
    int gY();

    bool dVis; // Dijkstra's Visited

private:
    CellState state;
    int visitTime;

    int x, y;

    bool furniture;
    SDL_Surface* sFurniture;

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

    int populate(int rcb, int diff);
    int sMode(FieldMode fMode);
    int visit(int x, int y);
    int leave(int x, int y);

    int placeFurns(int rcb, int amount);
    bool evaluate();

    bool isEmpty(int x, int y);
    int scan(int x, int y);

private:
    std::vector< std::vector<cCell*> > field;

    FieldMode mode;

    SDL_Surface* furnies[9];

    // int x, y, w, h;
} ;

#endif
