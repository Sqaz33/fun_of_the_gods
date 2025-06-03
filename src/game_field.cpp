#include "game_field.hpp"

#include "player.hpp"

namespace game_field {

GameFieldWithFigure::GameFieldWithFigure(
        int width, int height, 
        std::unique_ptr<factory::ICreatureFactory> creatFactory,
        std::unique_ptr<factory::ICellFactory> cellFactory,
        std::unique_ptr<IFigure> figure) :
    creatFactory_(std::move(creatFactory))
    , cellFactory_(std::move(cellFactory))
    , figure_(std::move(figure))
    
{   
    initField_(width, height); 
    initCells_();
}

const creature::ICreature&
GameFieldWithFigure::getCreatureByCell(int xidx, int yidx) const {
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx)->creature();
}

void GameFieldWithFigure::setCreatureInCell(int xidx, int yidx, 
    std::shared_ptr<player::Player> player)
{   
    verifyThenThrowCellPos_(xidx, yidx);
    auto creat = creatFactory_->createCreature(player);
    field_.at(yidx).at(xidx)->setCreature(std::move(creat)); 
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureSet_();
}

void GameFieldWithFigure::removeCreatureInCell(
    int xidx, int yidx)
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    field_.at(yidx).at(xidx)->removeCreature();
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureRemove_();
}

bool GameFieldWithFigure::hasCreatureInCell(
    int xidx, int yidx) const 
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx)->hasCreature();
}
std::map<const std::shared_ptr<player::Player>, int> 
GameFieldWithFigure::countCellNeighborsCreatures(int xidx, int yidx) const 
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx)->countNeighborsCreatures();
}

void GameFieldWithFigure::clear() {
    int w = width();
    int h = height();
    field_.clear();
    initField_(w, h);
    initCells_();
    fireFieldClear_();
}

std::pair<int, int> 
GameFieldWithFigure::lastAffectedCell() const noexcept
{ return lastAffectedCell_; }

int GameFieldWithFigure::width() const noexcept 
{ return field_.back().size(); }

int GameFieldWithFigure::height() const noexcept 
{ return field_.size(); }

bool GameFieldWithFigure::isExcludedCell(
    int xidx, int yidx) const 
{ return !figure_->isPointInFigure(xidx, yidx); }

void GameFieldWithFigure::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t)
{ ISubject::attach(obs, event_t); }

void GameFieldWithFigure::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t)
{ ISubject::detach(obs, event_t); }

void GameFieldWithFigure::notify(int event_t) {
    ISubject::notify(event_t);
}

void GameFieldWithFigure::verifyThenThrowCellPos_(
    int xidx, int yidx) const 
{
    if (isExcludedCell(xidx, yidx)) {
        throw std::logic_error("Accessing a forbidden cell.");
    }
}

void GameFieldWithFigure::fireFieldClear_() {
    int evt = static_cast<int>(
        game_event::event_t::FIELD_CLEAR);   
    notify(evt);
}

void GameFieldWithFigure::fireCellClear_() {
    int evt = static_cast<int>(
        game_event::event_t::CELL_CLEAR_IN_FIELD);   
    notify(evt);
}

void GameFieldWithFigure::fireCreatureSet_() {
    int evt = static_cast<int>(
        game_event::event_t::CREATURE_REVIVE_IN_FIELD);   
    notify(evt);
}

void GameFieldWithFigure::fireCreatureRemove_() {
    int evt = static_cast<int>(
            game_event::event_t::CREATURE_KILL_IN_FIELD);   
    notify(evt);
}

void GameFieldWithFigure::initField_(int width, int height) {
    field_ = decltype(field_)();
    for (int i = 0; i < height; ++i) {
        std::vector<std::unique_ptr<cell::ICell>> r;
        for (int j = 0; j < width; ++j) {
            r.emplace_back(cellFactory_->createCell());
        }
        field_.emplace_back(std::move(r));
    }
}

void GameFieldWithFigure::initCells_() {
    for (int i = 0; i < height(); ++i) {
        for (int j = 0; j < width(); ++j) {
            auto&& cell = field_[i][j];
            for (auto [x, y] : neighborsPos_) {
                y += i;
                x += j;
                if (!isExcludedCell(x, y) &&
                    y >= 0 && y < height() &&
                    x >= 0 && x < width()) 
                { 
                    auto&& ne = field_[y][x];
                    cell->addNeighbors(ne.get());
                }
            }
        }
    }
}

}