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

    // // it has the texture of the tileset, the size of the tiles, maping from tileID to tile rect in the tileset
    // // struct tilesetData
    // // {
    // //     sf::Texture texture;
    // //     sf::Vector2u tileSize;
    // //     std::map<std::string, sf::IntRect> tileRects;
    // // };

    // TilesetData testingTilesetData;
    // testingTilesetData.texture = texture;
    // testingTilesetData.tileSize = {48, 48};
    // testingTilesetData.tileRects["sand"] = sf::IntRect(0, 0, 48, 48);




    // // gameObject->setTexture(tileTextures["sand"]);
    // gameObject->setTexture(texture);
    // // // set the position of the game object
    // gameObject->position = {50, 50};

    // // gameObject->sprite->setPosition({50,50});

    // // add the game object to the game
    // std::cout << "Adding game object: " << gameObject->name << std::endl;
    // game.addGameObject(gameObject);

    // std::cout << "Game object list size: " << game.gameObjectList.size() << std::endl;


    TileMap* tileMap = new TileMap("testingTileMap");
    std::string tileMapDataFilepath = getExecutablePath() / "assets/tileMap/TestingMap1.json";
    tileMap->loadTileMapData(tileMapDataFilepath);
    game.addGameObject(tileMap);

    // print the game object list, print the game object name
    std::cout << "Game object list size: " << game.gameObjectList.size() << std::endl;
    for (auto gameObject : game.gameObjectList)
    {
        std::cout << "Game object name: " << gameObject->name << std::endl;
    }

    // check if the gameObjects hold the data.
    std::cout << "TileMap: " << tileMap->name << std::endl;
    std::cout << tileMap->tilesetTextureDataDict.size() << std::endl;
    for (auto& [key, value] : tileMap->tilesetTextureDataDict)
    {
        std::cout << "TilesetName: " << key << std::endl;
        std::cout << "TilesetTexture: " << value.texture.getSize().x << ", " << value.texture.getSize().y << std::endl;
        std::cout << "TileSize: " << value.tileSize.x << ", " << value.tileSize.y << std::endl;
        std::cout << "TileRects: " << value.tileRects.size() << std::endl;
    }




    game.run();
    return 0;
}
