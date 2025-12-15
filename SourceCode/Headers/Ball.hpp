#pragma once
#include "INCLUDES.hpp"

class Ball {
private:
    // Shape & Appearance
    //sf::CircleShape ballShape;
    //sf::Color color;
    //float radius;

    sf::Texture texture;
    std::unique_ptr<sf::Sprite> ballShape;

    // Physics
    sf::Vector2f position;   // Current position of the ball
    sf::Vector2f velocity;   // Velocity vector (x = horizontal, y = vertical)

    std::array<int, 11> BallSpeeds = {BALL_SPEED_INIT, 350, 400, 425, 450, 475, 500, 525, 550, 575, 600};
    //float SpeedMultiplier = 1.f;
    int BallSpeedsIndex = 0;

public:
    // Constructors / Destructor
    Ball() = default;
    Ball(const std::string& fileName, sf::Vector2f p);
    ~Ball() = default;

    // Update & Movement
    void Update(float deltaTime);           // Update ball position based on velocity
    void Move(const sf::Vector2f& delta) { position += delta; }
    void Draw(sf::RenderWindow& window);   // Render the ball
    void IncreaseSpeed();
    void ResetSpeed() {BallSpeedsIndex = 0;}
    // Getters
    sf::FloatRect GetBounds() const { return ballShape->getGlobalBounds(); }
    sf::Vector2f GetVelocity() const { return velocity; }
    sf::Vector2f GetPosition() const { return position; }
    int GetSpeed() const { return BallSpeeds[BallSpeedsIndex]; }

    // Setters
    void SetPosition(sf::Vector2f p) { position = p; }
    //void SetSpeedMultiplier(float s) { SpeedMultiplier = s; }
    //void MultiplySpeed(float mult) { SpeedMultiplier *= mult; }
    void SetVelocityX(float v) {velocity.x = v;}
    void SetVelocityY(float v) {velocity.y = v;}

    // Random Direction (for game start)
    //float RandomFloat(float min, float max);
    //void ChooseDirection();

    // Velocity Changes (collision reactions)
    void FlipOnX();
    void FlipOnY();
};
