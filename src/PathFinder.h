#pragma once

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <utility>  // For std::pair

/**
 * @struct Node
 * @brief Represents a cell in the grid for Dijkstra's algorithm prioritization.
 * 
 * Contains grid coordinates (x, y) and the accumulated cost to reach this cell.
 * The operator> overload enables ordering in the priority queue (min-heap).
 */
struct Node {
    int x;          ///< X-coordinate (row) in the grid
    int y;          ///< Y-coordinate (column) in the grid
    int cost;       ///< Total cost to reach this node from the start

    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

/**
 * @class PathFinder
 * @brief Finds the minimum-cost path between two points using Dijkstra's algorithm.
 * 
 * Handles grid-based pathfinding with variable movement costs and obstacles (-1).
 */
class PathFinder {
public:
    /**
     * @brief Default constructor (creates an empty PathFinder instance).
     * 
     * Use setters to configure the map, start, and target before calling findPath().
     */
    PathFinder() = default;

    /**
     * @brief Sets the difficulty/cost map for pathfinding.
     * @param map A 2D grid where values represent movement cost (-1 = obstacle).
     * @throws std::invalid_argument If the map is empty or has inconsistent row sizes.
     */
    void setDifficultyMap(const std::vector<std::vector<int> >& map);

    /**
     * @brief Sets the starting position for pathfinding.
     * @param start The (row, column) coordinates of the start point.
     */
    void setStart(const std::pair<int, int>& start) { this->start = start; }

    /**
     * @brief Sets the target position for pathfinding.
     * @param target The (row, column) coordinates of the target point.
     */
    void setTarget(const std::pair<int, int>& target) { this->target = target; }

    /**
     * @brief Executes Dijkstra's algorithm to find the minimum-cost path.
     * @return True if a valid path exists, false otherwise.
     * 
     * Requires the map, start, and target to be configured first.
     */
    bool findPath();

    /**
     * @brief Returns the accumulated cost to each cell from the start.
     * @return A 2D matrix where values represent the minimum cost to reach each cell.
     */
    const std::vector<std::vector<int> >& getCurrentCostMap() const { return currentCostMap; }

    /**
     * @brief Returns the path as a binary matrix (1=path, 0=non-path).
     * @return A 2D matrix marking the optimal path from start to target.
     */
    const std::vector<std::vector<int> >& getPathVisualization() const { return pathVisualization; }

     /**
     * @brief Returns the path as a sequence of coordinates from start to target.
     * @return Vector of (row, column) pairs representing the path.
     */
    const std::vector<std::pair<int, int> >& getPathCoordinates() const { return pathCoordinates; }



private:
    // Movement directions (up, down, left, right)
    static const std::vector<std::pair<int, int> > directions;

    // Configuration
    std::vector<std::vector<int> > difficultyMap; ///< 2D grid with movement costs (-1 = blocked)
    std::pair<int, int> start;                   ///< Start position (row, column)
    std::pair<int, int> target;                  ///< Target position (row, column)

    std::vector<std::vector<int> > pathVisualization; ///< Binary matrix for display
    std::vector<std::pair<int, int> > pathCoordinates; ///< Ordered path coordinates

    // Algorithm state
    int rows = 0;                                ///< Number of rows in the grid
    int cols = 0;                                ///< Number of columns in the grid
    std::vector<std::vector<int> > currentCostMap;///< Minimum cost to reach each cell
    // std::vector<std::vector<int> > path;          ///< Binary matrix marking the path
    std::vector<std::vector<int> > distance;      ///< Temporary storage for Dijkstra's costs
    std::vector<std::vector<std::pair<int, int> > > previous; ///< Backtracking information
    std::priority_queue<Node, std::vector<Node>, std::greater<Node> > priorityQueue; ///< Min-heap

    /**
     * @brief Resets all algorithm containers for a new pathfinding operation.
     */
    void initializeContainers();

    /**
     * @brief Checks if a grid position is valid and traversable.
     * @param pos The (row, column) position to validate.
     * @return True if the position is within bounds and not an obstacle.
     */
    bool isValid(const std::pair<int, int>& pos) const;

    /**
     * @brief Explores neighboring cells of the current node during Dijkstra's traversal.
     * @param current The node being processed from the priority queue.
     */
    void exploreNeighbors(const Node& current);

    /**
     * @brief Reconstructs the path from target back to start using previous cells.
     * @return True if the path is successfully reconstructed.
     */
    bool reconstructPath();

    /**
     * @brief Calculates the cost to move to a neighboring cell.
     * @param neighborX X-coordinate of the neighboring cell.
     * @param neighborY Y-coordinate of the neighboring cell.
     * @return The movement cost (cell value from difficultyMap).
     */
    int getStepCost(int neighborX, int neighborY) const;
};

#endif // PATHFINDER_H

