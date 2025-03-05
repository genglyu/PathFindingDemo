#include <SFML/Graphics.hpp> // Looks like this is only a display issue in VSCode. The code intellisense is working fine.
#include <stdio.h>
#include "Game.h"

#include "Utility.hpp"
#include <map>
#include "TileMap.h"
#include "PathFinder.h"

int main()
{
    std::cout << "The executable path is: " << getExecutablePath() << std::endl;
    Game game;

    TileMap* tileMap = new TileMap("testingTileMap");
    std::string tileMapDataFilepath = getExecutablePath() / "assets/tileMap/TestingMap1.json";
    tileMap->loadTileMapData(tileMapDataFilepath);
    game.addGameObject(tileMap);


    // check the tileMap->difficultyMap
    std::cout << "The difficulty map is: " << std::endl;
    for (int i = 0; i < tileMap->difficultyMap.size(); i++)
    {
        for (int j = 0; j < tileMap->difficultyMap[i].size(); j++)
        {
            std::cout << std::setw(3) << std::setfill(' ') << tileMap->difficultyMap[i][j] << " ";
        }
        std::cout << std::endl;
    }


    PathFinder finder;

    // Configure after instantiation
    finder.setDifficultyMap(tileMap->difficultyMap);
    finder.setStart({10, 7});
    finder.setTarget({0, 7});

    if (finder.findPath()) {
        // Get coordinate sequence
        const auto& path = finder.getPathCoordinates();
        std::cout << "Path coordinates:\n";
        for (const auto& [x, y] : path) {
            std::cout << "(" << x << ", " << y << ") ";
        }
    
        // Get visualization matrix
        const auto& pathShownOnGrid = finder.getPathVisualization();
        std::cout << "\nPath visualization:\n";
        for (const auto& row : pathShownOnGrid) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }
        
    }


    game.run();
    return 0;
}



//====================================================================================================

// #include <iostream>
// #include <vector>
// #include <queue>
// #include <limits>
// #include <stdexcept>

// using namespace std;

// struct Node {
//     int x, y, cost;
//     bool operator>(const Node& other) const {
//         return cost > other.cost;
//     }
// };

// vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// class PathFinder {
// public:
//     PathFinder(const vector<vector<int>>& difficultyMap) 
//         : difficultyMap(difficultyMap), rows(difficultyMap.size()), cols(difficultyMap[0].size()) {
//         if (rows == 0 || cols == 0) throw invalid_argument("Invalid difficulty map");
//     }

//     bool findPath(const pair<int, int>& start, const pair<int, int>& target) {
//         initializeContainers();
        
//         if (!isValid(start) || !isValid(target)) return false;

//         distance[start.first][start.second] = difficultyMap[start.first][start.second];
//         pq.push({start.first, start.second, distance[start.first][start.second]});

//         while (!pq.empty()) {
//             Node current = pq.top(); pq.pop();
//             if (current.x == target.first && current.y == target.second) break;
//             exploreNeighbors(current);
//         }

//         currentCostFromBeginningMap = distance;
//         return reconstructPath(start, target);
//     }

//     const vector<vector<int>>& getCurrentCostMap() const { return currentCostFromBeginningMap; }
//     const vector<vector<int>>& getPath() const { return path; }

// private:
//     vector<vector<int>> difficultyMap;
//     int rows, cols;
//     vector<vector<int>> currentCostFromBeginningMap;
//     vector<vector<int>> path;
//     vector<vector<int>> distance;
//     vector<vector<pair<int, int>>> prev;
//     priority_queue<Node, vector<Node>, greater<Node>> pq;

//     void initializeContainers() {
//         distance = vector<vector<int>>(rows, vector<int>(cols, numeric_limits<int>::max()));
//         prev = vector<vector<pair<int, int>>>(rows, vector<pair<int, int>>(cols, {-1, -1}));
//         path = vector<vector<int>>(rows, vector<int>(cols, 0));
//         pq = priority_queue<Node, vector<Node>, greater<Node>>();
//     }

//     bool isValid(const pair<int, int>& pos) const {
//         return pos.first >= 0 && pos.first < rows &&
//                pos.second >= 0 && pos.second < cols &&
//                difficultyMap[pos.first][pos.second] != -1;
//     }

//     void exploreNeighbors(const Node& current) {
//         for (const auto& [dx, dy] : directions) {
//             int nx = current.x + dx, ny = current.y + dy;
//             if (isValid({nx, ny})) {
//                 int newCost = distance[current.x][current.y] + difficultyMap[nx][ny];
//                 if (newCost < distance[nx][ny]) {
//                     distance[nx][ny] = newCost;
//                     prev[nx][ny] = {current.x, current.y};
//                     pq.push({nx, ny, newCost});
//                 }
//             }
//         }
//     }

//     bool reconstructPath(const pair<int, int>& start, const pair<int, int>& target) {
//         pair<int, int> current = target;
//         while (current != start) {
//             if (current.first == -1 || current.second == -1) return false;
//             path[current.first][current.second] = 1;
//             current = prev[current.first][current.second];
//         }
//         path[start.first][start.second] = 1;
//         return true;
//     }
// };

// // 示例用法
// int main() {
//     vector<vector<int>> difficultyMap = {
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, -1, -1, -1, 5, 5, 5, -1, -1, -1, 1, 1, 1},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//     };

//     PathFinder pathFinder(difficultyMap);
//     pair<int, int> start = {10, 8}, target = {0, 8}; // 调整起点和终点

//     if (pathFinder.findPath(start, target)) {
//         cout << "Path found!\nPath Matrix (1=path):\n";
//         for (const auto& row : pathFinder.getPath()) {
//             for (int val : row) cout << val << " ";
//             cout << endl;
//         }

//         cout << "\nCost Map:\n";
//         for (const auto& row : pathFinder.getCurrentCostMap()) {
//             for (int cost : row) {
//                 if (cost == numeric_limits<int>::max()) cout << "INF ";
//                 else cout << cost << " ";
//             }
//             cout << endl;
//         }
//     } else {
//         cout << "No valid path exists!" << endl;
//     }

//     return 0;
// }