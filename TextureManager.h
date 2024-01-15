#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace std;

class TextureManager {
    static unordered_map<string, sf::Texture> textures;

  public:
    static sf::Texture& getTexture(string textureName);
};
