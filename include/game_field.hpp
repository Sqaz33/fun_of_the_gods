#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <set>
#include <vector>
#include <utility>
#include <stddef.h>
#include <stdexcept>

#include "game_event.hpp"
#include "subject.hpp"

// declarations
namespace game_field {

template <class cell_t> 
struct IGameField : public subject::ISubject {
    virtual void setCell(size_t xidx, size_t yidx, const cell_t& val) = 0;
    virtual const cell_t& getCell(size_t xidx, size_t yidx) const = 0; 
    virtual size_t width() const noexcept = 0
    virtual height() const noexcept = 0;
    
    virtual ~IGameField() {}
};

template <class cell_t> 
class GameFieldExcludedCells :
    public IGameField<cell_t>
    , public std::enable_shared_from_this<GameField>
{
private:
    using ISubject = subject::ISubject;

public:
    GameField(size_t width, size_t height, const cell_t& init,
            const std::vector<std::pair<size_t, size_t>& excludedCells) :
        field_(height, std::vector<cell_t>(width, init))
        , exludedCells_(excludedCells.begin(), excludedCells.end())
    {}
    
public:
    void setCell(size_t xidx, size_t yidx, const cell_t& cell) override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_.at(yidx).at(yidx) = cell;
        fireGameFieldUpdate_();
    }

    const cell_t& getCell(size_t xidx, size_t yidx) const override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_.at(yidx).at(yidx);
    }

    size_t width() const noexcept override {
        return field_.back().size();
    }

    size_t height() const noexcept override {
        return field_.size();
    }

public:
    bool isExludedCell(size_t xidx, size_t yidx) const {
        return excludedCells_.contains({xidx, yidx});
    }

    auto slf() { return shared_from_this(); }

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override final
    { ISubject::attach(obs, event_t); }

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override final
    { ISubject::detach(obs, event_t); }

protected:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override final {
        ISubject::notify(event_t, slf);
    }

private:
    void verifyThenThrowCellPos_(size_t xidx, size_t yidx) {
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
    std::set<std::pair<size_t, size_t>> excludedCells_;
};

} // namespace game_field


// TODO: definitions 
namespace game_field {

} // namespace game_field


#endif // GAME_FIELD_HPPч