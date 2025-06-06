#ifndef POINT_OF_EXPANSION_HPP
#define POINT_OF_EXPANSION_HPP

#include "game_field.hpp"

namespace game_field {

class GamefieldWithFigureAndTriangularNeighbors :
    public GameFieldWithFigure
{
public:
    GamefieldWithFigureAndTriangularNeighbors(
        int width, int height, 
        std::unique_ptr<factory::ICreatureFactory> creatFactory,
        std::unique_ptr<factory::ICellFactory> cellFactory,
        std::unique_ptr<figure::IFigure> figure);

public:
    std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const override;
    
private:
    void computeAddNeighbors_();

private:
    // клетка в середине противолежащей стороны
    std::map<std::pair<int, int>, std::pair<int, int>> addNeighbor_;
};

}

#endif // POINT_OF_EXPANSION_HPP