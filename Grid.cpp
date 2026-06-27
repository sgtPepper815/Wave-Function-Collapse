#include "Grid.h"
#include <vector>
#include <set>

Grid::Grid(int w, int h, const std::vector<Tile>& tileSet)
    : width(w), height(h), tiles(tileSet) {

    cells.resize(width * height);

	// init all possible tiles for each cell
    std::set<int> allTiles;
    for (const auto& tile : tiles) {
        allTiles.insert(tile.id);
    }

    for (auto& cell : cells) {
        cell.possibleTiles = allTiles;
    }

    std::random_device rd;
    rng.seed(rd());
}

int Grid::getIndex(int x, int y) const {
    return y * width + x;
}

bool Grid::isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}


bool Grid::isComplete() const {
    for (const auto& cell : cells) {
        if (!cell.collapsed) return false;
    }
    return true;
}

bool Grid::step() {
	// find non-collapsed cell with minimum entropy
    int minEntropy = std::numeric_limits<int>::max();
    std::vector<int> minEntropyIndices;

    for (size_t i = 0; i < cells.size(); ++i) {
        if (!cells[i].collapsed) {
            int entropy = cells[i].getEntropy();

            if (entropy == 0) {
                // start over
                return false;
            }

            if (entropy < minEntropy) {
                minEntropy = entropy;
                minEntropyIndices.clear();
                minEntropyIndices.push_back(i);
            }
            else if (entropy == minEntropy) {
                minEntropyIndices.push_back(i);
            }
        }
    }

    if (minEntropyIndices.empty()) {
        return true;
    }

    // choose a random cell from cells with minimum entropy
    std::uniform_int_distribution<size_t> dist(0, minEntropyIndices.size() - 1);
    int chosenIndex = minEntropyIndices[dist(rng)];

    // collapse the chosen cell
    cells[chosenIndex].collapse(rng);

	// propagate constraints to neighbors
    propagate(chosenIndex % width, chosenIndex / width);

    return true;
}

void Grid::propagate(int startX, int startY) {
    std::vector<std::pair<int, int>> stack;
    stack.push_back({ startX, startY });

    while (!stack.empty()) {
        auto [x, y] = stack.back();
        stack.pop_back();

        
        const int dx[] = { 0, 1, 0, -1 };
        const int dy[] = { -1, 0, 1, 0 };

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (!isValid(nx, ny)) continue;

            int nIndex = getIndex(nx, ny);
            if (cells[nIndex].collapsed) continue;

            
            std::set<int> allowedTiles;
            int currentIndex = getIndex(x, y);

            if (cells[currentIndex].collapsed) {
                int tileId = cells[currentIndex].finalTile;
                for (const auto& tile : tiles) {
                    if (tile.id == tileId) {
                        allowedTiles = tile.allowedNeighbors[dir];
                        break;
                    }
                }
            }
            else {
				// collect allowed neighbors from all possible tiles
                for (int possibleTile : cells[currentIndex].possibleTiles) {
                    for (const auto& tile : tiles) {
                        if (tile.id == possibleTile) {
                            allowedTiles.insert(
                                tile.allowedNeighbors[dir].begin(),
                                tile.allowedNeighbors[dir].end()
                            );
                            break;
                        }
                    }
                }
            }

			// cut down the possible tiles of the neighbor
            std::set<int> newPossible;
            std::set_intersection(
                cells[nIndex].possibleTiles.begin(),
                cells[nIndex].possibleTiles.end(),
                allowedTiles.begin(),
                allowedTiles.end(),
                std::inserter(newPossible, newPossible.begin())
            );

            if (newPossible.size() < cells[nIndex].possibleTiles.size()) {
                cells[nIndex].possibleTiles = newPossible;
                stack.push_back({ nx, ny });
            }
        }
    }
}

void Grid::render(sf::RenderWindow& window, int cellSize) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = getIndex(x, y);
            const Cell& cell = cells[index];

            sf::RectangleShape rect(sf::Vector2f(cellSize - 1.f, cellSize - 1.f));
            rect.setPosition(sf::Vector2f(x * cellSize, y * cellSize));

            if (cell.collapsed) {
                // show final tile 
                for (const auto& tile : tiles) {
                    if (tile.id == cell.finalTile) {
                        rect.setFillColor(tile.color);
                        break;
                    }
                }
            }
            else {
				// show entrophy (darker -> more possibilities)
                float entropy = static_cast<float>(cell.getEntropy()) / tiles.size();
                int _color = static_cast<int>(255 * entropy);
                rect.setFillColor(sf::Color(_color, 0, 0));
            }

            window.draw(rect);
        }
    }
}
