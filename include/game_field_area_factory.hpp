#ifndef GAME_FIELD_AREA_FACTORY_HPP
#define GAME_FIELD_AREA_FACTORY_HPP

#include <memory>

#include "game_field_area.hpp"

namespace factory {

struct IGameFieldAreaCurryFactory {
    virtual
        std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) = 0;
    virtual ~IGameFieldAreaCurryFactory() {} 
};

class GameFieldExcludedCellsAreaCurryFactory : 
public IGameFieldAreaCurryFactory
{
    using GameFieldExcludedCells = 
        game_field::GameFieldExcludedCells;

public:
    GameFieldExcludedCellsAreaCurryFactory(
        std::shared_ptr<GameFieldExcludedCells> field);

public:
    std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) override;

private:
    std::shared_ptr<GameFieldExcludedCells> field_;
};

} // namespace factory

#endif // GAME_FIELD_AREA_FACTORY_HPP