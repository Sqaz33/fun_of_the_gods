/**
 * @file game_field.hpp
 * @brief Header file for game field interfaces and implementations.
 * 
 * Defines the game field interface and implementation with excluded cells support,
 * including creature management and observer pattern integration.
 */

#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <set>
#include <vector>
#include <utility>
#include <stdexcept>

#include "cell.hpp"
#include "game_event.hpp"
#include "subject.hpp"
#include "creature_factory.hpp"

namespace game_field {

/**
 * @interface IGameField
 * @brief Interface representing a game field with creature management capabilities.
 * 
 * Provides operations for managing creatures on a 2D grid, including
 * creation, destruction, and querying of creatures in cells.
 */
struct IGameField {
    /**
     * @brief Gets the creature at specified cell coordinates.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return const reference to the creature
     * @throws std::runtime_error if cell is empty or out of bounds
     */
    virtual const creature::ICreature& getCreatureByCell(int xidx, int yidx) const = 0;
    
    /**
     * @brief Revives a creature in the specified cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @param id Creature ID to revive
     * @throws std::runtime_error if cell is out of bounds or excluded
     */
    virtual void reviveCreatureInCell(int xidx, int yidx, int id) = 0;
    
    /**
     * @brief Kills the creature in the specified cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @throws std::runtime_error if cell is out of bounds or excluded
     */
    virtual void killCreatureInCell(int xidx, int yidx) = 0;
    
    /**
     * @brief Clears the specified cell (removes any creature).
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @throws std::runtime_error if cell is out of bounds or excluded
     */
    virtual void clearCell(int xidx, int yidx) = 0;
    
    /**
     * @brief Clears all cells in the field.
     */
    virtual void clear() = 0;
    
    /**
     * @brief Gets the last affected cell coordinates.
     * @return Pair of coordinates (x,y) or (-1,-1) if none
     */
    virtual std::pair<int, int> lastAffectedCell() const noexcept = 0;
    
    /**
     * @brief Gets the width of the field.
     * @return Width in cells
     */
    virtual int width() const noexcept = 0;
    
    /**
     * @brief Gets the height of the field.
     * @return Height in cells
     */
    virtual int height() const noexcept = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IGameField() = default;
};

/**
 * @class GameFieldExcludedCells
 * @brief Game field implementation with excluded cells support.
 * 
 * Implements a game field with configurable excluded cells where creatures
 * cannot be placed. Implements both ISubject and IGameField interfaces.
 */
class GameFieldExcludedCells :
    public IGameField,
    public std::enable_shared_from_this<GameFieldExcludedCells>,
    public subject::ISubject
{
private:
    using ISubject = subject::ISubject;
    using ICreatureFactory = factory::ICreatureFactory;

public:
    /**
     * @brief Constructs a game field with excluded cells.
     * @param width Field width in cells
     * @param height Field height in cells
     * @param excludedCells Vector of (x,y) coordinates of excluded cells
     * @param creatFactory Factory for creating creatures
     */
    GameFieldExcludedCells(
        int width, int height,
        const std::vector<std::pair<int, int>>& excludedCells,
        std::unique_ptr<factory::ICreatureFactory> creatFactory);
    
    // IGameField interface implementation
    const creature::ICreature& getCreatureByCell(int xidx, int yidx) const override;
    void reviveCreatureInCell(int xidx, int yidx, int id) override;
    void killCreatureInCell(int xidx, int yidx) override;
    void clearCell(int xidx, int yidx) override;
    void clear() override;
    std::pair<int, int> lastAffectedCell() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

    // Additional public methods
    /**
     * @brief Checks if a cell is excluded.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return true if cell is excluded, false otherwise
     */
    bool isExcludedCell(int xidx, int yidx) const;
    
    /**
     * @brief Gets a shared pointer to this instance.
     * @return shared_ptr to this field
     */
    std::shared_ptr<GameFieldExcludedCells> slf();

    // ISubject interface implementation
    void attach(std::shared_ptr<observer::IObserver> obs, int event_t) override;
    void detach(std::weak_ptr<observer::IObserver> obs, int event_t) override;

protected:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override;

private:
    /**
     * @brief Verifies cell coordinates and throws if invalid.
     * @param xidx X coordinate to check
     * @param yidx Y coordinate to check
     * @throws std::out_of_range if coordinates are invalid
     */
    void verifyThenThrowCellPos_(int xidx, int yidx) const;
    
    /**
     * @brief Notifies observers of field clear event.
     */
    void fireFieldClear_();
    
    /**
     * @brief Notifies observers of cell clear event.
     */
    void fireCellClear_();
    
    /**
     * @brief Notifies observers of creature revive event.
     */
    void fireCreatureRevive_();
    
    /**
     * @brief Notifies observers of creature kill event.
     */
    void fireCreatureKill_();
    
    /**
     * @brief Prepares the field for use.
     */
    void prepareField_();
    
    /**
     * @brief Initializes the field with specified dimensions.
     * @param width Field width
     * @param height Field height
     */
    void initField_(int width, int height);
    
private:
    std::vector<std::vector<cell::Cell>> field_;          ///< 2D vector of cells
    std::set<std::pair<int, int>> excludedCells_;         ///< Set of excluded cell coordinates
    std::pair<int, int> lastAffectedCell_ = {-1, -1};     ///< Last modified cell coordinates
    std::unique_ptr<factory::ICreatureFactory> creatFactory_; ///< Creature factory
};

} // namespace game_field

#endif // GAME_FIELD_HPP