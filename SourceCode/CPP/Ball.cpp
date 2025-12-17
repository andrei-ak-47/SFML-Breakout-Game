#include "../Headers/Ball.hpp"

Ball::Ball(sf::Texture& texture) :
    ballShape(texture){
}

void Ball::Update(float deltaTime){
    ballShape.move(velocity * deltaTime);
}

void Ball::FlipOnX(){
    velocity.x *= -1.f;
}

void Ball::FlipOnY(){
    velocity.y *= -1.f;
}

void Ball::Draw(sf::RenderWindow& window){
    window.draw(ballShape);
}

void Ball::IncreaseSpeed() {
    if (BallSpeedsIndex + 1 < BallSpeeds.size()) BallSpeedsIndex++;
}

