#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include "Planet.hpp"
#include "StaticPlanet.hpp"
#include "ForceCalculation.hpp"
#include <iostream>
const float dt_mult = 20;

const float G = 10.f;
const float k = 10.f;

void calculateNewPlanetForce(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {

    for (auto& p : all) {
        p->lastxForce = 0;
        p->lastyForce = 0;
    }
    for (size_t i = 0; i < all.size(); i++) {
        for (size_t k = 0; k < staticPlanets.size(); k++) {
            if (!all[i]->active || !staticPlanets[k]->active) {
                continue;
            }

            float xDelta = staticPlanets[k]->positionX - all[i]->positionX;
            float yDelta = staticPlanets[k]->positionY - all[i]->positionY;

            float r2 = xDelta * xDelta + yDelta * yDelta;
            float weightMult = all[i]->weight * staticPlanets[k]->weight;

            if (r2 < 100) {
                float r = sqrt(r2);

                float forceCommon = (G * weightMult) / 100;

                float distMult = r / 10;

                forceCommon *= distMult;
            }


            float forceCommon = (G * weightMult) / r2;

            float fx = forceCommon * xDelta;
            float fy = forceCommon * yDelta;

            all[i]->lastxForce += fx;
            all[i]->lastyForce += fy;
        }

        
        for (size_t j = i + 1; j < all.size(); j++) {
            if (!all[i]->active || !all[j]->active) {
                continue;
            }
            float xDelta = staticPlanets[k]->positionX - all[i]->positionX;
            float yDelta = staticPlanets[k]->positionY - all[i]->positionY;

            float r2 = xDelta * xDelta + yDelta * yDelta;
            float weightMult = all[i]->weight * staticPlanets[k]->weight;

            if (r2 < 100) {
                float r = sqrt(r2);

                float forceCommon = (G * weightMult) / 100;

                float distMult = r / 10;

                forceCommon *= distMult;
            }


            float forceCommon = (G * weightMult) / r2;

            float fx = forceCommon * xDelta;
            float fy = forceCommon * yDelta;

            all[i]->lastxForce += fx;
            all[i]->lastyForce += fy;

            all[j]->lastxForce += -fx;
            all[j]->lastyForce += -fy;
        }
        
    }

}

/* old, approximates inverse square law, but too resource intensive
float k4 = k * k * k * k;
void calculateNewPlanetForce(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {

    for (auto& p : all) {
        p->lastxForce = 0;
        p->lastyForce = 0;
    }
    for (size_t i = 0; i < all.size(); i++) {
        for (size_t k = 0; k < staticPlanets.size(); k++) {
            if (!all[i]->active || !staticPlanets[k]->active) {
                continue;
            }
            float xDelta = staticPlanets[k]->positionX - all[i]->positionX;
            float yDelta = staticPlanets[k]->positionY - all[i]->positionY;

            float r2 = xDelta * xDelta + yDelta * yDelta;
            float r4 = r2 * r2;
            float weightMult = all[i]->weight * staticPlanets[k]->weight;
            
            float denBase = r4 + k4;
            float denominator = denBase * pow(denBase, 0.25);

            float forceCommon = (G * weightMult * r2) / denominator;
            float fx = forceCommon * xDelta;
            float fy = forceCommon * yDelta;

            all[i]->lastxForce += fx;
            all[i]->lastyForce += fy;
        }

        
        for (size_t j = i + 1; j < all.size(); j++) {
            if (!all[i]->active || !all[j]->active) {
                continue;
            }
            float xDelta = all[j]->positionX - all[i]->positionX;
            float yDelta = all[j]->positionY - all[i]->positionY;

            float r2 = xDelta * xDelta + yDelta * yDelta;
            float r4 = r2 * r2;
            
            float weightMult = all[i]->weight * all[j]->weight;
            
            float denBase = r4 + k4;
            float denominator = denBase * pow(denBase, 0.25);

            float forceCommon = (G * weightMult * r2) / denominator;
            float fx = forceCommon * xDelta;
            float fy = forceCommon * yDelta;

            all[i]->lastxForce += fx;
            all[i]->lastyForce += fy;

            all[j]->lastxForce += -fx;
            all[j]->lastyForce += -fy;
        }
        
    }

}
*/
float calculateCurrentDT(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {
    float lowest = 100000000;
    for (size_t j = 0; j < all.size(); j++) {
        for (size_t i = 0; i < staticPlanets.size(); i++) {
            float Xdist = (all[j]->positionX - staticPlanets[i]->positionX);
            float Ydist = (all[j]->positionY - staticPlanets[i]->positionY);
            float dist = sqrt(Xdist*Xdist + Ydist*Ydist);
            
            if (lowest > dist) {
                lowest = dist;
            }

            if (lowest < 10) {
                lowest = 10;
            }
        }
    }
    return ((lowest * sqrt(lowest)) / 10000) * dt_mult;
}
// 0,00131
void applyPlanetVelocity(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets, int iterations) {

    float dt = calculateCurrentDT(all, staticPlanets);
    for (int i = all.size() - 1; i >= 0; i--) {
        if (!all[i]->active) {
            continue;
        }
        all[i]->updateVelocity(all[i]->lastxForce / 2, all[i]->lastyForce / 2, dt);
    }

    for (int i = all.size() - 1; i >= 0; i--) {
        if (!all[i]->active) {
            continue;
        }
        all[i]->updatePosition(dt);
    }
    calculateNewPlanetForce(all, staticPlanets);
    
    for (int i = all.size() - 1; i >= 0; i--) {
        if (!all[i]->active) {
            continue;
        }
        all[i]->updateVelocity(all[i]->lastxForce / 2, all[i]->lastyForce / 2, dt);
    }
    if (iterations % 100 == 0) {
        for (int i = all.size() - 1; i >= 0; i--) {
            if (!all[i]->active) {
                continue;
            }

            all[i]->checkIfSettled(all, staticPlanets);
        }
    }

}