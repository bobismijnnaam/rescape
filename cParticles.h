#ifndef CPARTICLES_H
#define CPARTICLES_H

#include <SDL/SDL.h>
#include <vector>

const int PART_KILL = 1;

class cParticle {
public:
    cParticle(int X, int Y, float VX, float VY, float Slowdown, int Lifespan, SDL_Surface* T);
    ~cParticle();

    int logic();
    int render(SDL_Surface* dst);

private:
    float x, y;
    float vx, vy;
    float slowdown;

    int creationTime;
    int lastTime;
    int currTime;

    int lifespan;

    SDL_Surface* t;

} ;

class cEmitter {
public:
    cEmitter(float MinVX, float MaxVX, float MinVY, float MaxVY, float Slowdown, int MinLifespan, int MaxLifespan, SDL_Surface* T);
    ~cEmitter();

    int pulse(int X, int Y, int MinAmount, int MaxAmount);
    int line(int Sx, int Sy, int Ex, int Ey, int Time, int Delay);
    int emit(int x, int y);

    int logic();
    int render(SDL_Surface* dst);

private:
    int minVX, minVY;
    int maxVX, maxVY;
    float slowdown;
    int minLife, maxLife;
    SDL_Surface* t;

    bool isEmitting; // Is it emitting
    int emitStart; // When did it start
    int emitTime; // How long does it last
    int emitDelay; // Time between emitted particles
    int emitLast;
    int sx, sy, ex, ey; // start & end

    std::vector<cParticle*> p;
} ;

#endif
