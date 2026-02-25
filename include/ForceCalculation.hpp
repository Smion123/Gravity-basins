#pragma once
#include <vector>
#include <memory>
#include "Planet.hpp"

void calculateNewPlanetForce(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets);
void applyPlanetVelocity(std::vector<std::unique_ptr<Planet>>& all, const std::vector<std::unique_ptr<Planet>>& staticPlanets, int iterations);