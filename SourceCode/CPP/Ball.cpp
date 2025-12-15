#include "../Headers/Ball.hpp"

Ball::Ball(const std::string& fileName, sf::Vector2f p)
    : texture(),                  // default construct texture
      position(p){

    if (!texture.loadFromFile(fileName)) {
        std::cout << "[ERROR]: Failed to load Ball texture\n";
    }
    ballShape = std::make_unique<sf::Sprite>(sf::Sprite(texture));
    ballShape->setPosition(position);
}

void Ball::Update(float deltaTime){
    /*
        Updates the X and Y coordinate of the Ball using X and Y Velocity and deltaTime

    */

    position.x += deltaTime * velocity.x;
    position.y += deltaTime * velocity.y;
    ballShape->setPosition(position);
}

void Ball::FlipOnX(){
    velocity.x *= -1.f;
}

void Ball::FlipOnY(){
    velocity.y *= -1.f;
}

void Ball::Draw(sf::RenderWindow& window){
    window.draw(*ballShape);
}

void Ball::IncreaseSpeed() {
    if (BallSpeedsIndex + 1 < BallSpeeds.size()) BallSpeedsIndex++;
}

