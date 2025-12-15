#include "../Headers/Brick.hpp"

Brick::Brick(sf::Texture& t, sf::Vector2f p, BRICK_TYPE type) :
    texture(t),                  // default construct texture
    position(p),
    BrickType(type){

    brickShape = std::make_unique<sf::Sprite>(sf::Sprite(texture));
    brickShape->setPosition(position);
}

void Brick::Draw(sf::RenderWindow& window){
    window.draw(*brickShape);
}