#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Board.h"

using namespace std;

void Board::setAdjacentTiles(int x, int y) {
    for (int i = max(0, x - 1); i <= min(rows - 1, x + 1); i++) {
        for (int j = max(0, y - 1); j <= min(cols - 1, y + 1); j++) {
            if (i == x && j == y) {
                continue;
            }

            board[x][y]->neighbors.push_back(board[i][j]);
        }
    }
}

Board::Board(void) {
    this->rows = 0;
    this->cols = 0;
    this->mines = 0;
    this->hiddenTiles = 0;
}

Board::Board(int rows, int cols, int mines) {
    this->rows = rows;
    this->cols = cols;
    this->mines = mines;
    this->hiddenTiles = rows * cols;

    // Resize board to proper dimensions
    board.resize(rows, std::vector<Tile*>(cols, nullptr));

    // Populate board w/ tiles
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = new Tile();
        }
    }

    // Populate board w/ mines
    int temp_mines = mines;

    srand(static_cast<unsigned>(time(0)));

    while (temp_mines) {
        int x = rand() % rows;
        int y = rand() % cols;

        if (!board[x][y]->isMine) {
            board[x][y]->isMine = true;
            temp_mines--;
        }
    }

    // Calculate each tile's neighbors
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            setAdjacentTiles(i, j);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            board[i][j]->calculateAdjacentMines();
    }
}

Board::~Board(void) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete board[i][j];
        }
    }
}

void Board::setBoard(int rows, int cols, int mines) {
    this->rows = rows;
    this->cols = cols;
    this->mines = mines;
    this->hiddenTiles = rows * cols;

    // Resize board to proper dimensions
    board.resize(rows, std::vector<Tile*>(cols, nullptr));

    // Populate board w/ tiles
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = new Tile();
        }
    }

    // Populate board w/ mines
    int temp_mines = mines;

    srand(static_cast<unsigned>(time(0)));

    while (temp_mines) {
        int x = rand() % rows;
        int y = rand() % cols;

        if (!board[x][y]->isMine) {
            board[x][y]->isMine = true;
            temp_mines--;
        }
    }

    // Calculate each tile's neighbors
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            setAdjacentTiles(i, j);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            board[i][j]->calculateAdjacentMines();
    }
}

void Board::print() {
    cout << "Rows: " << rows << endl;
    cout << "Columns: " << cols << endl;
    cout << "Mines: " << mines << endl;

    cout << "Board:" << endl;
    for (auto row : board) {
        for (auto col : row) {
            cout << col->isMine << " ";
        }
        cout << endl;
    }
}

void Board::click(Tile* tile, bool rightClick) {
    if (!tile->isHidden)
        return;

    if (rightClick) {
        if (tile->isFlagged)
            mines++;
        else
            mines--;

        tile->isFlagged = !tile->isFlagged;

    } else if (!tile->isFlagged) {
        tile->isHidden = false;
        hiddenTiles--;

        for (auto* neighbor : tile->neighbors) {
            if (!tile->adjacentMines && !tile->isMine)
                click(neighbor, false);
        }
    }
}

void Board::draw(sf::RenderWindow& window, bool debugMode, bool paused) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            board[i][j]->draw(window, i * 32, j * 32, debugMode, paused);
    }
}

void Board::revealMines() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j]->isMine)
                board[i][j]->isHidden = false;
        }
    }
}

void Board::flagMines() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j]->isMine) {
                board[i][j]->isFlagged = true;
                mines--;
            }
        }
    }
}

void Board::revealTiles(sf::RenderWindow& window) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            board[i][j]->draw(window, i * 32, j * 32, false, true);
    }

    window.display();
}
