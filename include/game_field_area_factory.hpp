/**
 * @file game_field_area_factory.hpp
 * @brief Header file for the game field area factory interfaces and implementations.
 * 
 * Defines factories for creating game field areas with specific exclusion patterns.
 */

#ifndef GAME_FIELD_AREA_FACTORY_HPP
#define GAME_FIELD_AREA_FACTORY_HPP

#include <memory>

#include "game_field_area.hpp"

namespace factory {

/**
 * @interface IGameFieldAreaCurryFactory
 * @brief Interface for factories that create game field areas within specified bounds.
 * 
 * This factory pattern creates IGameFieldArea instances constrained to
 * specific rectangular regions of the game field.
 */
struct IGameFieldAreaCurryFactory {
    /**
     * @brief Creates a new game field area within specified bounds.
     * @param upperLeftCorner Coordinates of the upper-left corner (x,y)
     * @param lowerRightCorner Coordinates of the lower-right corner (x,y)
     * @return std::unique_ptr<IGameFieldArea> A new game field area instance
     */
    virtual std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IGameFieldAreaCurryFactory() = default;
};

/**
 * @class GameFieldExcludedCellsAreaCurryFactory
 * @brief Factory that creates game field areas while respecting excluded cells.
 * 
 * This implementation creates areas that honor the exclusion patterns
 * defined in the GameFieldExcludedCells configuration.
 */
class GameFieldExcludedCellsAreaCurryFactory : 
    public IGameFieldAreaCurryFactory
{
    using GameFieldExcludedCells = game_field::GameFieldExcludedCells;

public:
    /**
     * @brief Constructs a factory with excluded cells configuration.
     * @param field Shared pointer to excluded cells configuration
     */
    GameFieldExcludedCellsAreaCurryFactory(
        std::shared_ptr<GameFieldExcludedCells> field);

    /**
     * @brief Creates a new game field area within bounds, respecting exclusions.
     * @param upperLeftCorner Coordinates of the upper-left corner (x,y)
     * @param lowerRightCorner Coordinates of the lower-right corner (x,y)
     * @return std::unique_ptr<IGameFieldArea> New area respecting excluded cells
     * 
     * @note The created area will not include any cells marked as excluded
     *       in the GameFieldExcludedCells configuration.
     */
    std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) override;

private:
    std::shared_ptr<GameFieldExcludedCells> field_; ///< Excluded cells configuration
};

} // namespace factory

#endif // GAME_FIELD_AREA_FACTORY_HPP