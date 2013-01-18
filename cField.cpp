#include <list>
#include <iostream>
#include "mtwist.h"

#include "cField.h"
#include "functions.h"

// Constructor
cField::cField(SDL_Surface* dst) { // int fx, int fy, int fw, int fh,
    field.resize(15);

    furnies[0] = loadImage("Media/Images/fur0res.png");
    furnies[1] = loadImage("Media/Images/fur1res.png");
    furnies[2] = loadImage("Media/Images/fur2res.png");
    furnies[3] = loadImage("Media/Images/fur3res.png");
    furnies[4] = loadImage("Media/Images/fur4res.png");
    furnies[5] = loadImage("Media/Images/fur5res.png");
    furnies[6] = loadImage("Media/Images/fur6res.png");
    furnies[7] = loadImage("Media/Images/fur7res.png");
    furnies[8] = loadImage("Media/Images/fur8res.png");

    int d, r;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            field[x].push_back(new cCell(x, y, dst));

            /*
            d = mt_lrand() % 6;
            if (d == 0) {
                r = mt_lrand() % 10;
                field[x][y]->setFurn(true, furnies[r]);
            }
            */
        }
    }

    field[0][0]->visit();
    field[14][8]->visit();

    mode = MODE_SHADOW;
}

// Destructor
cField::~cField() {
    for (int i = 0; i < 9; i++) {
        SDL_FreeSurface(furnies[i]);
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            delete field[x][y];
        }
    }
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
int cField::populate(int rcb, int diff) { // right corner bottom
    placeFurns(rcb, diff);

    while (!evaluate()) {
        placeFurns(rcb, diff);
    }

    /*
    for (std::list<cCell*>::const_iterator i = cells.begin(), end = cells.end();i != end; ++i) {
        (*i)->setFurn(true, furnies[mt_lrand() % 9]);
    }
    */

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

int cField::placeFurns(int rcb, int amount) {
    std::list<cCell*> cells;

    // Clean the field. Being clean's never a bad thing
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            field[x][y]->setFurn(false, NULL);
            field[x][y]->dVis = false;
        }
    }

    // Make a list of all the possible cells to put furniture
    // Keep in mind the starting position, ending position & first move (rcb = right corner bottom)
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 15; x++) {
            if (!(rcb == POS_R && x == 1 && y == 0) != !(rcb == POS_C && x == 1 && y == 1) != !(rcb == POS_B && x == 0 && y == 1)) {
                if (!((x == 0 && y == 0) || (x == 14 && y == 8))) {
                    cells.push_back(field[x][y]);
                }
            }
        }
    }

    // Place diff amount of pieces of furniture in the room
    int c;
    std::list<cCell*>::iterator which;
    for (int i = 0; i < amount; ++i) {
        c = mt_lrand() % cells.size();
        which = cells.begin();

        for (int count = 0; count < c; ++count) {
            ++which;
        }

        (*which)->setFurn(true, furnies[mt_lrand() % 9]);
        cells.erase(which);
    }

    return 0;
}

bool cField::evaluate() {
    std::list<cCell*> cells;
    bool found = false;

    // Add first cell (top left one)
    cells.push_back(field[0][0]);
    field[0][0]->dVis = true;

    // Keep iterating until every possible route failed or we find the last cell
    // std::list<cCell*>::iterator i;
    int cx, cy;
    while (cells.size() > 0 && !found) {
        cCell*& i = cells.front();

        // Check if current cell is ending cell; if not continue dijkstra's, otherwise profit
        if (i->gX() == 14 && i->gY() == 8) {
            found = true;
        } else {
            // Iterate through all neighbours
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    cx = x + i->gX();
                    cy = y + i->gY();

                    // Mustn't be the middle or out of bounds
                    if (!((cx == i->gX() && cy == i->gY()) ||  cx < 0 || cy < 0 || cx > 14 || cy > 8)) {
                        // Must not be visited or be furniture
                        if (!field[cx][cy]->dVis && !field[cx][cy]->isFurniture()) {
                            cells.push_back(field[cx][cy]);
                            field[cx][cy]->dVis = true;
                        }
                    }
                }
            }

            // Cell has been checked, remove it from the queue
            cells.pop_front();
        }
    }

    return found;
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
