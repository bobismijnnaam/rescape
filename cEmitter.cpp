#include "mtwist.h"

#include "constants.h"
#include "cParticles.h"

cEmitter::cEmitter(float MinVX, float MaxVX, float MinVY, float MaxVY, float Slowdown, int MinLifespan, int MaxLifespan, SDL_Surface* T) {
    minVX = MinVX;
    minVY = MinVY;
    maxVX = MaxVX;
    maxVY = MaxVY;
    slowdown = Slowdown;
    minLife = MinLifespan;
    maxLife = MaxLifespan;
    t = T;

    isEmitting = false;
}

cEmitter::~cEmitter() {
    for (int i = 0; i < p.size(); i++) {
        delete p[i];
    }
}

int cEmitter::pulse(int X, int Y, int MinAmount, int MaxAmount) {
    int vX, vY, lifeSpan, amount;

    if (MinAmount == MaxAmount) {
        amount = MaxAmount;
    } else {
        amount = (mt_lrand() % (MaxAmount - MinAmount + 1));
    }

    for (int i = 0; i < amount; i++) {
        this->emit(X, Y);
    }

    return 0;
}

int cEmitter::line(int Sx, int Sy, int Ex, int Ey, int Time, int Delay) {
    isEmitting = true;
    emitStart = SDL_GetTicks();
    emitTime = Time;
    emitDelay = Delay;

    sx = Sx;
    sy = Sy;
    ex = Ex;
    ey = Ey;

    this->emit(Sx, Sy);

    return 0;
}

int cEmitter::emit(int X, int Y) {
    int vX = (mt_lrand() % (maxVX - minVX)) + minVX;
    int vY = (mt_lrand() % (maxVY - minVY)) + minVY;
    int lifeSpan = (mt_lrand() % (maxLife - minLife)) + minLife;

    p.push_back(new cParticle(X, Y, vX, vY, slowdown, lifeSpan, t));

    return 0;
}

int cEmitter::logic() {
    if (isEmitting) {
        if (SDL_GetTicks() - emitStart > emitTime) {
            isEmitting = false;
        } else {
            if (SDL_GetTicks() - emitLast >= emitDelay) {
                float d = (SDL_GetTicks() - emitStart) / (float)emitTime;

                emit(sx * (1 - d) + ex * d, sy * (1 - d) + ey * d);

                emitLast = SDL_GetTicks();
            }
        }
    }

    for (int i = 0; i < p.size(); i++) {
        if (p[i]->logic() == PART_KILL) {
            cParticle* dummy;
            cParticle* deleted; // Switch the to be deleted particle to the back

            dummy = p[p.size() - 1];
            deleted = p[i];
            p[i] = dummy;
            p[p.size() - 1] = deleted;

            p.pop_back(); // Delete the particle in the back;
        }
    }

    return 0;
}

int cEmitter::render(SDL_Surface* dst) {
    for (int i = 0; i < p.size(); i++) {
        p[i]->render(dst);
    }

    return 0;
}
