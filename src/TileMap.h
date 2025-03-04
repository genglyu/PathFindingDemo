// in this file we define the TileMap class, which is a class that represents a map of tiles
// the map is a 2D array of tiles, and each tile has a texture and a position

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "ThirdPartyUtility/json.hpp"
#include <filesystem>
#include <map>

// TileMap is a game object that represents a map of tiles, it has a 2D array of tiles, and each tile has a texture and a position

class TileMap : public GameObject 
{
    public:
    TileMap(std::string tileMapName = ""); // TileMap is a game object has no sprite, but multiple children game objects, so no texture is needed
    ~TileMap();

    // The source texture of the tilesets
    sf::Texture tilesetsTexture;
    // Tileset parameters: size, vertical and horizontal count, and spacing
    sf::Vector2u tileSize = {48, 48};
    sf::Vector2u tileCount = {1, 1};

    std::map<std::string, sf::Texture> tilesetTextures;
    std::vector<std::vector<std::vector<std::string>>> tileMap; // layer, row, column of the tile map. The string elements are the name of the remapped tile indices.

    bool loadTileMapData(const std::string& tileMapDataFilepath);
};
