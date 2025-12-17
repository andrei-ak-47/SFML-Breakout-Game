#pragma once
#include "INCLUDES.hpp"

class Ball {
private:

    //sf::Texture texture;
    sf::Sprite ballShape;

    // Physics
    sf::Vector2f velocity;   // Velocity vector (x = horizontal, y = vertical)

    std::array<int, 11> BallSpeeds = {BALL_SPEED_INIT, 350, 400, 425, 450, 475, 500, 525, 550, 575, 600};
    int BallSpeedsIndex = 0;

public:
    // Constructors / Destructor
    Ball() = default;
    Ball(sf::Texture& texture);
    ~Ball() = default;

    // Update & Movement
    void Update(float deltaTime);           // Update ball position based on velocity
    void Move(const sf::Vector2f& delta) { ballShape.setPosition(ballShape.getPosition() + delta); }
    void Draw(sf::RenderWindow& window);   // Render the ball
    void IncreaseSpeed();
    void ResetSpeed() {BallSpeedsIndex = 0;}
    // Getters
    sf::FloatRect GetBounds() const { return ballShape.getGlobalBounds(); }
    sf::Vector2f GetVelocity() const { return velocity; }
    sf::Vector2f GetPosition() const { return ballShape.getPosition(); }
    int GetSpeed() const { return BallSpeeds[BallSpeedsIndex]; }

    // Setters
    void SetPosition(sf::Vector2f p) { ballShape.setPosition(p); }
    void SetVelocityX(float v) {velocity.x = v;}
    void SetVelocityY(float v) {velocity.y = v;}

    // Velocity Changes (collision reactions)
    void FlipOnX();
    void FlipOnY();
};
