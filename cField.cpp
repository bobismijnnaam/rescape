
#include "cField.h"

// Constructor
cField::cField(SDL_Surface* dst) { // int fx, int fy, int fw, int fh,
    // x = fx;
    // y = fy;
    // w = fw;
    // h = fh;

    field.resize(15);

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            field[x].push_back(new cCell(x, y, dst));
        }
    }

    field[0][0]->visit();
    field[14][8]->visit();

    mode = MODE_SHADOW;
}

// Destructor
cField::~cField() {

}

// Logic loop
int cField::logic() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            field[x][y]->logic();
        }
    }

    return 0;
}

// Render loop
int cField::render(SDL_Surface* dst) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            field[x][y]->render(mode, dst);
        }
    }

    return 0;
}

// Fills the field and keeps the first move in mind
int cField::populate(int rcb) { // right corner bottom

    return 0;
}

// Sets the display mod (fogged, furniture)
int cField::sMode(FieldMode fMode) {
    mode = fMode;

    return 0;
}

// Visits a cell, i.e. removes the fog above a cell
// The fog will start to fade away.
int cField::visit(int x, int y) {
    field[x][y]->visit();
    return 0;
}

// Leaves the cell, makes it fade in properly
int cField::leave(int x, int y) {
    field[x][y]->leave();
    return 0;
}

// Checks if there's furniture on a designated spot
bool cField::isEmpty(int x, int y) {
    field[x][y]->visit();
    return 0;
}

// Returns the amount of furnishings in the vicinity
int cField::scan(int x, int y) {
    int cX, cY;
    int count;

    for (int iX = -1; iX < 2; ++iX) {
        for (int iY = -1; iY < 2; ++iY) {
            cX = x + iX; cY = y + iY;
            if (!(cX < 0 || cX > 15 || cY < 0 || cY > 9)) {
                // ++count if furniture
            }
        }
    }

    return count;
}
