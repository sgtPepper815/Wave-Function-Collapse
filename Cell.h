#pragma once
#include <set>
#include <random>


class Cell {
public:
    std::set<int> possibleTiles;
    bool collapsed;
    int finalTile;

    Cell() : collapsed(false), finalTile(-1) {}

    int getEntropy() const {
        return static_cast<int>(possibleTiles.size());
    }

    void collapse(std::mt19937& rng) {
        if (collapsed || possibleTiles.empty()) return;

        std::vector<int> tiles(possibleTiles.begin(), possibleTiles.end());
        std::uniform_int_distribution<int> dist(0, tiles.size() - 1);
        finalTile = tiles[dist(rng)];
        collapsed = true;
        possibleTiles.clear();
    }
};

