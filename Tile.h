#pragma once
#include <SFML/Graphics.hpp>
#include <set>

struct Tile {
    int id;
    sf::Color color;
    std::set<int> allowedNeighbors[4];
};

