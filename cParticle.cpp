

#include "cParticles.h"
#include "functions.h"

cParticle::cParticle(int X, int Y, float VX, float VY, float Slowdown, int Lifespan, SDL_Surface* T) {
    currTime = SDL_GetTicks();

    x = X;
    y = Y;
    vx = VX;
    vy = VY;
    slowdown = Slowdown;

    creationTime = SDL_GetTicks();
    lifespan = Lifespan;

    t = T;
}

cParticle::~cParticle() {

}

int cParticle::logic() {
    lastTime = currTime;
    currTime = SDL_GetTicks();

    if (currTime - creationTime > lifespan) {
        return PART_KILL;
    }

    float dt = (currTime - lastTime) / 1000.0;

    vx = vx - vx * (1 - slowdown) * dt;
    vy = vy - vy * (1 - slowdown) * dt;

    x = x + vx * dt;
    y = y + vy * dt;

    return 0;
}

int cParticle::render(SDL_Surface* dst) {
    applySurface(t, dst, (int)x, (int)y);

    return 0;
}
