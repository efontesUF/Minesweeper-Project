#include "cScreen.h"

#include <SFML/Graphics.hpp>

class screen_welcome : public cScreen {
  public:
    sf::String username;
    screen_welcome(void);
    virtual int Run(sf::RenderWindow& App);
};
