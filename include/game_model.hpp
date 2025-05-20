/**
 * @file game_model.hpp
 * @brief Header file for the game model interfaces and implementations.
 * 
 * Defines the game model interface and implementation for computing
 * game state transitions and determining game outcomes.
 */

#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <memory>
#include <utility>
#include <unordered_map>

#include "creature_factory.hpp"
#include "game_field_area.hpp"

namespace game_model {

/**
 * @interface IGameModel
 * @brief Interface for game models that compute game state transitions.
 * 
 * Provides the basic operation for computing one iteration of the game model.
 */
struct IGameModel {
    /**
     * @brief Computes one iteration of the game model.
     * @return std::pair<bool, int> where:
     *         - first: true if game has concluded, false otherwise
     *         - second: winning player ID if game concluded, undefined otherwise
     */
    virtual std::pair<bool, int> compute() = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IGameModel() = default;
};

/**
 * @class GameModel
 * @brief Concrete implementation of the game model.
 * 
 * Implements game logic for computing creature interactions and determining
 * game outcomes based on the current state of the game field area.
 */
class GameModel : public IGameModel {
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    /**
     * @brief Constructs a game model for the specified area.
     * @param area Game field area to model
     * @param playerCount Number of players in the game
     */
    GameModel(std::unique_ptr<IGameFieldArea> area, int playerCount);
    
    /**
     * @brief Computes one iteration of the game model.
     * @return std::pair<bool, int> where:
     *         - first: true if game has concluded (win/draw), false otherwise
     *         - second: winning player ID if game concluded, undefined otherwise
     * 
     * @note The computation follows these rules:
     *       1. Any live creature with fewer than two live neighbors dies (underpopulation)
     *       2. Any live creature with two or three live neighbors lives on
     *       3. Any live creature with more than three live neighbors dies (overpopulation)
     *       4. Any dead cell with exactly three live neighbors becomes a live creature (reproduction)
     */
    std::pair<bool, int> compute() override;

private:
    /**
     * @brief Counts neighboring creatures for a given cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return Map of creature IDs to their neighbor counts
     */
    std::unordered_map<int, int> countNeighbors_(int xidx, int yidx) const;
    
    /**
     * @brief Computes next state changes without applying them.
     */
    void computeAside_();
    
    /**
     * @brief Applies computed state changes and clears the aside buffer.
     */
    void applyNClearAside_();
    
    /**
     * @brief Counts alive creatures in the area by player.
     * @return Map of player IDs to whether they have alive creatures
     */
    std::unordered_map<int, bool> countAliveCreatureInArea_();

private:
    std::unique_ptr<IGameFieldArea> area_;  ///< Game field area being modeled
    std::vector<std::tuple<int, bool, int, int>> aside_; ///< Buffer for state transitions
    int playerCount_; ///< Number of players in the game
};

} // namespace game_model

#endif // GAME_MODEL_HPP