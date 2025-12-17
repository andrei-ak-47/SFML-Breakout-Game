#pragma once
#include "INCLUDES.hpp"
class Brick{
    protected:

       sf::Sprite brickShape;

        
        //sf::Vector2f position;//Holds X and Y position of brickShape

    public:
        Brick() = default;//Default Constructer
        //Brick(sf::Texture& t, sf::Vector2f p);
        public:
        Brick(sf::Texture& texture, sf::Vector2f pos) : brickShape(texture){ brickShape.setPosition(pos); }
        ~Brick() = default;

        void Draw(sf::RenderWindow& window);//draw function, render the shape

        virtual bool OnHit() = 0;//different types of bricks have different(return true if broken, false if not)
        virtual void Update(float deltaTime) {};//MovingBrick updates
        /*HELPERS*/
        sf::FloatRect GetBounds() const {return brickShape.getGlobalBounds();}
        sf::Vector2f GetPosition() const {return brickShape.getPosition();}
};
class StandardBrick : public Brick{
    private:

    public:
        StandardBrick(sf::Texture& t, sf::Vector2f p) : Brick(t, p) {}
        bool OnHit() override {return true;};

};
class StrongBrick : public Brick{
    private:
        int HitsLeft;
        sf::Texture BrokenTexture;
    public:
        StrongBrick(sf::Texture& t, sf::Vector2f p, sf::Texture brokenTexture, int hits);
        bool OnHit() override;
};

