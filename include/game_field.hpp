#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <set>
#include <vector>
#include <utility>
#include <tuple>
#include <stdexcept>

#include "game_event.hpp"
#include "subject.hpp"

// declarations
namespace game_field {

template <class cell_t> 
struct IGameField : public subject::ISubject {
    virtual void setCell(int xidx, int yidx, const cell_t& cell) = 0;
    virtual const cell_t& getCell(int xidx, int yidx) const = 0; 
    virtual cell_t& getCell(int xidx, int yidx) = 0; 
    virtual void clear() = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
    virtual ~IGameField() {}
};

template <class cell_t> 
class GameFieldExcludedCells :
    public IGameField<cell_t>
    , public std::enable_shared_from_this<
                        GameFieldExcludedCells<cell_t>>
{
private:
    using ISubject = subject::ISubject;

public:
    GameFieldExcludedCells(int width, int height, const cell_t& init,
            const std::vector<std::pair<int, int>>& excludedCells) :
        field_(height, std::vector<cell_t>(width, init))
        , excludedCells_(excludedCells.begin(), excludedCells.end())
    {}
    
public:
    void setCell(int xidx, int yidx, const cell_t& cell) override {
        verifyThenThrowCellPos_(xidx, yidx);
        field_.at(yidx).at(yidx) = cell;
        fireGameFieldUpdate_();
    }

    const cell_t& getCell(int xidx, int yidx) const override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_.at(yidx).at(yidx);
    }

    cell_t& getCell(int xidx, int yidx) override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_.at(yidx).at(yidx);
    }

    void clear() override {
        field_.clear();
    }

    int width() const noexcept override {
        return field_.back().size();
    }

    int height() const noexcept override {
        return field_.size();
    }

public:
    bool isExludedCell(int xidx, int yidx) const {
        return excludedCells_.contains({xidx, yidx});
    }

    auto slf() { return shared_from_this(); }

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override
    { ISubject::attach(obs, event_t); }

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override
    { ISubject::detach(obs, event_t); }

protected:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override {
        ISubject::notify(event_t, slf);
    }

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) {
        if (isExludedCell(xidx, yidx)) {
            throw std::logic_error("Accessing a forbidden cell.");
        }
    }

    void fireGameFieldUpdate_() {
        notify(static_cast<int>(
                game_event::event_t::GAME_FIELD_UPDATE), 
               slf());
    }

private:
    std::vector<std::vector<cell_t>> field_;
    std::set<std::pair<int, int>> excludedCells_;
};

} // namespace game_field


// TODO: definitions 
namespace game_field {

} // namespace game_field


#endif // GAME_FIELD_HPPч