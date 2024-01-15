#include "screen_leaderboard.h"
#include "cScreen.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct LeaderboardEntry {
    int minutes;
    int seconds;
    string username;
};

vector<LeaderboardEntry> readLeaderboard() {
    vector<LeaderboardEntry> leaderboard;

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

    return leaderboard;
}

screen_leaderboard::screen_leaderboard(void) {}

int screen_leaderboard::Run(sf::RenderWindow& App) {
    sf::Event Event;
    sf::Font Font;
    sf::Text Leaderboard("LEADERBOARD", Font, 20);
    sf::Text Content("", Font, 18);

    Font.loadFromFile("files/font.ttf");

    Leaderboard.setFillColor(sf::Color::White);
    Leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(Leaderboard, (float)App.getSize().x / 2,
            (float)App.getSize().y / 2 - 120);

    Content.setFillColor(sf::Color::White);
    Content.setStyle(sf::Text::Bold);

    vector<LeaderboardEntry> leaderboard = readLeaderboard();
    for (int i = 0; i < leaderboard.size(); i++) {
        string temp_string = Content.getString();
        string minutes = (!(leaderboard[i].minutes / 10 % 10))
                             ? ("0" + to_string(leaderboard[i].minutes))
                             : to_string(leaderboard[i].minutes);
        string seconds = (!(leaderboard[i].seconds / 10 % 10))
                             ? ("0" + to_string(leaderboard[i].seconds))
                             : to_string(leaderboard[i].seconds);

        temp_string = temp_string + "\n\n" + to_string(i + 1) + ".\t" +
                      minutes + ":" + seconds + "\t" +
                      ((i == usernameIndex) ? (leaderboard[i].username + "*")
                                            : leaderboard[i].username);

        Content.setString(temp_string);
    }
    setText(Content, (float)App.getSize().x / 2,
            (float)App.getSize().y / 2 + 20);

    while (App.isOpen()) {
        while (App.pollEvent(Event)) {
            switch (Event.type) {
            case sf::Event::Closed: {
                App.close();
                return 1;
                break;
            }

            default:
                break;
            }
        }

        App.clear(sf::Color::Blue);

        App.draw(Leaderboard);
        App.draw(Content);

        App.display();
    }
}
