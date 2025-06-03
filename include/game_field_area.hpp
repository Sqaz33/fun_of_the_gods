/**
 * @file game_field_area.hpp
 * @brief Header file for game field area interfaces and implementations.
 * 
 * Defines the interface and implementation for managing bounded areas of game fields
 * with support for cell exclusions and creature management.
 */

#ifndef GAME_FIELD_AREA_HPP
#define GAME_FIELD_AREA_HPP

#include "game_field.hpp"

namespace game_field_area {

/**
 * @interface IGameFieldArea
 * @brief Interface representing a bounded rectangular area of a game field.
 * 
 * Provides thread-safe operations for managing creatures within a defined area,
 * with support for cell availability checking and area boundary management.
 */
struct IGameFieldArea {
    /**
     * @brief Constructs a game field area with specified boundaries.
     * @param upperLeftCorner Coordinates of the upper-left corner (x,y)
     * @param lowerRightCorner Coordinates of the lower-right corner (x,y)
     */
    IGameFieldArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner):
        upperLeftCorner_(upperLeftCorner),
        lowerRightCorner_(lowerRightCorner)
    {}
    
    /**
     * @brief Locks the area for thread-safe operations.
     */
    virtual void lock() noexcept = 0;
    
    /**
     * @brief Unlocks the area after operations are complete.
     */
    virtual void unlock() noexcept = 0;
    
    /**
     * @brief Checks if a cell is available for creature placement.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return true if the cell is available, false otherwise
     */
    virtual bool isCellAvailable(int xidx, int yidx) const = 0;
    
    /**
     * @brief Revives a creature in the specified cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @param id Creature ID to revive
     */
    virtual void setCreatureInCell(int xidx, int yidx, 
                std::shared_ptr<player::Player> player) = 0;
    
    /**
     * @brief Kills the creature in the specified cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     */
    virtual void removeCreatureInCell(int xidx, int yidx) = 0;

    virtual bool hasCreatureInCell(int xidx, int yidx) const = 0;

    virtual const creature::ICreature& 
        getCreatureByCell(int xidx, int yidx) const = 0;

    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const = 0;

    virtual std::set<std::shared_ptr<player::Player>> 
        checkCreatureInArea() const = 0;

    /**
     * @brief Clears all cells in the area.
     */
    virtual void clear() = 0;
    
    /**
     * @brief Gets the upper-left corner coordinates.
     * @return Coordinates pair (x,y)
     */
    virtual std::pair<int, int> upperLeftCorner() const noexcept = 0;
    
    /**
     * @brief Gets the lower-right corner coordinates.
     * @return Coordinates pair (x,y)
     */
    virtual std::pair<int, int> lowerRightCorner() const noexcept = 0;
    
    /**
     * @brief Calculates the width of the area.
     * @return Width in cells
     */
    virtual int width() const noexcept = 0;
    
    /**
     * @brief Calculates the height of the area.
     * @return Height in cells
     */
    virtual int height() const noexcept = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IGameFieldArea() = default;

protected:
    std::pair<int, int> upperLeftCorner_;   ///< Upper-left boundary coordinates
    std::pair<int, int> lowerRightCorner_;  ///< Lower-right boundary coordinates
};

/**
 * @class GameFieldWithFigureArea
 * @brief Implementation of IGameFieldArea that respects excluded cells.
 * 
 * Manages a rectangular area of the game field while honoring a configuration
 * of excluded cells where creatures cannot be placed.
 */
class GameFieldWithFigureArea : public IGameFieldArea {
    using GameFieldWithFigure = game_field::GameFieldWithFigure;
    using ICreature = creature::ICreature;
    
public:
    /**
     * @brief Constructs an area with excluded cells configuration.
     * @param field Shared pointer to excluded cells configuration
     * @param upperLeftCorner Coordinates of the upper-left corner (x,y)
     * @param lowerRightCorner Coordinates of the lower-right corner (x,y)
     */
    GameFieldWithFigureArea(
        std::shared_ptr<GameFieldWithFigure> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner);

    // Interface implementations
    void lock() noexcept override;
    void unlock() noexcept override;
    bool isCellAvailable(int xidx, int yidx) const override;
    void setCreatureInCell(int xidx, int yidx, 
            std::shared_ptr<player::Player> player) override;
    void removeCreatureInCell(int xidx, int yidx) override;
    bool hasCreatureInCell(int xidx, int yidx) const override;
    const ICreature& getCreatureByCell(int xidx, int yidx) const override;
    std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const override;
    std::set<std::shared_ptr<player::Player>> 
        checkCreatureInArea() const override;
    void clear() override;
    std::pair<int, int> upperLeftCorner() const noexcept override;
    std::pair<int, int> lowerRightCorner() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

private:
    /**
     * @brief Verifies cell position and throws if invalid.
     * @param xidx X coordinate to check
     * @param yidx Y coordinate to check
     * @throws std::out_of_range if coordinates are invalid
     */
    void verifyThenThrowCellPos_(int xidx, int yidx) const;

private:
    bool isLocked_ = true; ///< Thread-safety lock flag
    std::shared_ptr<GameFieldWithFigure> field_; ///< Excluded cells configuration
};

} // namespace game_field_area

#endif // GAME_FIELD_AREA_HPP