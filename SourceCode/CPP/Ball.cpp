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

float Ball::RandomFloat(float min, float max){
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

void Ball::ChooseDirection(){
    float angleDeg;
    float rand = RandomFloat(0.f, 1.f); // random number between 0 and 1
    if(rand < 0.5f){
        angleDeg = RandomFloat(-60.f, -20.f); // left side
    } else {
        angleDeg = RandomFloat(20.f, 60.f);  // right side
    }
    float angleRad = angleDeg * M_PI / 180.f;//Convert to Radians for sin/cos functions
    
    velocity.x = BALL_SPEED_INIT * std::sin(angleRad) * SpeedMultiplier;//Set initial velocity for that random diretion, while maintaining correct speed
    velocity.y = BALL_SPEED_INIT * std::cos(angleRad) * SpeedMultiplier;//Set initial velocity for that random diretion, while maintaining correct speed
}
