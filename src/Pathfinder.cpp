#include "PathFinder.h"
#include <iostream>

// Initialize static member: movement directions (up, down, left, right)
const std::vector<std::pair<int, int> > PathFinder::directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Configure the difficulty map and validate its consistency
void PathFinder::setDifficultyMap(const std::vector<std::vector<int> >& map) {
    if (map.empty() || map[0].empty()) {
        throw std::invalid_argument("Difficulty map cannot be empty");
    }

    // Ensure all rows have the same number of columns
    const size_t expectedCols = map[0].size();
    for (const auto& row : map) {
        if (row.size() != expectedCols) {
            throw std::invalid_argument("All rows in the map must have the same length");
        }
    }

    difficultyMap = map;
    rows = static_cast<int>(map.size());
    cols = static_cast<int>(map[0].size());
}

// Reset internal state for a new pathfinding operation
void PathFinder::initializeContainers() {
    // Initialize distance matrix with infinity (indicating unreachable cells)
    distance = std::vector<std::vector<int> >(rows, 
        std::vector<int>(cols, std::numeric_limits<int>::max()));
    
    // Reset previous cell tracking (used for path reconstruction)
    previous = std::vector<std::vector<std::pair<int, int> > >(rows, 
        std::vector<std::pair<int, int> >(cols, {-1, -1}));
    
    // Clear existing path markers
    pathVisualization = std::vector<std::vector<int> >(rows, std::vector<int>(cols, 0));
    
    // Reset the priority queue (min-heap)
    priorityQueue = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >();
}

// Check if a position is within grid bounds and traversable (not -1)
bool PathFinder::isValid(const std::pair<int, int>& pos) const {
    // Check grid boundaries
    if (pos.first < 0 || pos.first >= rows || pos.second < 0 || pos.second >= cols) {
        return false;
    }
    
    // Check if the cell is an obstacle
    return difficultyMap[pos.first][pos.second] != -1;
}


//============== COST CALCULATION (DIJKSTRA vs BFS) ==============//
// To convert this to BFS:
// 1. Change return value to 1
// 2. Breadth-first search will ignore difficulty values
int PathFinder::getStepCost(int neighborX, int neighborY) const {
    return difficultyMap[neighborX][neighborY];  // Dijkstra's uses actual costs
    // return 1;  // Uncomment this line for BFS behavior
}
//===============================================================//


// Explore all valid neighbors of the current node and update costs
void PathFinder::exploreNeighbors(const Node& current) {
    for (const auto& [dx, dy] : directions) {
        const int neighborX = current.x + dx;
        const int neighborY = current.y + dy;
        const std::pair<int, int> neighborPos(neighborX, neighborY);

        // Skip invalid or blocked cells
        if (!isValid(neighborPos)) continue;

        // Calculate new accumulated cost to reach this neighbor
        const int stepCost = getStepCost(neighborX, neighborY);
        const int newCost = distance[current.x][current.y] + stepCost;

        // Update if this path is cheaper than previously known
        if (newCost < distance[neighborX][neighborY]) {
            distance[neighborX][neighborY] = newCost;
            previous[neighborX][neighborY] = {current.x, current.y};
            priorityQueue.push({neighborX, neighborY, newCost});
        }
    }
}

// Backtrack from target to start using previous cell information
bool PathFinder::reconstructPath() {
    pathCoordinates.clear();  // Clear previous path
    
    std::pair<int, int> current = target;
    std::vector<std::pair<int, int> > reversePath;

    // Backtrack from target to start
    while (current != start) {
        if (current.first == -1 || current.second == -1) {
            pathVisualization = std::vector<std::vector<int> >(rows, std::vector<int>(cols, 0));
            pathCoordinates.clear();
            return false;
        }
        
        reversePath.push_back(current);
        pathVisualization[current.first][current.second] = 1;
        current = previous[current.first][current.second];
    }

    // Add start and reverse the path
    reversePath.push_back(start);
    std::reverse(reversePath.begin(), reversePath.end());
    
    // Store ordered coordinates
    pathCoordinates = reversePath;
    return true;
}

// Main Dijkstra's algorithm implementation
bool PathFinder::findPath() {
    // Validate preconditions
    if (difficultyMap.empty()) {
        throw std::logic_error("Difficulty map not set. Call setDifficultyMap() first.");
    }
    if (!isValid(start) || !isValid(target)) {
        return false;
    }

    initializeContainers();

    // Initialize starting node
    distance[start.first][start.second] = getStepCost(start.first, start.second);
    priorityQueue.push({start.first, start.second, distance[start.first][start.second]});

    // Process nodes in order of increasing cost
    while (!priorityQueue.empty()) {
        const Node current = priorityQueue.top();
        priorityQueue.pop();

        // Early exit if target is reached
        if (current.x == target.first && current.y == target.second) break;

        exploreNeighbors(current);
    }

    // Save cost map and reconstruct path
    currentCostMap = distance;
    return reconstructPath();
}