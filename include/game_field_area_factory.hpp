#ifndef GAME_FIELD_AREA_FACTORY_HPP
#define GAME_FIELD_AREA_FACTORY_HPP

#include <memory>

#include "game_field_area.hpp"

namespace factory {

struct IGameFieldAreaCurryFactory {
    virtual std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) = 0;
    
    virtual ~IGameFieldAreaCurryFactory() = default;
};

class GameFieldWithFigureAreaCurryFactory : 
    public IGameFieldAreaCurryFactory
{
    using GameFieldWithFigure = game_field::GameFieldWithFigure;

public:
    GameFieldWithFigureAreaCurryFactory(
        std::shared_ptr<GameFieldWithFigure> field);
    std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) override;

private:
    std::shared_ptr<GameFieldWithFigure> field_;
};

} // namespace factory

#endif // GAME_FIELD_AREA_FACTORY_HPP