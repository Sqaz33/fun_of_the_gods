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
           !field_->isExcludedCell(xidx, yidx);
}

void GameFieldExcludedCellsArea::setCreatureInCell(
    int xidx, int yidx, std::shared_ptr<player::Player> player) 
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->setCreatureInCell(xidx, yidx, player);
}

void GameFieldExcludedCellsArea::removeCreatureInCell(
    int xidx, int yidx)
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->removeCreatureInCell(xidx, yidx);
}  

const creature::ICreature& 
GameFieldExcludedCellsArea::getCreatureByCell(int xidx, int yidx) const 
{
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->getCreatureByCell(xidx, yidx);
}

std::map<const std::shared_ptr<player::Player>, int> 
GameFieldExcludedCellsArea::countCellNeighborsCreatures(int xidx, int yidx) const 
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->countCellNeighborsCreatures(xidx, yidx);
}

bool GameFieldExcludedCellsArea::hasCreatureInCell(
    int xidx, int yidx) const 
{
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->hasCreatureInCell(xidx, yidx);
}

void GameFieldExcludedCellsArea::clear() {
    if (width() == field_->width() && 
        height() == field_->height()) 
    { 
        field_->clear();
    } 
    else 
    {
        for (int y = upperLeftCorner_.second; 
                y <= lowerRightCorner_.second; ++y)
        {
            for (int x = upperLeftCorner_.first; 
                x <= lowerRightCorner_.second; ++x) 
            {   
                if (!field_->isExcludedCell(x, y)) {
                    field_->removeCreatureInCell(x, y);
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

