#ifndef GAME_FIELD_HPP
#define GAME_FIELD_HPP

#include <set>
#include <vector>
#include <utility>
#include <stdexcept>
#include <array>

#include "game_event.hpp"
#include "subject.hpp"
#include "creature_factory.hpp"
#include "cell_factory.hpp"
#include "figure.hpp"

namespace player {
    class Player;
} // namespace player

namespace game_field {

struct IGameField {
    virtual const creature::ICreature& getCreatureByCell(int xidx, int yidx) const = 0;
    virtual void setCreatureInCell(int xidx, int yidx, 
                        std::shared_ptr<player::Player> player) = 0;
    virtual void removeCreatureInCell(int xidx, int yidx) = 0;
    virtual bool hasCreatureInCell(int xidx, int yidx) const = 0;
    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const = 0;
    virtual void clear() = 0;
    virtual std::pair<int, int> lastAffectedCell() const noexcept = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
    
    virtual ~IGameField() = default;
};

class GameFieldWithFigure :
    public IGameField,
    public std::enable_shared_from_this<GameFieldWithFigure>,
    public subject::ISubject
{
private:
    using ISubject = subject::ISubject;
    using ICreatureFactory = factory::ICreatureFactory;
    using IFigure = figure::IFigure;

public:
    GameFieldWithFigure(
        int width, int height,
        std::unique_ptr<factory::ICreatureFactory> creatFactory,
        std::unique_ptr<factory::ICellFactory> cellFactory,
        std::unique_ptr<IFigure> figure);
    
public:
    const creature::ICreature& getCreatureByCell(int xidx, int yidx) const override;
    void setCreatureInCell(int xidx, int yidx, 
            std::shared_ptr<player::Player> player) override;
    void removeCreatureInCell(int xidx, int yidx) override;
    bool hasCreatureInCell(int xidx, int yidx) const override;
    std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const override;
    void clear() override;
    std::pair<int, int> lastAffectedCell() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

public:
    bool isExcludedCell(int xidx, int yidx) const;

public:
    void attach(std::shared_ptr<observer::IObserver> obs, int event_t) override;
    void detach(std::weak_ptr<observer::IObserver> obs, int event_t) override;

private:
    void notify(int event_t) override;

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) const;
    void fireFieldClear_();
    void fireCreatureSet_();
    void fireCreatureRemove_();
    void initField_(int width, int height);
    void initCells_();
    
private:
    std::vector<std::vector<std::unique_ptr<cell::ICell>>> field_;
    std::pair<int, int> lastAffectedCell_ = {-1, -1};             
    std::unique_ptr<factory::ICreatureFactory> creatFactory_;     
    std::unique_ptr<factory::ICellFactory> cellFactory_;          
    std::unique_ptr<IFigure> figure_;

    static constexpr std::array<const std::pair<int, int>, 8> neighborsPos_ {{
        {-1,  -1}, {0, -1}, {1, -1},
        {-1,   0},          {1,  0},
        {-1,   1}, {0,  1}, {1,  1}  
    }};

};

} // namespace game_field

#endif // GAME_FIELD_HPP