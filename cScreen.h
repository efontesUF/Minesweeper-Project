#pragma once

#include <SFML/Graphics.hpp>

class cScreen {
  public:
    virtual int Run(sf::RenderWindow& App) = 0;
    void setText(sf::Text& text, float x, float y);
};
