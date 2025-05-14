#ifndef GAME_FIELD_AREA_HPP
#define GAME_FIELD_AREA_HPP

#include "game_field.hpp"

namespace game_field_area {

struct IGameFieldArea {
    IGameFieldArea(
            std::pair<int, int> upperLeftCorner, 
            std::pair<int, int> lowerRightCorner):
        upperLeftCorner_(upperLeftCorner)
        , lowerRightCorner_(lowerRightCorner)
    {}
    
    virtual void lock() noexcept = 0;
    virtual void unlock() noexcept = 0;
    virtual bool isCellAvailable(int xidx, int yidx) const = 0;
    virtual void reviveCreatureInCell(int xidx, int yidx, int id) = 0;
    virtual void killCreatureInCell(int xidx, int yidx) = 0;
    virtual const creature::ICreature& 
        getCreatureByCell(int xidx, int yidx) const = 0; 
    virtual void clearCell(int xidx, int yidx) = 0;
    virtual void clear() = 0;
    virtual std::pair<int, int> upperLeftCorner() const noexcept = 0;
    virtual std::pair<int, int> lowerRightCorner() const noexcept = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
    virtual ~IGameFieldArea() {} 

protected:
    std::pair<int, int> upperLeftCorner_;
    std::pair<int, int> lowerRightCorner_;
};

class GameFieldExcludedCellsArea : public IGameFieldArea
{
    using GameFieldExcludedCells = game_field::GameFieldExcludedCells;
    using ICreature = creature::ICreature;
    
public:
    GameFieldExcludedCellsArea(
        std::shared_ptr<GameFieldExcludedCells> field,
        std::pair<int, int> upperLeftCorner, 
        std::pair<int, int> lowerRightCorner);

public:
    void lock() noexcept override;
    void unlock() noexcept override;
    bool isCellAvailable(int xidx, int yidx) const override;
    void reviveCreatureInCell(int xidx, int yidx, int id) override;
    void killCreatureInCell(int xidx, int yidx) override;
    const ICreature& getCreatureByCell(int xidx, int yidx) const override;
    void clearCell(int xidx, int yidx) override;
    void clear() override;
    std::pair<int, int> upperLeftCorner() const noexcept override;
    std::pair<int, int> lowerRightCorner() const noexcept override;
    int width() const noexcept override;
    int height() const noexcept override;

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) const;

private:
    bool isLocked_ = true; 
    std::shared_ptr<GameFieldExcludedCells> field_;
};


} // namespace game_field_area

#endif 