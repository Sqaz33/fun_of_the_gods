#include "game_field_area_factory.hpp"

namespace {
    using GameFieldWithFigure = 
        game_field::GameFieldWithFigure;
} // namespace

namespace factory {

GameFieldWithFigureAreaCurryFactory::
    GameFieldWithFigureAreaCurryFactory(
        std::shared_ptr<GameFieldWithFigure> field):
    field_(field)
{}

std::unique_ptr<game_field_area::IGameFieldArea>
GameFieldWithFigureAreaCurryFactory::createArea(
    std::pair<int, int> upperLeftCorner, 
    std::pair<int, int> lowerRightCorner)
{       
    return std::make_unique<
        game_field_area::GameFieldWithFigureArea>(
        field_,
        upperLeftCorner,
        lowerRightCorner);
}

} // namespace factory
