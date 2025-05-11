#ifndef GAME_FIELD_AREA_FACTORY_HPP
#define GAME_FIELD_AREA_FACTORY_HPP

#include <memory>

#include "game_field_area.hpp"

namespace factory {

template <class cell_t>
struct IGameFieldAreaFactory {
    virtual
        std::unique_ptr<game_field_area::IGameFieldArea<cell_t>>
    createArea(
        std::shared_ptr<game_field::IGameField<cell_t>> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) const = 0;
    virtual ~IGameFieldAreaFactory() {} 
};

template <class cell_t>
class GameFieldExcludedCellsAreaFactory : 
    public IGameFieldAreaFactory<cell_t>  
{
public:
        std::unique_ptr<game_field_area::IGameFieldArea<cell_t>>
    createArea(
        std::shared_ptr<game_field::IGameField<cell_t>> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner) const override
    {       
        if (auto f = std::dynamic_pointer_cast<
                game_field::GameFieldExcludedCells<cell_t>>(field); !f)
        {
            throw std::logic_error(
                "The field should actually be an GameFieldExcludedCells<cell_t>");
        } else {
            return std::make_unique<
                game_field_area::GameFieldExcludedCellsArea<cell_t>>(
                    f,
                    upperLeftCorner,
                    lowerRightCorner);
        }

    }
};

} // namespace factory

#endif // GAME_FIELD_AREA_FACTORY_HPP