#include "Planet.hpp"
#include <iostream>
#include <memory>


void Planet::updateVelocity(float fx, float fy, float dt)
{
    velocityX += (fx / weight) * dt;
    velocityY += (fy / weight) * dt;

    velocityX *= (1.f - (damping * dt));
    velocityY *= (1.f - (damping * dt));
}

void Planet::updatePosition(float dt)
{
    positionX += velocityX * dt;
    positionY += velocityY * dt;
}

void Planet::checkIfSettled(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {
    if (sqrt(velocityX*velocityX + velocityY*velocityY) < 5) {
        // enable for cross dynamic planet interractions
        /* 
        for (int i = 0; i < (int)all.size(); i++) {
            if (i == this->idx) {
                continue;
            }
            float differenceOfX = this->positionX-all[i]->positionX;
            float differenceOfY = this->positionY-all[i]->positionY;
            if ((differenceOfX* differenceOfX + differenceOfY * differenceOfY) < 30) {
                this->active = false;
                this->endedUpIn = all[i]->idx;
                return;
            }
        }
        */
        for (int i = 0; i < (int)staticPlanets.size(); i++) {
            float differenceOfX = this->positionX-staticPlanets[i]->positionX;
            float differenceOfY = this->positionY-staticPlanets[i]->positionY;
            if ((sqrt(differenceOfX* differenceOfX + differenceOfY * differenceOfY) < 100)) {
                this->active = false;
                this->endedUpIn = staticPlanets[i]->idx;
                return;
            }
        }
    }
}

