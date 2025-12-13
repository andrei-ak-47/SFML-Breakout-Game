#include "Brick.hpp"

Brick::Brick(sf::Vector2f p, sf::Color c, sf::Vector2f s){
    position = p;
    color = c;
    size = s;
}

void Brick::Draw(sf::RenderWindow& window){
    brickShape.setFillColor(color);
    brickShape.setPosition(position);
    brickShape.setSize(size);
    window.draw(brickShape);
}