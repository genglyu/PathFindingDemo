#pragma once
// in this file we define the TileMap class, which is a class that represents a map of tiles
// the map is a 2D array of tiles, and each tile has a texture and a position

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "ThirdPartyUtility/json.hpp"
#include <filesystem>
#include <map>



// define a struct to contain the info of one tileset. 
// it has the texture of the tileset, the size of the tiles, maping from tileID to tile rect in the tileset
class TilesetData
{
    public:
        sf::Texture texture;
        sf::Vector2u tileSize;
        std::map<std::string, sf::IntRect> tileRects;
};


// TileMap is a game object that represents a map of tiles, it has a 2D array of tiles, and each tile has a texture and a position

class TileMap : public GameObject 
{
    public:
    TileMap(std::string tileMapName = ""); // TileMap is a game object has no sprite, but multiple children game objects, so no texture is needed
    ~TileMap();

    // The source texture of the tileset
    // sf::Texture tilesetTexture;
    // Tileset parameters: size, vertical and horizontal count, and spacing
    sf::Vector2u tileSize = {48, 48};
    sf::Vector2u tileCount = {1, 1};

    // store the tileset data
    std::map<std::string, TilesetData> tilesetTextureDataDict;
    // 2D array of difficulty values
    // int** difficultyMap;
    std::vector<std::vector<int>> difficultyMap;
    bool loadTileMapData(const std::string& tileMapDataFilepath);
};
