#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include "Planet.hpp"
#include "StaticPlanet.hpp"
#include <iostream>
#include "ForceCalculation.hpp"
#include <bits/stdc++.h>
#include <thread>
//const float dt = 0.1;

const int width = 2560;
const int height = 1440;
const int fractalScaleMultiplier = 4;
const float G = 10.f;
const float k = 10.f;
const float drag = 0.002;

int pixelBuffer[height + fractalScaleMultiplier][width + fractalScaleMultiplier] = {};

/*
void computeRowsRange(int yStart, int yEnd, const std::vector<std::unique_ptr<Planet>>& staticPlanets)
{
    for (int y = yStart; y < yEnd; y += fractalScaleMultiplier) {
        computeRow(y, staticPlanets);
    }
}
*/
void computeRow(const int y, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {
    std::vector<std::unique_ptr<Planet>> all;

    all.emplace_back(std::make_unique<Planet>(1, 0.0, 0.0, 0.0, 0.0, drag));

    for (int x = 0; x < width; x = x + fractalScaleMultiplier) {

        all[0]->positionX = x;
        all[0]->positionY = y;
        all[0]->velocityX = 0;
        all[0]->velocityY = 0;
        all[0]->lastxForce = 0;
        all[0]->lastyForce = 0;
        all[0]->active = true;
        all[0]->endedUpIn = -1;
        
        int iterations = 0;

        // Simulate this one particle until it hits a static planet or exits
        while (all[0]->active && iterations < 50000) {
            applyPlanetVelocity(all, staticPlanets, iterations);
            iterations++;
        }

        // 4. Color the pixel based on which planet it hit
        if (all[0]->endedUpIn != -1) {
            for (int i = 0; i < fractalScaleMultiplier; i++) {
                for (int j = 0; j < fractalScaleMultiplier; j++) {
                    pixelBuffer[y+j][x+i] = all[0]->endedUpIn;
                }
            }
        }
    }
}

int main() {



    for (int x = 0; x < height; ++x)
        for (int y = 0; y < width; ++y)
            pixelBuffer[x][y] = -1;


    std::vector<std::unique_ptr<Planet>> staticPlanets;
    std::vector<std::unique_ptr<Planet>> all;

    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 800.f, 450.f));
    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 800.f, 800.f));
    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 1200.f, 450.f));
    all.emplace_back(std::make_unique<Planet>(1, 300, 370, 0.0, 0.0, drag));
    //all.emplace_back(std::make_unique<Planet>(50, 700.0, 450.0, 0.0, 0.0, 0.001));

    sf::RenderWindow window(sf::VideoMode(width, height), "Gravity Basins");
    window.setFramerateLimit(165);
    calculateNewPlanetForce(all, staticPlanets);

    for (size_t i = 0; i < all.size(); i++) {
        all[i]->r = (250 + 13 * i) % 155 + 100;
        all[i]->g = (0 + 97 * i) % 255;
        all[i]->b = (200 + 53 * i) % 255;
        all[i]->idx = i;
    }

    for (size_t i = 0; i < staticPlanets.size(); i++) {
        staticPlanets[i]->r = (250 + 13 * i) % 155 + 100;
        staticPlanets[i]->g = (0 + 97 * i) % 255;
        staticPlanets[i]->b = (100 + 53 * i) % 255;
        staticPlanets[i]->idx = i;
    }
    /*
    int i = 0;
    while (window.isOpen()) {
        i++;
        if (i < 50000000 && all[0]->active) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            applyPlanetVelocity(all, staticPlanets, i);
            window.clear(sf::Color::Black);

            for (size_t i = 0; i < all.size(); i++) {
                if (!all[i]->active) {
                    continue;
                }
                sf::CircleShape shape(10.f);
                shape.setFillColor(sf::Color(
                    all[i]->r,
                    all[i]->g,
                    all[i]->b));
                shape.setPosition(all[i]->positionX, all[i]->positionY);
                window.draw(shape);
            }

            for (size_t i = 0; i < staticPlanets.size(); i++) {
                if (!staticPlanets[i]->active) {
                    continue;
                }
                sf::CircleShape shape(10.f);
                shape.setFillColor(sf::Color(
                    staticPlanets[i]->r,
                    staticPlanets[i]->g,
                    staticPlanets[i]->b));
                shape.setPosition(staticPlanets[i]->positionX, staticPlanets[i]->positionY);
                window.draw(shape);
            }

            window.display();
        } else {
            break;
            //window.close();
        }
    }*/
    

    sf::Image fractalImage;
    fractalImage.create(width, height, sf::Color::Black);

    // 2. Define colors for your 3 Static Planets
    sf::Color colors[] = { sf::Color::Red, sf::Color::Blue, sf::Color::Green };

    std::cout << "Generating Fractal..." << std::endl;
    sf::Texture fractalTexture;
    // 3. Loop through every pixel
    
    for (int y = 0; y < height; y = y + fractalScaleMultiplier) {
        
        computeRow(y, staticPlanets);
        /*
        for (int x = 0; x < width; x = x + fractalScaleMultiplier) {

            all[0]->positionX = x;
            all[0]->positionY = y;
            all[0]->velocityX = 0;
            all[0]->velocityY = 0;
            all[0]->lastxForce = 0;
            all[0]->lastyForce = 0;
            all[0]->active = true;
            all[0]->endedUpIn = -1;
            
            bool settled = false;
            int iterations = 0;

            // Simulate this one particle until it hits a static planet or exits
            while (!settled && iterations < 50000) {
                applyPlanetVelocity(all, staticPlanets, iterations);
                iterations++;
            }

            // 4. Color the pixel based on which planet it hit
            if (all[0]->endedUpIn != -1) {
                for (int i = 0; i < fractalScaleMultiplier; i++) {
                    for (int j = 0; j < fractalScaleMultiplier; j++) {
                        fractalImage.setPixel(x + i, y + j, colors[all[0]->endedUpIn]);
                    }
                }
            }
        }
        */
        // Print progress every 10 rows
        std::cout << "Progress: " << (y / 1440.0f) * 100 << "%" << std::endl;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                fractalImage.setPixel(i, j, colors[pixelBuffer[j][i]]);
            }
        }

        fractalTexture.loadFromImage(fractalImage);
        sf::Sprite fractalSprite(fractalTexture);


        window.clear();
        window.draw(fractalSprite);

        for (size_t i = 0; i < staticPlanets.size(); i++) {
            if (!staticPlanets[i]->active) {
                continue;
            }
            sf::CircleShape shape(10.f);
            shape.setFillColor(sf::Color(
                staticPlanets[i]->r,
                staticPlanets[i]->g,
                staticPlanets[i]->b));
            shape.setPosition(staticPlanets[i]->positionX, staticPlanets[i]->positionY);
            window.draw(shape);
        }
        window.display();
    }

    // 5. Display the result

    fractalTexture.loadFromImage(fractalImage);
    sf::Sprite fractalSprite(fractalTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(fractalSprite);

        for (size_t i = 0; i < staticPlanets.size(); i++) {
            if (!staticPlanets[i]->active) {
                continue;
            }
            sf::CircleShape shape(10.f);
            shape.setFillColor(sf::Color(
                staticPlanets[i]->r,
                staticPlanets[i]->g,
                staticPlanets[i]->b));
            shape.setPosition(staticPlanets[i]->positionX, staticPlanets[i]->positionY);
            window.draw(shape);
        }

        window.display();
    }


    return 0;
}

