#include "Cell.h"
#include "Tile.h"
#include "Grid.h"
#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <limits>

int main() {
    const int gridWidth = 20;
    const int gridHeight = 20;
    const int cellSize = 30;

    std::vector<Tile> tiles;

    // Tile 0: gras
    Tile grass;
    grass.id = 0;
    grass.color = sf::Color(34, 139, 34);
    grass.allowedNeighbors[0] = { 0, 2, 3 }; // up
    grass.allowedNeighbors[1] = { 0, 2, 3 }; // right
    grass.allowedNeighbors[2] = { 0, 2, 3 }; // down
    grass.allowedNeighbors[3] = { 0, 2, 3 }; // left
    tiles.push_back(grass);

    // Tile 1: water
    Tile water;
    water.id = 1;
    water.color = sf::Color(30, 144, 255);
    water.allowedNeighbors[0] = { 1, 2 }; // up
    water.allowedNeighbors[1] = { 1, 2 }; // right
    water.allowedNeighbors[2] = { 1, 2 }; // down
    water.allowedNeighbors[3] = { 1, 2 }; // left
    tiles.push_back(water);

    // Tile 2: sand
    Tile sand;
    sand.id = 2;
    sand.color = sf::Color(238, 214, 175);
    sand.allowedNeighbors[0] = { 0, 1, 2 }; // up
    sand.allowedNeighbors[1] = { 0, 1, 2 }; // right
    sand.allowedNeighbors[2] = { 0, 1, 2 }; // down
    sand.allowedNeighbors[3] = { 0, 1, 2 }; // left
    tiles.push_back(sand);

    // Tile 3: highland
    Tile highland;
    highland.id = 3;
    highland.color = sf::Color(111, 111, 111);
    highland.allowedNeighbors[0] = { 0, 3, 4 }; // up
    highland.allowedNeighbors[1] = { 0, 3, 4 }; // right
    highland.allowedNeighbors[2] = { 0, 3, 4 }; // down
    highland.allowedNeighbors[3] = { 0, 3, 4 }; // left
    tiles.push_back(highland);

    // Tile 4: mountain
    Tile mountain;
    mountain.id = 4;
    mountain.color = sf::Color(55, 55, 55);
    mountain.allowedNeighbors[0] = { 3, 4 }; // up
    mountain.allowedNeighbors[1] = { 3, 4 }; // right
    mountain.allowedNeighbors[2] = { 3, 4 }; // down
    mountain.allowedNeighbors[3] = { 3, 4 }; // left
    tiles.push_back(mountain);

    // create sfml window
    sf::RenderWindow window(
        sf::VideoMode({ gridWidth * cellSize, gridHeight * cellSize }),
        "Wave Function Collapse"
    );

    Grid grid(gridWidth, gridHeight, tiles);

    bool autoStep = false;
    sf::Clock clock;
    float stepDelay = 0.1f; // seconds between auto steps

    // main loop
    while (window.isOpen()) {
        
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto* kp = ev->getIf<sf::Event::KeyPressed>()) {
                if (kp->scancode == sf::Keyboard::Scancode::Space) {
                    // one step
                    if (!grid.isComplete()) {
                        grid.step();
                    }
                }
                if (kp->scancode == sf::Keyboard::Scancode::A) {
                    // toggle auto step
                    autoStep = !autoStep;
                }
                if (kp->scancode == sf::Keyboard::Scancode::R) {
                    // restart
                    grid = Grid(gridWidth, gridHeight, tiles);
                    autoStep = false;
                }
            }
        }

		// auto step if enabled and not complete
        if (autoStep && !grid.isComplete() && clock.getElapsedTime().asSeconds() > stepDelay) {
            if (!grid.step()) {
                grid = Grid(gridWidth, gridHeight, tiles);
            }
            clock.restart();
        }

        window.clear(sf::Color::Black);
        grid.render(window, cellSize);
        window.display();
    }

    return 0;
}