#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

class Tile {
  private:
    sf::Sprite sprite;

  public:
    bool isMine;
    bool isHidden;
    bool isFlagged;
    int adjacentMines;
    vector<Tile*> neighbors;

    Tile(void);
    int calculateAdjacentMines();
    void draw(sf::RenderWindow& window, float x, float y, bool debugMode,
              bool paused);
};

#endif
