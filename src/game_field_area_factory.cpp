#include "game_field_area_factory.hpp"

namespace factory {

std::unique_ptr<game_field_area::IGameFieldArea>
IGameFieldAreaFactory::createArea(
    std::shared_ptr<game_field::IGameField> field,
    std::pair<int, int> upperLeftCorner, 
    std::pair<int, int> lowerRightCorner) const 
{       
    if (auto f = std::dynamic_pointer_cast<
            game_field::GameFieldExcludedCells>(field); !f)
    {
        throw std::logic_error(
            "The field should actually be an GameFieldExcludedCells<cell_t>");
    } else {
        return std::make_unique<
            game_field_area::GameFieldExcludedCellsArea>(
                f,
                upperLeftCorner,
                lowerRightCorner);
    }
}

} // namespace factory
