#include "../Headers/Brick.hpp"

Brick::Brick(sf::Vector2f p, sf::Color c, sf::Vector2f s) :
    position(p),
    color(c),
    size(s){
        
    brickShape.setFillColor(color);
    brickShape.setPosition(position);
    brickShape.setSize(size);
}

void Brick::Draw(sf::RenderWindow& window){
    window.draw(brickShape);
}