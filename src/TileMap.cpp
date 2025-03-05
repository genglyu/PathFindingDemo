#include "TileMap.h"
#include <fstream>
#include <ranges>

TileMap::TileMap(std::string tileMapName)
{
    name = tileMapName;
}

TileMap::~TileMap()
{
    destroy();
}

bool TileMap::loadTileMapData(const std::string& tileMapDataFilepath)
{
    // load the tile map data from a json file
    // the json file should have the following structure:
    // {
    //     "tileTexture": {
    //         "tilesetName": "relativePathTotilesetDataFile.json"
    //     },
    //     "indexMaping": {
    //         "tileID": ["tilesetName", "tileName"]
    //     },
    //     "tileSize": {
    //         "width": 48,
    //         "height": 48
    //     },
    //     "mapLayout": [
    //         [
    //             ["tileID", "tileID"],   
    //             ["tileID", "tileID"]
    //         ]
    //     ],
    //     "difficultyMap": [
    //         [1,  1],
    //         [1,  4]    
    //     ]    
    // }

    // the tilesetDataFile.json should have the following structure:
    // {
    //     "tilesetImg": "DungeonTilesets.png",
    //     "tileSize": {
    //         "width": 48,
    //         "height": 48
    //     },
    //     "tileLayout": [
    //         ["sand", "rock"], 
    //         ["lava", "water"]
    //     ]
    // }

    
    try {
        // load json file from the filepath
        std::cout << "Attempting to load tile map data from file: " << tileMapDataFilepath << std::endl;
        std::ifstream tileMapDataFile(tileMapDataFilepath);

        // print the loaded json file tileMapDataFile in terminal
        // std::string line;
        // while (std::getline(tileMapDataFile, line)) {
        //     std::cout << line << std::endl;  // Print each line
        // }

        // process the loaded json file
        nlohmann::json tileMapData;
        tileMapDataFile >> tileMapData;

        // close the file
        tileMapDataFile.close();

        std::cout << "Successfully loaded tile map data from file: " << tileMapDataFilepath << std::endl;

        // get the directory path of the tileMapDataFile
        std::filesystem::path tileMapDataFileDirectoryPath = std::filesystem::path(tileMapDataFilepath).parent_path();
        // print the json file in terminal
        // std::cout << tileMapData.dump(4) << std::endl;

        // go through the "tileTexture" in the all related tileset files. Load the tileset data from json and process them
        for (auto& [tilesetName, tilesetDataFileRelativePath] : tileMapData["tileTexture"].items())
        {
            std::cout << "Tileset: " << tilesetName << ", tilesetDataFileRelativePath: " << tilesetDataFileRelativePath << std::endl;
            // create a new tilesetData object
            TilesetData tilesetTextureData;

            // load the tileset data from json
            std::string tilesetDataFilepath = (tileMapDataFileDirectoryPath / tilesetDataFileRelativePath).string();
            std::ifstream tilesetDataFile(tilesetDataFilepath);
            nlohmann::json tilesetDataFromJson;
            tilesetDataFile >> tilesetDataFromJson;

            std::cout << "Loaded tileset data from file: " << tilesetDataFilepath << std::endl;

            // get the tileset files directory path
            std::filesystem::path tilesetDataFileDirectoryPath = std::filesystem::path(tilesetDataFilepath).parent_path();
            

            // sf::Texture tilesetTexture;
            std::string tilesetTextureFilepath = (tilesetDataFileDirectoryPath / tilesetDataFromJson["tilesetImg"]).string();
            
            if (tilesetTextureData.texture.loadFromFile(tilesetTextureFilepath))
            {
                //std::cout << "Loaded tileset texture: " << tilesetTextureFilepath << std::endl;
            }
            else
            {
                std::cerr << "Failed to load tileset texture: " << tilesetTextureFilepath << std::endl;
                return false;
            }

            std::cout << "Loaded tileset texture: " << tilesetName << std::endl << "from file: " << tilesetTextureFilepath << std::endl;

            // store the tile size
            tilesetTextureData.tileSize = {tilesetDataFromJson["tileSize"]["width"], tilesetDataFromJson["tileSize"]["height"]};
            // store the tile rects. The tile layout tilesetDataFromJson["tileLayout"] is a 2D array, 
            // so we need to go through the tilesets by rows and columns and calculate the tile rects
            int tileCountX = 0;
            for (auto& row : tilesetDataFromJson["tileLayout"])
            {
                int tileCountY = 0;
                for (auto& tileName : row)
                {

                    sf::IntRect tileRect(sf::Vector2i(tileCountY * tilesetTextureData.tileSize.x, tileCountX * tilesetTextureData.tileSize.y), 
                                         sf::Vector2i(tilesetTextureData.tileSize.x, tilesetTextureData.tileSize.y));
                    tilesetTextureData.tileRects[tileName] = tileRect;
                    tileCountY++;
                }
                tileCountX++;
            }
            // add the tilesetTextureData to the tilesetTextureDataDict
            tilesetTextureDataDict[tilesetName] = tilesetTextureData;
        }

        std::cout << "Loaded all tileset textures, and related data" << std::endl;

        // go through the "mapLayout" in the tileMapData and create game objects for each tile.
        // The mapLayout is a 3D array, so we need to go through the layers, rows and columns to create the game objects

        std::cout << "Creating game objects for each tile in the mapLayout" << std::endl;
        for (int layer = 0; layer < tileMapData["mapLayout"].size(); layer++)
        {
            for (int y = 0; y < tileMapData["mapLayout"][layer].size(); y++)
            {
                for (int x = 0; x < tileMapData["mapLayout"][layer][y].size(); x++)
                {
                    // std::cout << "Creating game object for tile at layer: " << layer << ", row: " << y << ", column: " << x << std::endl;
                    // std::cout << "        Remapped TileID: " << tileMapData["mapLayout"][layer][y][x] << std::endl;

                    // get the tileID
                    std::string remappedTileID = tileMapData["mapLayout"][layer][y][x];
                    // get the tilesetName and tileName from the indexMaping
                    std::string tilesetName = tileMapData["indexMaping"][remappedTileID][0];
                    std::string tileName = tileMapData["indexMaping"][remappedTileID][1];

                    // std::cout << "        For this tile, tilesetName: " << tilesetName << ", tileName: " << tileName << std::endl;

                    sf::IntRect tileRect = tilesetTextureDataDict[tilesetName].tileRects[tileName];

                    // create a new game object for the tile. The name of the game object is like this: "tile_layer_x_y_tilesetName_tileName"
                    GameObject* tile = new GameObject("tile_" + std::to_string(layer) + "_" + std::to_string(x) + "_" + std::to_string(y) + "_" + tilesetName + "_" + tileName);
                    // GameObject* tile = new GameObject();
                    // make an unique sprite for the tile
                    // sf::Sprite* tileSprite = new sf::Sprite(tilesetTextureData.texture, tileRect);
                    tile->sprite = std::make_unique<sf::Sprite>(tilesetTextureDataDict[tilesetName].texture, tileRect);
                    
                    // get tile size from tileMapData json as int

                    // std::cout << "        Tile size: " << tileMapData["tileSize"]["width"]<< ", " << tileMapData["tileSize"]["height"]<< std::endl;

                    tile->position = {x * (tileMapData["tileSize"]["width"].get<float>()), y * tileMapData["tileSize"]["height"].get<float>()};

                    // std::cout << "        Tile position: " << tile->position.x << ", " << tile->position.y << std::endl;

                    // tile->position = {(float)x * tilesetTextureData.tileSize.x, (float)y * tilesetTextureData.tileSize.y};
                    // add the tile to the tileMap
                    addChild(tile);
                }
            }
        }
        // store the difficulty map
        std::cout << "Storing the difficulty map" << std::endl;
        for (auto& row : tileMapData["difficultyMap"])
        {
            std::vector<int> difficultyRow;
            for (auto& difficulty : row)
            {
                difficultyRow.push_back(difficulty);
            }
            difficultyMap.push_back(difficultyRow);
        }
        return true;
    }
    catch(const std::exception& e)
    {
        std::cout << "Failed to make tile map according to the tileMap file: " << tileMapDataFilepath << std::endl;
        std::cerr << e.what() << '\n';
        return false;
    }
}
  