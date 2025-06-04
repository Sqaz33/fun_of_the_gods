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
#include <array>

#include "game_event.hpp"
#include "subject.hpp"
#include "creature_factory.hpp"
#include "cell_factory.hpp"
#include "figure.hpp"


namespace player {
    class Player;
} // namespace player


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

     */
    virtual void setCreatureInCell(int xidx, int yidx, 
                        std::shared_ptr<player::Player> player) = 0;
    
    /**
     * @brief Remove the creature in the specified cell.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @throws std::runtime_error if cell is out of bounds or excluded
     */
    virtual void removeCreatureInCell(int xidx, int yidx) = 0;

    /**
     * @brief 
     * 
     * @param xidx 
     * @param yidx 
     * @return true 
     * @return false 
     */
    virtual bool hasCreatureInCell(int xidx, int yidx) const = 0;

    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const = 0;

    
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
 * @class GameFieldWithFigure
 * @brief Game field implementation with excluded cells support.
 * 
 * Implements a game field with configurable excluded cells where creatures
 * cannot be placed. Implements both ISubject and IGameField interfaces.
 */
class GameFieldWithFigure :
    public IGameField,
    public std::enable_shared_from_this<GameFieldWithFigure>,
    public subject::ISubject
{
private:
    using ISubject = subject::ISubject;
    using ICreatureFactory = factory::ICreatureFactory;
    using IFigure = figure::IFigure;

public:
    /**
     */
    GameFieldWithFigure(
        int width, int height,
        std::unique_ptr<factory::ICreatureFactory> creatFactory,
        std::unique_ptr<factory::ICellFactory> cellFactory,
        std::unique_ptr<IFigure> figure);
    
public:
    // IGameField interface implementation
    const creature::ICreature& getCreatureByCell(int xidx, int yidx) const override;
    void setCreatureInCell(int xidx, int yidx, 
            std::shared_ptr<player::Player> player) override;
    void removeCreatureInCell(int xidx, int yidx) override;
    bool hasCreatureInCell(int xidx, int yidx) const override;
    std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const override;
    void clear() override;
    std::pair<int, int> lastAffectedCell() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

public:
    // Additional public methods
    /**
     * @brief Checks if a cell is excluded.
     * @param xidx X coordinate of the cell
     * @param yidx Y coordinate of the cell
     * @return true if cell is excluded, false otherwise
     */
    bool isExcludedCell(int xidx, int yidx) const;

    // ISubject interface implementation
    void attach(std::shared_ptr<observer::IObserver> obs, int event_t) override;
    void detach(std::weak_ptr<observer::IObserver> obs, int event_t) override;

private:
    void notify(int event_t) override;

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
     * @brief Notifies observers of creature revive event.
     */
    void fireCreatureSet_();
    
    /**
     * @brief Notifies observers of creature kill event.
     */
    void fireCreatureRemove_();
    
    /**
     * @brief Initializes the field with specified dimensions.
     * @param width Field width
     * @param height Field height
     */
    void initField_(int width, int height);

    void initCells_();
    
private:
    std::vector<std::vector<std::unique_ptr<cell::ICell>>> field_;    ///< 2D vector of cells
    std::pair<int, int> lastAffectedCell_ = {-1, -1};                 ///< Last modified cell coordinates
    std::unique_ptr<factory::ICreatureFactory> creatFactory_;         ///< Creature factory
    std::unique_ptr<factory::ICellFactory> cellFactory_;              ///< Cell factory
    std::unique_ptr<IFigure> figure_;

    static constexpr std::array<const std::pair<int, int>, 8> neighborsPos_ {{
        {-1,  -1}, {0, -1}, {1, -1},
        {-1,   0},          {1,  0},
        {-1,   1}, {0,  1}, {1,  1}  
    }};

};

} // namespace game_field

#endif // GAME_FIELD_HPP