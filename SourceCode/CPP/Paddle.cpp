#include "../Headers/Paddle.hpp"

Paddle::Paddle(sf::Vector2f p, sf::Color c, sf::Vector2f s){
    position = p;
    color = c;
    size = s;

    //set the color and size once in constructer, assuming the color and size is static(doesnt change)
    paddleShape.setFillColor(color);
    paddleShape.setSize(size);
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

    position.x += deltaTime * velocity;

    position.x = std::clamp(position.x, 0.f, WINDOW_WIDTH - paddleShape.getSize().x);
}

void Paddle::Draw(sf::RenderWindow& window){
    paddleShape.setPosition(position);
    window.draw(paddleShape);

}