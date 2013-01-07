#ifndef GSMENU_H
#define GSMENU_H

#include "gamestate.h"

class gsMenu : public cGameState {
public:
    gsMenu();
    ~gsMenu();

    int events();
    int logic();
    int render();

private:

} ;

#endif
