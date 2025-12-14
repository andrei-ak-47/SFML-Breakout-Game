#include "../Headers/Paddle.hpp"

Paddle::Paddle(const std::string& fileName, sf::Vector2f p)
    : texture(),                  // default construct texture
      position(p){

    if (!texture.loadFromFile(fileName)) {
        std::cout << "[ERROR]: Failed to load Ball texture\n";
    }
    paddleShape = std::make_unique<sf::Sprite>(sf::Sprite(texture));
    paddleShape->setPosition(position);
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

    position.x = std::clamp(position.x, 0.f, static_cast<float>(WINDOW_WIDTH - PADDLE_WIDTH));

    paddleShape->setPosition(position);
}

void Paddle::Draw(sf::RenderWindow& window){
    window.draw(*paddleShape);

}