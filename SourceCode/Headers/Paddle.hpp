#pragma once
#include "INCLUDES.hpp"

class Paddle{
    private:

        sf::Sprite paddleShape;

        
        float velocity = 0;//Not sf::vector2f because only moves in one dimension (X axis)

        //sf::Vector2f position;//X and Y positions of the paddleShape
    public:
        Paddle() = default;//Default Constructer
        Paddle(sf::Texture& texture);
        ~Paddle() = default;

        void MoveRight();//changes velocity to positive
        void MoveLeft();//changes velocity to negative
        void Update(float deltaTime);//updates position of paddle
        void Draw(sf::RenderWindow& window);//renders paddle shape

        void SetPosition(sf::Vector2f p) {paddleShape.setPosition(p);}

        /*HELPERS*/
        void SetVelocityX(float v){velocity = v;}
        sf::FloatRect GetBounds() const {return paddleShape.getGlobalBounds();}
};