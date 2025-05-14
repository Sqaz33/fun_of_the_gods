#include "game_field_area.hpp"

namespace game_field_area {

bool IGameFieldArea::isCellAvailable(
    int xidx, int yidx) const
{
    return xidx >= upperLeftCorner_.first && 
           xidx <= lowerRightCorner_.first &&
           yidx >= upperLeftCorner_.second &&
           yidx <= lowerRightCorner_.second;
}

GameFieldExcludedCellsArea::GameFieldExcludedCellsArea(
    std::shared_ptr<GameFieldExcludedCells> field,
    std::pair<int, int> upperLeftCorner, 
    std::pair<int, int> lowerRightCorner) :
    IGameFieldArea(upperLeftCorner, lowerRightCorner)
{ field_ = field; }

void GameFieldExcludedCellsArea::lock() noexcept {
    isLocked_ = true;
}

void GameFieldExcludedCellsArea::unlock() noexcept {
    isLocked_ = false;
}

bool GameFieldExcludedCellsArea::isCellAvailable(
    int xidx, int yidx) const
{
    return IGameFieldArea::isCellAvailable(xidx, yidx) && 
           !isLocked_ &&
           !field_->isExludedCell(xidx, yidx);
}

void GameFieldExcludedCellsArea::reviveCreatureInCell(
    int xidx, int yidx, int id) 
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->reviveCreatureInCell(xidx, yidx, id);
}

void GameFieldExcludedCellsArea::killCreatureInCell(
    int xidx, int yidx)
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->killCreatureInCell(xidx, yidx);
}  

const creature::ICreature& 
GameFieldExcludedCellsArea::getCreatureByCell(int xidx, int yidx) const 
{
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->getCreatureByCell(xidx, yidx);
}

void GameFieldExcludedCellsArea::clearCell(int xidx, int yidx)  
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->clearCell(xidx, yidx);
}

void GameFieldExcludedCellsArea::clear() {
    if (width() == field_->width() && 
        height() == field_->height()) 
    { 
        field_->clear();
    } else {
        for (int y = upperLeftCorner_.second; 
                y <= lowerRightCorner_.second; ++y)
        {
            for (int x = upperLeftCorner_.first; 
                x <= lowerRightCorner_.second; ++x) 
            {   
                if (!field_->isExludedCell(x, y)) {
                    field_->clearCell(x, y);
                }
            }
        }
    }
}

std::pair<int, int> 
GameFieldExcludedCellsArea::upperLeftCorner() const noexcept 
{ return upperLeftCorner_; }

std::pair<int, int> 
GameFieldExcludedCellsArea::lowerRightCorner() const noexcept 
{ return lowerRightCorner_; }

int GameFieldExcludedCellsArea::width() const noexcept {
    return lowerRightCorner_.first 
            - upperLeftCorner_.first
            + 1;
}

int GameFieldExcludedCellsArea::height() const noexcept  {
    return lowerRightCorner_.second
            - upperLeftCorner_.second 
            + 1;
}

void GameFieldExcludedCellsArea::verifyThenThrowCellPos_(
    int xidx, int yidx) const {
    if (!isCellAvailable(xidx, yidx)) {
        throw std::logic_error("Accessing a forbidden cell.");
    }
}

} // namespace game_field_area

