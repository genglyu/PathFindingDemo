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
    //         "tilesetsName": "relativePathToTilesetsDataFile.json"
    //     },
    //     "indexMaping": {
    //         "tileID": ["tilesetsName", "tileName"]
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

    
    try {
        // load json file from the filepath
        std::ifstream tileMapDataFile(tileMapDataFilepath);
        nlohmann::json tileMapData;
        tileMapDataFile >> tileMapData;

        // get the tileMap data file directory
        std::filesystem::path tileMapDataFileDirectory = std::filesystem::path(tileMapDataFilepath).parent_path();
        // create game objects for each tile in the tile map

        // load the tilesets data
        std::map<std::string, nlohmann::json> tilesetsData;
        for (auto& [tilesetsName, tilesetsDataFileRelativePath] : tileMapData["tileTexture"].items())
        {
            std::string tilesetsDataFilepath = (tileMapDataFileDirectory / tilesetsDataFileRelativePath).string();
            std::ifstream tilesetsDataFile(tilesetsDataFilepath);
            nlohmann::json tilesetsDataJson;
            tilesetsDataFile >> tilesetsDataJson;
            tilesetsData[tilesetsName] = tilesetsDataJson;
        }



        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}
  