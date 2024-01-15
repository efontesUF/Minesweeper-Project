#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>
#include <fstream>

#include "screens.h"

int main() {
    // ====================================
    // ==== Read data from config file ====
    // ====================================
    std::ifstream config("files/board_config.cfg");
    int colCount, rowCount, mineCount;
    config >> colCount >> rowCount >> mineCount;
    config.close();

    // =================
    // ==== Windows ====
    // =================
    screen_welcome Welcome;
    screen_game Game;
    screen_leaderboard Leaderboard;
    sf::RenderWindow welcomeWindow;
    sf::RenderWindow gameWindow;
    sf::RenderWindow leaderboardWindow;

    int screen = 0;
    while (screen >= 0) {
        switch (screen) {
        case 0: {
            welcomeWindow.create(
                sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Welcome",
                sf::Style::Close);
            screen = Welcome.Run(welcomeWindow);

            break;
        }
        case 1: {
            if (!gameWindow.isOpen()) {
                gameWindow.create(
                    sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Game",
                    sf::Style::Close);
            }

            Game.username = Welcome.username;

            gameWindow.setFramerateLimit(60);
            if (Game.reset)
                Game.Initialize();
            Game.reset = true;
            screen = Game.Run(gameWindow);

            break;
        }
        case 2: {
            leaderboardWindow.create(
                sf::VideoMode(colCount * 16, rowCount * 16 + 50), "Leaderboard",
                sf::Style::Close);

            Leaderboard.usernameIndex = Game.usernameIndex;

            screen = Leaderboard.Run(leaderboardWindow);

            break;
        }

        default:
            break;
        }
    }

    return EXIT_SUCCESS;
}
