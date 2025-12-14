#pragma once
#include "INCLUDES.hpp"

class Ball {
private:
    // Shape & Appearance
    sf::CircleShape ballShape;
    sf::Color color;
    float radius;

    // Physics
    sf::Vector2f position;   // Current position of the ball
    sf::Vector2f velocity;   // Velocity vector (x = horizontal, y = vertical)
    float SpeedMultiplier = 1.f;

public:
    // Constructors / Destructor
    Ball() = default;
    Ball(sf::Vector2f p, sf::Color c, float r);
    ~Ball() = default;

    // Update & Movement
    void Update(float deltaTime);           // Update ball position based on velocity
    void Move(const sf::Vector2f& delta) { position += delta; }
    void Draw(sf::RenderWindow& window);   // Render the ball

    // Getters
    sf::FloatRect GetBounds() const { return ballShape.getGlobalBounds(); }
    sf::Vector2f GetVelocity() const { return velocity; }
    sf::Vector2f GetPosition() const { return position; }
    float GetSpeedMultiplier() const { return SpeedMultiplier; }

    // Setters
    void SetPosition(sf::Vector2f p) { position = p; }
    void SetSpeedMultiplier(float s) { SpeedMultiplier = s; }
    void MultiplySpeed(float mult) { SpeedMultiplier *= mult; }
    void SetVelocityX(float v) {velocity.x = v;}
    void SetVelocityY(float v) {velocity.y = v;}

    // Random Direction (for game start)
    float RandomFloat(float min, float max);
    void ChooseDirection();

    // Velocity Changes (collision reactions)
    void FlipOnX();
    void FlipOnY();
};
