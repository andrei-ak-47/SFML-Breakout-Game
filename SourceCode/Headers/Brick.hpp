#pragma once
#include "INCLUDES.hpp"
class Brick{
    private:
        //sf::RectangleShape brickShape;
        //sf::Color color;
       //sf::Vector2f size;

       sf::Texture texture;
       std::unique_ptr<sf::Sprite> brickShape;

        
        sf::Vector2f position;//Holds X and Y position of brickShape

        //First value is the velocity on X axis
        //Second value is the velocity on Y axis
        sf::Vector2f velocity;

        bool broken = false;//Shows if the block is broken.
    public:
        Brick() = default;//Default Constructer
        Brick(const std::string& fileName, sf::Vector2f p);
        ~Brick() = default;

        void Break() { broken = true;}//break, not part of main game vector anymore

        void Draw(sf::RenderWindow& window);//draw function, render the shape

        /*HELPERS*/
        bool IsBroken() const {return broken;}
        sf::FloatRect GetBounds() const {return brickShape->getGlobalBounds();}
        sf::Vector2f GetPosition() const {return position;}
};