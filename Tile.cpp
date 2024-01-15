#include "Tile.h"
#include "TextureManager.h"
#include <SFML/System/Vector2.hpp>

// Load textures / sprites
TextureManager textureManager;

sf::Sprite tileHidden(textureManager.getTexture("tile_hidden"));
sf::Sprite tileRevealed(textureManager.getTexture("tile_revealed"));
sf::Sprite mine(textureManager.getTexture("mine"));
sf::Sprite flag(textureManager.getTexture("flag"));

sf::Sprite number1(textureManager.getTexture("number_1"));
sf::Sprite number2(textureManager.getTexture("number_2"));
sf::Sprite number3(textureManager.getTexture("number_3"));
sf::Sprite number4(textureManager.getTexture("number_4"));
sf::Sprite number5(textureManager.getTexture("number_5"));
sf::Sprite number6(textureManager.getTexture("number_6"));
sf::Sprite number7(textureManager.getTexture("number_7"));
sf::Sprite number8(textureManager.getTexture("number_8"));

Tile::Tile(void) {
    this->isMine = false;
    this->isHidden = true;
    this->isFlagged = false;
}

int Tile::calculateAdjacentMines() {
    int numMines = 0;

    for (auto tile : neighbors) {
        if (tile->isMine)
            numMines++;
    }

    this->adjacentMines = numMines;
    return numMines;
}

void Tile::draw(sf::RenderWindow& window, float x, float y, bool debugMode,
                bool paused) {
    auto set_draw = [&window, &x, &y](sf::Sprite& sprite) {
        sprite.setPosition(x, y);
        window.draw(sprite);
    };

    if (paused) {
        set_draw(tileRevealed);
        return;
    }

    if (isHidden) {
        set_draw(tileHidden);

        if (isFlagged)
            set_draw(flag);

    } else {
        set_draw(tileRevealed);

        if (isMine)
            set_draw(mine);

        else if (adjacentMines == 1)
            set_draw(number1);
        else if (adjacentMines == 2)
            set_draw(number2);
        else if (adjacentMines == 3)
            set_draw(number3);
        else if (adjacentMines == 4)
            set_draw(number4);
        else if (adjacentMines == 5)
            set_draw(number5);
        else if (adjacentMines == 6)
            set_draw(number6);
        else if (adjacentMines == 7)
            set_draw(number7);
        else if (adjacentMines == 8)
            set_draw(number8);
    }

    if (debugMode && isMine)
        set_draw(mine);
}
