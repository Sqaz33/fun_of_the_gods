#ifndef GAME_FIELD_AREA_FACTORY_HPP
#define GAME_FIELD_AREA_FACTORY_HPP

#include <memory>

#include "game_field_area.hpp"

namespace factory {

struct IGameFieldAreaFactory {
    virtual
        std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::shared_ptr<game_field::IGameField> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) const = 0;
    virtual ~IGameFieldAreaFactory() {} 
};

class GameFieldExcludedCellsAreaFactory : 
public IGameFieldAreaFactory
{
public:
    std::unique_ptr<game_field_area::IGameFieldArea>
    createArea(
        std::shared_ptr<game_field::IGameField> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) const override;
};

} // namespace factory

#endif // GAME_FIELD_AREA_FACTORY_HPP