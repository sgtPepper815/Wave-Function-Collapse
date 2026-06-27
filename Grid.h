#include "Tile.h"
#include "Cell.h"
#include <vector>

// WFC Grid 
class Grid {
private:
    int width, height;
    std::vector<Cell> cells;
    std::vector<Tile> tiles;
    std::mt19937 rng;

    int getIndex(int x, int y) const;

    bool isValid(int x, int y) const;

public:
    Grid(int w, int h, const std::vector<Tile>& tileSet);

    bool isComplete() const;

    bool step();
    void propagate(int startX, int startY);
    void render(sf::RenderWindow& window, int cellSize) const;
};