#pragma once
#include "INCLUDES.hpp"

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
class Game{
    private:
        /*Essetials*/
        sf::RenderWindow window;
        sf::Clock clock;

        /*Text*/
        sf::Text scoreText;
        sf::Text LivesText;
        sf::Text SpeedText;
        sf::Font GlobalFont;

        /*Sound*/

        sf::SoundBuffer brickBuffer;
        sf::Sound brickSound;

        sf::SoundBuffer paddleBuffer;
        sf::Sound paddleSound;

        sf::SoundBuffer wallBuffer;
        sf::Sound wallSound;

        sf::SoundBuffer diedBuffer;
        sf::Sound diedSound;
        
        /*Objects*/
        std::unique_ptr<Ball> ball;
        std::unique_ptr<Paddle> paddle;
        std::vector<std::unique_ptr<Brick>> bricks;

        /*Textures*/
        sf::Texture BallTexture;
        sf::Texture PaddleTexture;
        std::array<sf::Texture, 4> StandardBrickTextures;
        std::array<sf::Texture, 4> StrongBrickTextures;
        std::array<sf::Texture, 4> StrongBrickBrokenTextures;
        /*Values*/
        int score;
        int lives; 
        int CurrentLvl;

        /*Levels*/
        std::vector<std::string> LevelPaths;

        /*Game State*/
        GAME_STATE GameState = GAME_STATE::Start;
        
        void LoadLevel(int LevelID);//Restores bricks for the next level if 0 left
        void ResetPositions();//Resets positions for paddle, ball, and calculates direction and velocity/X and Y Vectors for the ball

        void Render();//calls draw for everything
        void CheckCollisions();//checks collisions FOR EVERYTHING

        void LoadFiles();
    public:
        Game();
        ~Game() = default;

        void run();
};