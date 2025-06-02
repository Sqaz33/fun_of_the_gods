#include "game_field.hpp"

#include "player.hpp"

namespace game_field {

GameFieldExcludedCells::GameFieldExcludedCells(
        int width, int height, 
        const std::vector<std::pair<int, int>>& excludedCells,
        std::unique_ptr<factory::ICreatureFactory> creatFactory,
        std::unique_ptr<factory::ICellFactory> cellFactory) :
    excludedCells_(excludedCells.begin(), excludedCells.end())
    , creatFactory_(std::move(creatFactory))
    , cellFactory_(std::move(cellFactory))
{   
    initField_(width, height); 
}

const creature::ICreature&
GameFieldExcludedCells::getCreatureByCell(int xidx, int yidx) const {
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx)->creature();
}

void GameFieldExcludedCells::setCreatureInCell(int xidx, int yidx, 
    std::shared_ptr<player::Player> player)
{   
    verifyThenThrowCellPos_(xidx, yidx);
    auto creat = creatFactory_->createCreature(player);
    field_.at(yidx).at(xidx)->setCreature(std::move(creat)); 
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureSet_();
}

void GameFieldExcludedCells::removeCreatureInCell(
    int xidx, int yidx)
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    field_.at(yidx).at(xidx)->removeCreature();
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureRemove_();
}

bool GameFieldExcludedCells::hasCreatureInCell(
    int xidx, int yidx) const 
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx)->hasCreature();
}

void GameFieldExcludedCells::clear() {
    int w = width();
    int h = height();
    field_.clear();
    initField_(w, h);
    fireFieldClear_();
}

std::pair<int, int> 
GameFieldExcludedCells::lastAffectedCell() const noexcept
{ return lastAffectedCell_; }

int GameFieldExcludedCells::width() const noexcept 
{ return field_.back().size(); }

int GameFieldExcludedCells::height() const noexcept 
{ return field_.size(); }

bool GameFieldExcludedCells::isExcludedCell(
    int xidx, int yidx) const 
{ return excludedCells_.contains({xidx, yidx}); }

void GameFieldExcludedCells::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t)
{ ISubject::attach(obs, event_t); }

void GameFieldExcludedCells::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t)
{ ISubject::detach(obs, event_t); }

void GameFieldExcludedCells::notify(int event_t) {
    ISubject::notify(event_t);
}

void GameFieldExcludedCells::verifyThenThrowCellPos_(
    int xidx, int yidx) const 
{
    if (isExcludedCell(xidx, yidx)) {
        throw std::logic_error("Accessing a forbidden cell.");
    }
}

void GameFieldExcludedCells::fireFieldClear_() {
    int evt = static_cast<int>(
        game_event::event_t::FIELD_CLEAR);   
    notify(evt);
}

void GameFieldExcludedCells::fireCellClear_() {
    int evt = static_cast<int>(
        game_event::event_t::CELL_CLEAR_IN_FIELD);   
    notify(evt);
}

void GameFieldExcludedCells::fireCreatureSet_() {
    int evt = static_cast<int>(
        game_event::event_t::CREATURE_REVIVE_IN_FIELD);   
    notify(evt);
}

void GameFieldExcludedCells::fireCreatureRemove_() {
    int evt = static_cast<int>(
            game_event::event_t::CREATURE_KILL_IN_FIELD);   
    notify(evt);
}

void GameFieldExcludedCells::initField_(int width, int height) {
    field_ = decltype(field_)();
    for (int i = 0; i < height; ++i) {
        std::vector<std::unique_ptr<cell::ICell>> r;
        for (int j = 0; j < width; ++j) {
            r.emplace_back(cellFactory_->createCell());
        }
        field_.emplace_back(std::move(r));
    }
}

}