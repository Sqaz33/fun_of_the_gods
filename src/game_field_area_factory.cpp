#include "game_field_area_factory.hpp"

namespace {
    using GameFieldExcludedCells = 
        game_field::GameFieldExcludedCells;
} // namespace

namespace factory {

GameFieldExcludedCellsAreaCurryFactory::
    GameFieldExcludedCellsAreaCurryFactory(
        std::shared_ptr<GameFieldExcludedCells> field):
    field_(field)
{}

std::unique_ptr<game_field_area::IGameFieldArea>
GameFieldExcludedCellsAreaCurryFactory::createArea(
    std::pair<int, int> upperLeftCorner, 
    std::pair<int, int> lowerRightCorner)
{       
    return std::make_unique<
        game_field_area::GameFieldExcludedCellsArea>(
        field_,
        upperLeftCorner,
        lowerRightCorner);
}

} // namespace factory
