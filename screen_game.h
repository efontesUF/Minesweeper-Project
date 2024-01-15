#include "Board.h"
#include "TextureManager.h"
#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <fstream>
#include <iostream>

class screen_game : public cScreen {
  public:
    bool paused;
    bool victory;
    bool defeat;
    bool debugMode;
    bool paused_temp;
    bool reset;

    // Initialize time
    chrono::time_point<chrono::high_resolution_clock,
                       chrono::duration<long long int, ratio<1, 1000000000>>>
        startTime;
    chrono::time_point<chrono::high_resolution_clock,
                       chrono::duration<long long int, ratio<1, 1000000000>>>
        pauseTime;
    chrono::seconds gameDuration;
    int elapsedPauseTime;
    int totalTime;
    int minutes;
    int seconds;

    // Read data from config file
    ifstream config;
    int colCount, rowCount, mineCount;

    // Load textures / sprites
    TextureManager textureManager;

    sf::Sprite faceHappy;
    sf::Sprite faceLose;
    sf::Sprite faceWin;
    sf::Sprite debug;
    sf::Sprite pause;
    sf::Sprite play;
    sf::Sprite leaderboard;
    sf::Sprite digits;

    map<int, sf::Sprite> digitMap;

    // Create game board
    Board gameBoard;
    bool leaderboardOpen;

    int usernameIndex;
    string username;

    screen_game(void);
    void Initialize();
    virtual int Run(sf::RenderWindow& App);
};
