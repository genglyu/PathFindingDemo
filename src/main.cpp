#include <SFML/Graphics.hpp> // Looks like this is only a display issue in VSCode. The code intellisense is working fine.
#include <stdio.h>
#include "Game.h"

#include "Utility.hpp"
#include <map>
#include "TileMap.h"

int main()
{
    std::cout << "The executable path is: " << getExecutablePath() << std::endl;
    Game game;

    // // create a game object
    // GameObject* gameObject = new GameObject("testingGameObject");

    // // set the texture of the game object
    // sf::Texture texture;
    // // use excutable path and relative path to load the texture
    // std::string textureImgPath = getExecutablePath() / "assets/tileMap/tilesets/DungeonTilesets.png";
    // texture.loadFromFile(textureImgPath);
    // std::cout << "Texture size: " << texture.getSize().x << ", " << texture.getSize().y << std::endl;

    // std::map<std::string, sf::Texture> tileTextures;
    // tileTextures["sand"] = texture;



    // gameObject->setTexture(tileTextures["sand"]);
    // // // set the position of the game object
    // gameObject->position = {100, 100};

    // // add the game object to the game
    // std::cout << "Adding game object: " << gameObject->name << std::endl;
    // game.addGameObject(gameObject);

    // std::cout << "Game object list size: " << game.gameObjectList.size() << std::endl;


    TileMap* tileMap = new TileMap("testingTileMap");
    std::string tilesetsDataFilepath = getExecutablePath() / "assets/tileMap/tilesets/DungeonTilesets.json";
    tileMap->loadTilesetsData(tilesetsDataFilepath);



    game.run();
    return 0;
}
