#ifndef GAME_FIELD_AREA_HPP
#define GAME_FIELD_AREA_HPP

#include "game_field.hpp"

namespace game_field_area {

struct IGameFieldArea {
    IGameFieldArea(
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner):
        upperLeftCorner_(upperLeftCorner),
        lowerRightCorner_(lowerRightCorner)
    {}
    
    virtual void lock() noexcept = 0;
    virtual void unlock() noexcept = 0;
    virtual bool isCellAvailable(int xidx, int yidx) const = 0;
    virtual void setCreatureInCell(int xidx, int yidx, 
                std::shared_ptr<player::Player> player) = 0;
    virtual void removeCreatureInCell(int xidx, int yidx) = 0;
    virtual bool hasCreatureInCell(int xidx, int yidx) const = 0;
    virtual const creature::ICreature& 
        getCreatureByCell(int xidx, int yidx) const = 0;
    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const = 0;
    virtual std::set<std::shared_ptr<player::Player>> 
        checkCreatureInArea() const = 0;
    virtual void clear() = 0;
    virtual std::pair<int, int> upperLeftCorner() const noexcept = 0;
    virtual std::pair<int, int> lowerRightCorner() const noexcept = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    virtual ~IGameFieldArea() = default;

protected:
    std::pair<int, int> upperLeftCorner_;   
    std::pair<int, int> lowerRightCorner_;  
};

class GameFieldWithFigureArea : public IGameFieldArea {
    using GameFieldWithFigure = game_field::GameFieldWithFigure;
    using ICreature = creature::ICreature;
    
public:
    GameFieldWithFigureArea(
        std::shared_ptr<GameFieldWithFigure> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner);

    void lock() noexcept override;
    void unlock() noexcept override;
    bool isCellAvailable(int xidx, int yidx) const override;
    void setCreatureInCell(int xidx, int yidx, 
            std::shared_ptr<player::Player> player) override;
    void removeCreatureInCell(int xidx, int yidx) override;
    bool hasCreatureInCell(int xidx, int yidx) const override;
    const ICreature& getCreatureByCell(int xidx, int yidx) const override;
    std::map<const std::shared_ptr<player::Player>, int> 
        countCellNeighborsCreatures(int xidx, int yidx) const override;
    std::set<std::shared_ptr<player::Player>> 
        checkCreatureInArea() const override;
    void clear() override;
    std::pair<int, int> upperLeftCorner() const noexcept override;
    std::pair<int, int> lowerRightCorner() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) const;

private:
    bool isLocked_ = true; 
    std::shared_ptr<GameFieldWithFigure> field_; 
};

} // namespace game_field_area

#endif // GAME_FIELD_AREA_HPP