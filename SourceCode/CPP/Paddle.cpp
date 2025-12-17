#include "../Headers/Paddle.hpp"

Paddle::Paddle(sf::Texture& texture)
    : paddleShape(texture){
}

void Paddle::MoveRight(){
    /*
        Changes the velocity of paddle to be positive
    */
    velocity = PADDLE_SPEED;
}

void Paddle::MoveLeft(){
    /*
        Changes the velocity of paddle to be negative
    */
    velocity = -PADDLE_SPEED;
}

void Paddle::Update(float deltaTime){
    /*
        Simply updates the position using delta time and curr velocity
        fixes boundry problems using std::clamp
    */

    float PositionX = paddleShape.getPosition().x;

    PositionX = std::clamp(PositionX, 0.f, static_cast<float>(WINDOW_WIDTH - PADDLE_WIDTH));

    paddleShape.setPosition({PositionX += deltaTime * velocity, paddleShape.getPosition().y});
}

void Paddle::Draw(sf::RenderWindow& window){
    window.draw(paddleShape);

}