//
#include "Game.h"

Game::Game()
{
    initialize();
}
Game::~Game()
{
    for (auto gameObject : gameObjectList)
    {
        delete gameObject;
    }
    delete window;
}

void Game::initialize(unsigned int windowWidth, 
                      unsigned int windowHeight, 
                      unsigned int frameRate, 
                      std::string windowTitle, 
                      bool cursorVisible)
{
    window = new sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), windowTitle);
    window->setFramerateLimit(frameRate);
    window->setMouseCursorVisible(false);
    window->setMouseCursorVisible(cursorVisible);
}

void Game::update()
{
    // std::cout << "Updating game objects" << std::endl;
    std::sort(gameObjectList.begin(), gameObjectList.end(), [](GameObject* a, GameObject* b) { return a->depth < b->depth; });
    // std::cout << "Game object list sorted, size: " << gameObjectList.size() << std::endl;

    for (auto gameObject : gameObjectList)
    {
        gameObject->update();
    }
    // sort the game objects by depth
}

void Game::draw()
{
    // std::cout << "Drawing game objects" << std::endl;
    window->clear();
    for (auto gameObject : gameObjectList)
    {
        if (gameObject->sprite != nullptr)
        {
            window->draw(*gameObject->sprite);
        }
    }
    window->display();
}

void Game::run()
{
    // main game loop. It runs until the window is closed. Need to have a separate thread for the game loop.

    std::cout << "Game loop started" << std::endl;

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
        }
        update();
        draw();
    }
}



bool Game::addGameObject(GameObject* gameObject)
{
    // All children game objects should be added to the game object list too, and a duplication check should be performed.
    // We can make use of this function recursively to add all the children game objects
    try{
        // check if gameObjects is nullptr
        if (gameObject == nullptr)
        {
            std::cout << "Game object is nullptr" << std::endl;
            return false;
        }
        // duplication check
        auto it = std::find(gameObjectList.begin(), gameObjectList.end(), gameObject);
        if (it != gameObjectList.end())
        {
            std::cout << "Game object: " << gameObject->name << " already exists in the game object list" << std::endl;
            return true;
        }
        gameObjectList.push_back(gameObject);
        // add all the children game objects
        for (auto child : gameObject->children)
        {
            addGameObject(child);
        }
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool Game::removeGameObject(GameObject* gameObject)
{
    // we should remove all the children game objects too
    // We can make use of this function recursively to remove all the children game objects
    try{
        // check if gameObjects is nullptr
        if (gameObject == nullptr)
        {
            std::cout << "Game object is nullptr" << std::endl;
            return false;
        }
        // if there are duplicate game objects, remove them all.
        gameObjectList.erase(std::remove(gameObjectList.begin(), gameObjectList.end(), gameObject), gameObjectList.end());
        // remove all the children game objects
        for (auto child : gameObject->children)
        {
            removeGameObject(child);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool Game::destroyGameObject(GameObject* gameObject)
{
    try{
        removeGameObject(gameObject);
        gameObject->destroy();
        delete gameObject;
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}
