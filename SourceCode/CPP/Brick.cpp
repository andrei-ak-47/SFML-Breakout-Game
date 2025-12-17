#include "../Headers/Brick.hpp"


void Brick::Draw(sf::RenderWindow& window){
    window.draw(brickShape);
}

StrongBrick::StrongBrick(sf::Texture& t, sf::Vector2f p, sf::Texture brokenTexture, int hits) : 
    Brick(t, p),
    BrokenTexture(brokenTexture),
    HitsLeft(hits){

}

bool StrongBrick::OnHit(){
    if(--HitsLeft <= 0){
        return true;
    }
    brickShape.setTexture(BrokenTexture);
    return false;
}