#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "Tile.h"

using namespace std;

class Board {
  public:
    int rows;
    int cols;
    int mines;
    int hiddenTiles;
    vector<vector<Tile*>> board;

    Board(void);
    Board(int rows, int cols, int mines);
    ~Board(void);
    void setBoard(int rows, int cols, int mines);
    void print();
    void setAdjacentTiles(int x, int y);
    void click(Tile* tile, bool isRight);
    void draw(sf::RenderWindow& window, bool debugMode, bool paused);
    void revealMines();
    void flagMines();
    void revealTiles(sf::RenderWindow& window);
};

#endif
