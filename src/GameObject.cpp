#include "GameObject.h"

GameObject::GameObject(std::string gameObjectName)
{
    name = gameObjectName;
}

// GameObject::GameObject(std::string gameObjectName, const sf::Texture& texture)
// {
//     name = gameObjectName;
//     setTexture(texture);
// }
// GameObject::GameObject(std::string gameObjectName, const sf::Texture& texture, const sf::IntRect& rectangle)
// {
//     name = gameObjectName;
//     setTexture(texture, rectangle);
// }
bool GameObject::destroy()
{
    // destroy the sprite
    sprite.reset();
    // destroy the children game objects
    for (auto child : children)
    {
        child->destroy();
    }
    // detach from the parent game object
    if (parent != nullptr)
    {
        detachFromParent();
    }
    return true;
}


bool GameObject::setTexture(const sf::Texture& texture)
{
    std::cout << "Setting texture for game object: " << name << std::endl;

    if (sprite == nullptr)
    {
        sprite = std::make_unique<sf::Sprite>(texture);
        return true;
    }
    else
    {
        sprite->setTexture(texture);
        return true;
    }
}

// bool GameObject::setTexture(const sf::Texture& texture, const sf::IntRect& rectangle)
// {
//     if (sprite == nullptr)
//     {
//         sprite = std::make_unique<sf::Sprite>(texture, rectangle);
//         return true;
//     }
//     else
//     {
//         sprite->setTexture(texture);
//         sprite->setTextureRect(rectangle);
//         return true;
//     }
// }

bool GameObject::attachToParent(GameObject* parent)
{
    if (this->parent != nullptr)
    {
        detachFromParent();
    }
    this->parent = parent;
    parent->addChild(this);
    return true;
}

bool GameObject::detachFromParent()
{
    if (this->parent != nullptr)
    {
        this->parent->removeChild(this);
        this->parent = nullptr;
        return true;
    }
    return false;
}

bool GameObject::addChild(GameObject* child)
{
    children.push_back(child);
    return true;
}

bool GameObject::removeChild(GameObject* child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        children.erase(it);
        return true;
    }
    return false;
}

bool GameObject::updateSprite()
{
    // if the sprite is not created, return false
    if (sprite == nullptr)
    {
        return false;
        std::cout << "GameObject: " << name << " has no sprite" << std::endl;
    }
    // set the position, rotation, scale of the sprite
    // trace back to the root parent object, and calculate the final position, rotation, scale
    // usually, the order should be scale, rotation, position.
    // And we can use the SFML Sprite class's setScale, setRotation, setPosition functions to set the position, rotation, scale of the sprite

    // trace back to the root parent object, and calculate the final position, rotation, scale
    sprite->setPosition(position);
    sprite->setRotation(rotation);
    sprite->setScale(scale);

    GameObject* parentObject = this -> parent;
    while (parentObject != nullptr)
    {
        sprite->scale(parentObject->scale);
        sprite->rotate(parentObject->rotation);
        sprite->move(parentObject->position);
        parentObject = parentObject->parent;
    }
    return true;
}


void GameObject::update()
{
    // since all the game objects are updated in the game loop, it is not necessary to update the children game objects here
    updateSprite();
}