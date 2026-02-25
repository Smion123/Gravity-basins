#pragma once
#include "Planet.hpp"
#include <memory>
#include <vector>

class StaticPlanet : public Planet {
public:
    StaticPlanet(float w, float x, float y)
        : Planet(w, x, y) {IsStaticPlanet = true;}
    
    void updateVelocity(float, float, float) override;
    void updatePosition(float) override;
    void checkIfSettled(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets) override {};
};
