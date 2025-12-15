#pragma once
#include "INCLUDES.hpp"
class Brick{
    private:

       sf::Texture texture;
       std::unique_ptr<sf::Sprite> brickShape;

        
        sf::Vector2f position;//Holds X and Y position of brickShape

        //bool broken = false;//Shows if the block is broken.
        BRICK_TYPE BrickType;
    public:
        Brick() = default;//Default Constructer
        Brick(sf::Texture& t, sf::Vector2f p, BRICK_TYPE type);
        ~Brick() = default;

        //void Break() { BrickType = BRICK_TYPE::BROKEN;}//break, not part of main game vector anymore

        void Draw(sf::RenderWindow& window);//draw function, render the shape

        /*HELPERS*/
        //bool IsBroken() const {return BrickType == BRICK_TYPE::BROKEN;}
        sf::FloatRect GetBounds() const {return brickShape->getGlobalBounds();}
        sf::Vector2f GetPosition() const {return position;}
};