#ifndef CLIGHT_H
#define CLIGHT_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

enum LightStates {
    STATE_MOVINGLEFT = 0,
    STATE_MOVINGRIGHT,
    STATE_CRASHED,
    STATE_DONE
} ;

class cLight {
public:
    cLight();
    ~cLight();

    int logic(Mix_Music* msc);
    int render(SDL_Surface* dst);

    LightStates gState();

private:
    SDL_Surface* light;
    Mix_Chunk* crash;
    Mix_Chunk* lightswitch;

    LightStates state;

    int absTime;
    int animTime;
    int delay;
} ;

#endif
