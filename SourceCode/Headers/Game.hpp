#pragma once
#include "INCLUDES.hpp"

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
class Game{
    private:
        sf::RenderWindow window;
        sf::Clock clock;

        sf::Text scoreText;
        sf::Text LivesText;
        sf::Text SpeedText;
        sf::Font GlobalFont;

        sf::SoundBuffer brickBuffer;
        sf::Sound brickSound;

        sf::SoundBuffer paddleBuffer;
        sf::Sound paddleSound;

        sf::SoundBuffer wallBuffer;
        sf::Sound wallSound;

        sf::SoundBuffer diedBuffer;
        sf::Sound diedSound;
        
        Ball ball;
        Paddle paddle;
        std::vector<std::vector<Brick>> bricks;

        int score;
        int lives; 
        int bricksNum;

        GAME_STATE GameState = GAME_STATE::Start;

        void Bricks_init();//Restores the bricks on screen if 0 left
        
        void ResetPositions();//Resets positions for paddle, ball, and calculates direction and velocity/X and Y Vectors for the ball

        void Render();//calls draw for everything
        void CheckCollisions();//checks collisions FOR EVERYTHING
    public:
        Game();
        ~Game() = default;

        void run();
};