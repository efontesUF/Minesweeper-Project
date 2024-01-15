#include "screen_game.h"
#include "Board.h"
#include "TextureManager.h"
#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

// Drawing functions
void drawCounter(sf::RenderWindow& window, Board& board,
                 map<int, sf::Sprite>& digits) {
    int num_mines = board.mines;
    int rows = board.cols;
    sf::Sprite container_left = digits.at(0);
    sf::Sprite container_middle = digits.at(0);
    sf::Sprite container_right = digits.at(0);

    if (board.mines < 0) {
        num_mines *= -1;
        digits.at(10).setPosition(12, 32 * (rows + 0.5) + 16);
        window.draw(digits.at(10));
    }

    container_left = digits.at(num_mines / 10 / 10 % 10);
    container_middle = digits.at(num_mines / 10 % 10);
    container_right = digits.at(num_mines % 10);

    container_left.setPosition(33, 32 * (rows + 0.5) + 16);
    container_middle.setPosition(33 + 21, 32 * (rows + 0.5) + 16);
    container_right.setPosition(33 + 21 + 21, 32 * (rows + 0.5) + 16);

    window.draw(container_left);
    window.draw(container_middle);
    window.draw(container_right);
}

void drawTimer(sf::RenderWindow& window, Board& board,
               map<int, sf::Sprite>& digits, int minutes = 0, int seconds = 0) {
    int rows = board.cols;
    int cols = board.rows;

    int minutes0 = minutes / 10 % 10;
    int minutes1 = minutes % 10;
    int seconds0 = seconds / 10 % 10;
    int seconds1 = seconds % 10;

    sf::Sprite timer_minutes0 = digits.at(minutes0);
    sf::Sprite timer_minutes1 = digits.at(minutes1);
    sf::Sprite timer_seconds0 = digits.at(seconds0);
    sf::Sprite timer_seconds1 = digits.at(seconds1);

    timer_minutes0.setPosition((cols * 32) - 97, 32 * (rows + 0.5) + 16);
    timer_minutes1.setPosition((cols * 32) - 97 + 21, 32 * (rows + 0.5) + 16);
    timer_seconds0.setPosition((cols * 32) - 54, 32 * (rows + 0.5) + 16);
    timer_seconds1.setPosition((cols * 32) - 54 + 21, 32 * (rows + 0.5) + 16);

    window.draw(timer_minutes0);
    window.draw(timer_minutes1);
    window.draw(timer_seconds0);
    window.draw(timer_seconds1);
}

// Leaderboard functions
struct LeaderboardEntry {
    int minutes;
    int seconds;
    string username;
};

bool compareEntries(LeaderboardEntry& entry1, LeaderboardEntry& entry2) {
    if (entry1.minutes != entry2.minutes)
        return entry1.minutes < entry2.minutes;
    else
        return entry1.seconds < entry2.seconds;
}

int addToLeaderboard(int minutes, int seconds, string username) {
    vector<LeaderboardEntry> leaderboard;

    // Read from leaderboard file
    ifstream leaderboardFileIn("files/leaderboard.txt");
    string line;
    while (getline(leaderboardFileIn, line)) {
        istringstream entryLine(line);
        LeaderboardEntry entry;
        char colon;
        char comma;
        entryLine >> entry.minutes >> colon >> entry.seconds >> comma >>
            entry.username;
        leaderboard.push_back(entry);
    }
    leaderboardFileIn.close();

    // Add the new entry
    LeaderboardEntry newEntry{minutes, seconds, username};
    leaderboard.push_back(newEntry);

    // Sort leaderboard
    sort(leaderboard.begin(), leaderboard.end(), compareEntries);

    // Trim to 5 entries
    if (leaderboard.size() > 5)
        leaderboard.resize(5);

    // Write leaderboard to leaderboard file
    ofstream leaderboardFileOut("files/leaderboard.txt");
    for (const auto& entry : leaderboard) {
        leaderboardFileOut << setfill('0') << setw(2) << entry.minutes << ":"
                           << setfill('0') << setw(2) << entry.seconds << ","
                           << entry.username << endl;
    }
    leaderboardFileOut.close();

    int index = 0;
    while (leaderboard[index].username != username ||
           leaderboard[index].seconds != seconds ||
           leaderboard[index].minutes != minutes)
        index++;

    return index;
}

// screen_game functions
screen_game::screen_game(void) {
    paused = false;
    victory = false;
    defeat = false;
    debugMode = false;
    reset = true;

    // Read data from config file
    config.open("files/board_config.cfg");
    config >> colCount >> rowCount >> mineCount;
    config.close();

    minutes = 0;
    seconds = 0;

    faceHappy.setTexture(textureManager.getTexture("face_happy"));
    faceLose.setTexture(textureManager.getTexture("face_lose"));
    faceWin.setTexture(textureManager.getTexture("face_win"));
    debug.setTexture(textureManager.getTexture("debug"));
    pause.setTexture(textureManager.getTexture("pause"));
    play.setTexture(textureManager.getTexture("play"));
    leaderboard.setTexture(textureManager.getTexture("leaderboard"));
    digits.setTexture(textureManager.getTexture("digits"));

    faceHappy.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    faceLose.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    faceWin.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    debug.setPosition(colCount * 32 - 304, 32 * (rowCount + 0.5));
    pause.setPosition(colCount * 32 - 240, 32 * (rowCount + 0.5));
    play.setPosition(colCount * 32 - 240, 32 * (rowCount + 0.5));
    leaderboard.setPosition(colCount * 32 - 176, 32 * (rowCount + 0.5));

    for (int i = 0; i < 11; i++) {
        sf::IntRect rect(i * 21, 0, 21, 32);
        digits.setTextureRect(rect);
        digitMap.emplace(i, digits);
    }

    gameBoard.setBoard(colCount, rowCount, mineCount);
}

void screen_game::Initialize() {
    paused = false;
    victory = false;
    defeat = false;
    debugMode = false;
    reset = true;

    // Initialize time
    startTime = chrono::high_resolution_clock::now();
    pauseTime = chrono::high_resolution_clock::now();
    elapsedPauseTime = chrono::duration_cast<chrono::seconds>(
                           chrono::high_resolution_clock::now() - pauseTime)
                           .count();
    gameDuration = chrono::duration_cast<chrono::seconds>(
        chrono::high_resolution_clock::now() - startTime);
    totalTime = gameDuration.count();

    // Read data from config file
    config.open("files/board_config.cfg");
    config >> colCount >> rowCount >> mineCount;
    config.close();

    faceHappy.setTexture(textureManager.getTexture("face_happy"));
    faceLose.setTexture(textureManager.getTexture("face_lose"));
    faceWin.setTexture(textureManager.getTexture("face_win"));
    debug.setTexture(textureManager.getTexture("debug"));
    pause.setTexture(textureManager.getTexture("pause"));
    play.setTexture(textureManager.getTexture("play"));
    leaderboard.setTexture(textureManager.getTexture("leaderboard"));
    digits.setTexture(textureManager.getTexture("digits"));

    faceHappy.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    faceLose.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    faceWin.setPosition((float)colCount / 2 * 32 - 32, 32 * (rowCount + 0.5));
    debug.setPosition(colCount * 32 - 304, 32 * (rowCount + 0.5));
    pause.setPosition(colCount * 32 - 240, 32 * (rowCount + 0.5));
    play.setPosition(colCount * 32 - 240, 32 * (rowCount + 0.5));
    leaderboard.setPosition(colCount * 32 - 176, 32 * (rowCount + 0.5));

    for (int i = 0; i < 11; i++) {
        sf::IntRect rect(i * 21, 0, 21, 32);
        digits.setTextureRect(rect);
        digitMap.emplace(i, digits);
    }

    gameBoard.setBoard(colCount, rowCount, mineCount);
}

int screen_game::Run(sf::RenderWindow& App) {
    // Check if exited leaderboard
    if (leaderboardOpen) {
        auto unpausedTime = chrono::high_resolution_clock::now();
        elapsedPauseTime +=
            chrono::duration_cast<chrono::seconds>(unpausedTime - pauseTime)
                .count();
        paused = paused_temp;
        leaderboardOpen = false;
    }

    // Draw tiles
    App.clear(sf::Color::White);
    gameBoard.draw(App, false, false);
    App.display();

    while (App.isOpen()) {
        sf::Event Event;

        // Process events
        while (!leaderboardOpen && App.pollEvent(Event)) {
            switch (Event.type) {
            // Close window
            case sf::Event::Closed: {
                App.close();
                return (-1);
                break;
            }

            // Mouse press
            case sf::Event::MouseButtonPressed: {
                // Tile clicks
                if (!victory && !defeat && !paused &&
                    Event.mouseButton.y <= rowCount * 32) {
                    int tile_x = Event.mouseButton.x / 32;
                    int tile_y = Event.mouseButton.y / 32;

                    if (Event.mouseButton.button == sf::Mouse::Right) {
                        gameBoard.click(gameBoard.board[tile_x][tile_y], true);

                    } else if (Event.mouseButton.button == sf::Mouse::Left) {
                        gameBoard.click(gameBoard.board[tile_x][tile_y], false);

                        // Check for defeat
                        if (gameBoard.board[tile_x][tile_y]->isMine) {
                            defeat = true;
                            paused = true;
                            debugMode = false;
                            gameBoard.revealMines();
                        }

                        // Check for victory
                        if (!defeat &&
                            gameBoard.hiddenTiles == gameBoard.mines) {
                            victory = true;
                            debugMode = false;
                            gameBoard.flagMines();

                            usernameIndex =
                                addToLeaderboard(minutes, seconds, username);

                            paused_temp = paused;
                            paused = true;
                            pauseTime = chrono::high_resolution_clock::now();

                            leaderboardOpen = true;
                            reset = false;

                            gameBoard.draw(App, false, false);
                            App.display();

                            return 2;
                        }
                    }
                }

                // Debug button
                if (!victory && !defeat &&
                    Event.mouseButton.button == sf::Mouse::Left &&
                    debug.getGlobalBounds().contains(Event.mouseButton.x,
                                                     Event.mouseButton.y))
                    debugMode = !debugMode;

                // Pause button
                if (!victory && !defeat &&
                    Event.mouseButton.button == sf::Mouse::Left &&
                    pause.getGlobalBounds().contains(Event.mouseButton.x,
                                                     Event.mouseButton.y)) {
                    paused = !paused;

                    if (paused)
                        pauseTime = chrono::high_resolution_clock::now();
                    else {
                        auto unpausedTime =
                            chrono::high_resolution_clock::now();
                        elapsedPauseTime +=
                            chrono::duration_cast<chrono::seconds>(
                                unpausedTime - pauseTime)
                                .count();
                    }
                }

                // Face button
                if (Event.mouseButton.button == sf::Mouse::Left &&
                    faceHappy.getGlobalBounds().contains(Event.mouseButton.x,
                                                         Event.mouseButton.y))
                    Initialize();

                // Leaderboard button
                if (Event.mouseButton.button == sf::Mouse::Left &&
                    leaderboard.getGlobalBounds().contains(
                        Event.mouseButton.x, Event.mouseButton.y)) {
                    paused_temp = paused;
                    paused = true;
                    pauseTime = chrono::high_resolution_clock::now();

                    gameBoard.revealTiles(App);

                    leaderboardOpen = true;
                    reset = false;

                    return 2;
                }

                break;
            }

            default:
                break;
            }
        }

        App.clear(sf::Color::White);

        gameBoard.draw(App, debugMode, paused && !defeat);
        drawCounter(App, gameBoard, digitMap);

        // Draw timer
        gameDuration = chrono::duration_cast<chrono::seconds>(
            chrono::high_resolution_clock::now() - startTime);
        totalTime = gameDuration.count();

        if (!paused) {
            totalTime -= elapsedPauseTime;
            minutes = totalTime / 60;
            seconds = totalTime % 60;
        }

        drawTimer(App, gameBoard, digitMap, minutes, seconds);

        // Draw face bttn
        if (defeat)
            App.draw(faceLose);
        else if (victory)
            App.draw(faceWin);
        else
            App.draw(faceHappy);

        // Draw pause/play bttn
        if (paused)
            App.draw(play);
        else
            App.draw(pause);

        App.draw(debug);
        App.draw(leaderboard);

        App.display();
    }
}
