#ifndef GSINTRO_H
#define GSINTRO_H

#include "gamestate.h"

class gsIntro : public cGameState {
public:
    gsIntro();
    ~gsIntro();

    int events();
    int logic();
    int render();

private:

} ;

#endif
