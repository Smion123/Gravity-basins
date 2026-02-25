#pragma once
#include <cmath>
#include <vector>
#include <memory>
class Planet {
public:
    Planet(float w, float x, float y)
        : weight(w), positionX(x), positionY(y),
          velocityX(0.f), velocityY(0.f), damping(0.f) {}

    Planet(float w, float x, float y, float velX, float velY, float damp = 0.f)
        : weight(w), positionX(x), positionY(y),
          velocityX(velX), velocityY(velY), damping(damp) {}

    virtual void updateVelocity(float, float, float);

    virtual void updatePosition(float);

    virtual void checkIfSettled(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets);

    virtual ~Planet() = default;

    float weight;
    float positionX;
    float positionY;
    float lastxForce = 0;
    float lastyForce = 0;
    int idx = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    bool active = true;
    bool IsStaticPlanet = false;
    int endedUpIn = -1;
    float velocityX;
    float velocityY;
    float damping;
};
