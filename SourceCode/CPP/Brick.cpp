#include "../Headers/Brick.hpp"

Brick::Brick(const std::string& fileName, sf::Vector2f p)
    : texture(),                  // default construct texture
      position(p){

    if (!texture.loadFromFile(fileName)) {
        std::cout << "[ERROR]: Failed to load Ball texture\n";
    }
    brickShape = std::make_unique<sf::Sprite>(sf::Sprite(texture));
    brickShape->setPosition(position);
}

void Brick::Draw(sf::RenderWindow& window){
    window.draw(*brickShape);
}