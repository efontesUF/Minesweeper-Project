#include "screen_welcome.h"
#include "cScreen.h"

#include <SFML/Graphics.hpp>

screen_welcome::screen_welcome(void) {}

int screen_welcome::Run(sf::RenderWindow& App) {
    bool Running = true;
    sf::Event Event;
    sf::Font Font;
    sf::String playerInput = "";
    sf::Text welcome("WELCOME TO MINESWEEPER!", Font, 24);
    sf::Text entername("Enter your name:", Font, 20);
    sf::Text username("|", Font, 18);

    Font.loadFromFile("files/font.ttf");

    welcome.setFillColor(sf::Color::White);
    setText(welcome, (float)App.getSize().x / 2,
            (float)App.getSize().y / 2 - 150);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);

    entername.setFillColor(sf::Color::White);
    setText(entername, (float)App.getSize().x / 2,
            (float)App.getSize().y / 2 - 75);
    entername.setStyle(sf::Text::Bold);

    username.setFillColor(sf::Color::Yellow);
    setText(username, (float)App.getSize().x / 2,
            (float)App.getSize().y / 2 - 45);
    username.setStyle(sf::Text::Bold);

    while (Running) {
        // Process events
        while (App.pollEvent(Event)) {
            switch (Event.type) {
            // Close window
            case sf::Event::Closed:
                App.close();
                return (-1);
                break;

            // Key pressed
            case sf::Event::KeyPressed:
                switch (Event.key.code) {
                case sf::Keyboard::Return:
                    if (!playerInput.isEmpty()) {
                        playerInput.erase(playerInput.getSize());
                        this->username = playerInput;
                        App.close();
                        return 1;
                    }
                    break;

                case sf::Keyboard::Backspace:
                    if (!playerInput.isEmpty()) {
                        playerInput.erase(playerInput.getSize() - 1, 1);
                        username.setString(playerInput + "|");
                        setText(username, (float)App.getSize().x / 2,
                                (float)App.getSize().y / 2 - 45);
                    }
                    break;

                default:
                    break;
                }
                break;

            // Text entered
            case sf::Event::TextEntered:
                if (std::isalpha(Event.text.unicode) &&
                    playerInput.getSize() < 10) {
                    if (playerInput.isEmpty())
                        playerInput +=
                            static_cast<char>(std::toupper(Event.text.unicode));
                    else
                        playerInput +=
                            static_cast<char>(std::tolower(Event.text.unicode));

                    username.setString(playerInput + "|");
                    setText(username, (float)App.getSize().x / 2,
                            (float)App.getSize().y / 2 - 45);
                }
                break;

            // Don't process other events
            default:
                break;
            }
        }

        App.clear(sf::Color::Blue);

        App.draw(welcome);
        App.draw(entername);
        App.draw(username);

        App.display();
    }
}
