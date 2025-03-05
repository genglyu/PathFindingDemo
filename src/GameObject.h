#pragma once
// in this file we define the GameObject class
// the GameObject class is a class that represents a game object
// a game object is an entity in the game world, it shall has a position, rotation, scale.
// potientially it can have a texture, parent object, children objects, and other properties
// the game object can be drawn, updated, and destroyed

// it is based on the SFML library. The GameObject class is a wrapper around the sf::Sprite class

#include <SFML/Graphics.hpp>
#include <iostream>



class GameObject
{
    public:
    GameObject(std::string gameObjectName = "new GameObject");
    // GameObject(std::string gameObjectName, const sf::Texture& texture);
    // GameObject(std::string gameObjectName, const sf::Texture& texture, const sf::IntRect& rectangle);
    bool destroy();
    
    std::string name = "";
    // potions, rotations, scales of the game object in the game world
    sf::Vector2f position  = {0, 0};
    sf::Angle rotation = sf::degrees(0.0f);
    sf::Vector2f scale = {1, 1};
    float depth = 0; // the depth of the game object, used for sorting the game objects to draw

    
    // parent object, children objects, and other properties
    GameObject* parent = nullptr;
    std::vector<GameObject*> children = {};

    // sprite of the game object. smart pointer is used to manage the memory of the sprite
    // each game object has only zero or one sprite, which is a 2D image that can be drawn on the screen
    std::unique_ptr<sf::Sprite> sprite = nullptr;
    
    // set the texture of the sprite
    bool setTexture(const sf::Texture& texture);
    bool setTexture(const sf::Texture& texture, const sf::IntRect& cropRectangle);
    // // set the texture of the sprite from a sub-rectangle of a texture
    // bool setTexture(const sf::Texture& texture, const sf::IntRect& rectangle);
    
    // deal with the parent object and children objects
    bool attachToParent(GameObject* parent);
    bool detachFromParent();
    bool addChild(GameObject* child);
    bool removeChild(GameObject* child);

    bool updateSprite();

    void update();
    // void draw(); // Since the GameObject class is a wrapper around the sf::Sprite class and draw loop is managed by main loop, the draw function is not needed here
};