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

GameFieldWithFigureArea::GameFieldWithFigureArea(
    std::shared_ptr<GameFieldWithFigure> field,
    std::pair<int, int> upperLeftCorner, 
    std::pair<int, int> lowerRightCorner) :
    IGameFieldArea(upperLeftCorner, lowerRightCorner)
{ field_ = field; }

void GameFieldWithFigureArea::lock() noexcept {
    isLocked_ = true;
}

void GameFieldWithFigureArea::unlock() noexcept {
    isLocked_ = false;
}

bool GameFieldWithFigureArea::isCellAvailable(
    int xidx, int yidx) const
{
    return IGameFieldArea::isCellAvailable(xidx, yidx) && 
           !isLocked_ &&
           !field_->isExcludedCell(xidx, yidx);
}

void GameFieldWithFigureArea::setCreatureInCell(
    int xidx, int yidx, std::shared_ptr<player::Player> player) 
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->setCreatureInCell(xidx, yidx, player);
}

void GameFieldWithFigureArea::removeCreatureInCell(
    int xidx, int yidx)
{
    verifyThenThrowCellPos_(xidx, yidx);
    field_->removeCreatureInCell(xidx, yidx);
}  

const creature::ICreature& 
GameFieldWithFigureArea::getCreatureByCell(int xidx, int yidx) const 
{
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->getCreatureByCell(xidx, yidx);
}

std::map<const std::shared_ptr<player::Player>, int> 
GameFieldWithFigureArea::countCellNeighborsCreatures(int xidx, int yidx) const 
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->countCellNeighborsCreatures(xidx, yidx);
}

bool GameFieldWithFigureArea::hasCreatureInCell(
    int xidx, int yidx) const 
{
    verifyThenThrowCellPos_(xidx, yidx);
    return field_->hasCreatureInCell(xidx, yidx);
}
std::set<std::shared_ptr<player::Player>> 
    GameFieldWithFigureArea::checkCreatureInArea() const 
{
    std::set<std::shared_ptr<player::Player>> res;
    auto corner = upperLeftCorner_;
    for (auto y = corner.second; 
        y <= lowerRightCorner_.second; ++y) 
    {
        for (auto x = corner.first; 
            x <= lowerRightCorner_.first; ++x) 
        {
            if (isCellAvailable(x, y)) {
                if (field_->hasCreatureInCell(x, y)) {
                    auto&& cr = field_->getCreatureByCell(x, y);
                    res.emplace(cr.player());
                }
            }
        }
    }
    return res;
}

void GameFieldWithFigureArea::clear() {
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
GameFieldWithFigureArea::upperLeftCorner() const noexcept 
{ return upperLeftCorner_; }

std::pair<int, int> 
GameFieldWithFigureArea::lowerRightCorner() const noexcept 
{ return lowerRightCorner_; }

int GameFieldWithFigureArea::width() const noexcept {
    return lowerRightCorner_.first 
            - upperLeftCorner_.first
            + 1;
}

int GameFieldWithFigureArea::height() const noexcept  {
    return lowerRightCorner_.second
            - upperLeftCorner_.second 
            + 1;
}

void GameFieldWithFigureArea::verifyThenThrowCellPos_(
    int xidx, int yidx) const {
    if (!isCellAvailable(xidx, yidx)) {
        throw std::logic_error("Accessing a forbidden cell.");
    }
}

} // namespace game_field_area

