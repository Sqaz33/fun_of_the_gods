#include "game_field.hpp"

namespace game_field {

GameFieldExcludedCells::GameFieldExcludedCells(
        int width, int height, 
        const std::vector<std::pair<int, int>>& excludedCells,
        std::unique_ptr<factory::ICreatureFactory> creatFactory) :
    excludedCells_(excludedCells.begin(), excludedCells.end())
    , creatFactory_(std::move(creatFactory))
{   
    initField_(width, height); 
    prepareField_(); 
}

void GameFieldExcludedCells::reviveCreatureInCell(
    int xidx, int yidx, int id)
{   
    verifyThenThrowCellPos_(xidx, yidx);
    field_.at(yidx).at(xidx).creature().revive(id); 
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureRevive_();
}

void GameFieldExcludedCells::killCreatureInCell(
    int xidx, int yidx)
{ 
    verifyThenThrowCellPos_(xidx, yidx);
    field_.at(yidx).at(xidx).creature().kill(); 
    lastAffectedCell_ = { xidx, yidx }; 
    fireCreatureKill_();
}

const creature::ICreature&
GameFieldExcludedCells::getCreatureByCell(int xidx, int yidx) const {
    verifyThenThrowCellPos_(xidx, yidx);
    return field_.at(yidx).at(xidx).creature();
}

void GameFieldExcludedCells::clear() {
    int w = width();
    int h = height();
    field_.clear();
    initField_(w, h);
    prepareField_();
    fireFieldClear_();
}

void GameFieldExcludedCells::clearCell(int xidx, int yidx) {
    verifyThenThrowCellPos_(xidx, yidx);
    field_.at(yidx).at(xidx).setCreature(
        creatFactory_->createCreature());
    lastAffectedCell_ = { xidx, yidx }; 
    fireCellClear_();
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

std::shared_ptr<GameFieldExcludedCells>
GameFieldExcludedCells::slf() 
{ return shared_from_this(); }

void GameFieldExcludedCells::attach(
    std::shared_ptr<observer::IObserver> obs, int event_t)
{ ISubject::attach(obs, event_t); }

void GameFieldExcludedCells::detach(
    std::weak_ptr<observer::IObserver> obs, int event_t)
{ ISubject::detach(obs, event_t); }

void GameFieldExcludedCells::notify(
    int event_t, std::weak_ptr<ISubject> slf) {
    ISubject::notify(event_t, slf);
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
    notify(evt, slf());
}

void GameFieldExcludedCells::fireCellClear_() {
    int evt = static_cast<int>(
        game_event::event_t::CELL_CLEAR_IN_FIELD);   
    notify(evt, slf());
}

void GameFieldExcludedCells::fireCreatureRevive_() {
    int evt = static_cast<int>(
        game_event::event_t::CREATURE_REVIVE_IN_FIELD);   
    notify(evt, slf());
}

void GameFieldExcludedCells::fireCreatureKill_() {
    int evt = static_cast<int>(
            game_event::event_t::CREATURE_KILL_IN_FIELD);   
    notify(evt, slf());
}

void GameFieldExcludedCells::prepareField_() {
    for (auto&& row : field_) {
        for (auto&& cell : row) {
            cell.setCreature(creatFactory_->createCreature());
        }
    }
}

void GameFieldExcludedCells::initField_(int width, int height) {
    field_ = decltype(field_)();
    for (int i = 0; i < height; ++i) {
        std::vector<cell::Cell> r;
        for (int j = 0; j < width; ++j) {
            r.emplace_back(cell::Cell());
        }
        field_.emplace_back(std::move(r));
    }
}

}