#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>

using namespace std;

class screen_leaderboard : public cScreen {
  public:
    int usernameIndex;
    screen_leaderboard(void);
    virtual int Run(sf::RenderWindow& App);
};
