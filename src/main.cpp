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
#include <unistd.h>
//const float dt = 0.1;
// todo 
// variable planet settlement - static planets further, constants can be larger
// potentially lowering resolution where not needed - is pixel not colored? are enough pixels around already one color? fill it
// removing virtual function calls
// optimize calculate planet force
const int width = 2560;
const int height = 1440;
const int fractalScaleMultiplier = 4;
const float G = 10.f;
const float k = 10.f;
const float drag = 0.001;

int pixelBuffer[height + fractalScaleMultiplier][width + fractalScaleMultiplier] = {};

void computeRow(const int y, const std::vector<std::unique_ptr<Planet>>& staticPlanets) {

    (void)!nice(10);

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

    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 1000.f, 454.f));
    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 800.f, 800.f));
    staticPlanets.emplace_back(std::make_unique<StaticPlanet>(500, 1200.f, 800.f));
    all.emplace_back(std::make_unique<Planet>(1, 600, 700, 0.0, 0.0, drag));
    //all.emplace_back(std::make_unique<Planet>(1, 500, 750, 0.0, 0.0, drag));
    //all.emplace_back(std::make_unique<Planet>(1, 550, 650, 0.0, 0.0, drag));
    //all.emplace_back(std::make_unique<Planet>(1, 600, 700, 0.0, 0.0, drag));
    //all.emplace_back(std::make_unique<Planet>(1, 550, 600, 0.0, 0.0, drag));

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

    sf::Color colors[] = { sf::Color::Red, sf::Color::Blue, sf::Color::Green };

    std::cout << "Generating Fractal..." << std::endl;
    sf::Texture fractalTexture;
    
    std::vector<std::future<void>> futures;

    for (int y = 0; y < height; y += fractalScaleMultiplier) {
        futures.push_back(std::async(std::launch::async, &computeRow, y, std::ref(staticPlanets)));
        
    }

    sf::Clock uiUpdateClock;
    sf::Sprite fractalSprite;
    while (window.isOpen() && !futures.empty()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        futures.erase(std::remove_if(futures.begin(), futures.end(), [](std::future<void>& f) {
            return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }), futures.end());

        if (uiUpdateClock.getElapsedTime().asMilliseconds() > 100) {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    int planetIdx = pixelBuffer[j][i];
                    if (planetIdx >= 0 && planetIdx < 3) {
                        fractalImage.setPixel(i, j, colors[planetIdx]);
                    } else {
                        fractalImage.setPixel(i, j, sf::Color::Black);
                    }
                }
            }

            fractalTexture.loadFromImage(fractalImage);
            fractalSprite.setTexture(fractalTexture);

            window.clear();
            window.draw(fractalSprite);

            for (auto& p : staticPlanets) {
                sf::CircleShape shape(10.f);
                shape.setFillColor(sf::Color(p->r, p->g, p->b));
                shape.setPosition(p->positionX, p->positionY);
                window.draw(shape);
            }

            window.display();
            uiUpdateClock.restart();
            
            float progress = 100.f * (1.0f - (static_cast<float>(futures.size()) / (height / fractalScaleMultiplier)));
            std::cout << "Progress: " << progress << "% (" << futures.size() << " threads remaining)" << std::endl;
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int planetIdx = pixelBuffer[j][i];
            if (planetIdx >= 0 && planetIdx < 3) {
                fractalImage.setPixel(i, j, colors[planetIdx]);
            } else {
                fractalImage.setPixel(i, j, sf::Color::Black);
            }
        }
    }
    fractalTexture.loadFromImage(fractalImage);
    fractalSprite.setTexture(fractalTexture);
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

