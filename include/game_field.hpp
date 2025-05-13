#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <set>
#include <vector>
#include <utility>
#include <stdexcept>

#include "cell.hpp"
#include "game_event.hpp"
#include "subject.hpp"
#include "creature_factory.hpp" 


namespace game_field {

struct IGameField  {
    virtual const creature::ICreature& getCreatureByCell(int xidx, int yidx) const = 0; 
    virtual void reviveCreatureInCell(int xidx, int yidx, int id) = 0;
    virtual void killCreatureInCell(int xidx, int yidx) = 0;
    virtual void clearCell(int xidx, int yidx) = 0;
    virtual void clear() = 0;
    virtual std::pair<int, int> lastAffectedCell() const noexcept = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
    virtual ~IGameField() {}
};

class GameFieldExcludedCells :
    public IGameField
    , public std::enable_shared_from_this<GameFieldExcludedCells>
    , public subject::ISubject
{
private:
    using ISubject = subject::ISubject;
    using ICreatureFactory = factory::ICreatureFactory;

public:
    GameFieldExcludedCells(
        int width, int height, 
        const std::vector<std::pair<int, int>>& excludedCells,
        std::unique_ptr<factory::ICreatureFactory> creatFactory);
    
public:
    const creature::ICreature& getCreatureByCell(int xidx, int yidx) const;
    void reviveCreatureInCell(int xidx, int yidx, int id);
    void killCreatureInCell(int xidx, int yidx);
    void clearCell(int xidx, int yidx);
    void clear();
    std::pair<int, int> lastAffectedCell() const noexcept;
    int width() const noexcept;
    int height() const noexcept;

public:
    bool isExludedCell(int xidx, int yidx) const;
    std::shared_ptr<GameFieldExcludedCells> slf();

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override;

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override;

protected:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override;

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) const;
    void fireFieldClear_();
    void fireCellClear_();
    void fireCreatureRevive_();
    void fireCreatureKill_();
    void prepareField_();
    
private:
    std::vector<std::vector<cell::Cell>> field_;
    std::set<std::pair<int, int>> excludedCells_;
    std::pair<int, int> lastAffectedCell_ = {-1, -1};
    std::unique_ptr<factory::ICreatureFactory> creatFactory_;
};

} // namespace game_field

#endif // GAME_FIELD_HPP