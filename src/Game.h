// This is the main frame of the application.
// It creates a window and runs a loop that polls events and clears the window.
// The window is displayed at the end of each loop iteration.
// The update and draw functions shall handle all the gameobjects involved in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameObject.h"


class Game
{
    public:
    Game();
    ~Game();

    // game initialization, set the window size, framerate, title, etc.
    void initialize(unsigned int windowWidth = 48u * 15, 
                    unsigned int windowHeight = 48u * 11, 
                    unsigned int frameRate = 60u, 
                    std::string windowTitle = "Path finding demo",
                    bool cursorVisible = true);

    // game loop
    sf::RenderWindow* window;
    void update();
    void draw();
    void run();

    // game world management
    sf::View view;

    // game object management
    std::vector<GameObject*> gameObjectList = {};

    bool addGameObject(GameObject* gameObject);
    bool removeGameObject(GameObject* gameObject);
    bool destroyGameObject(GameObject* gameObject);
};