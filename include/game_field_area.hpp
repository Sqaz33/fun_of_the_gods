#ifndef GAME_FIELD_AREA_HPP
#define GAME_FIELD_AREA_HPP

#include "game_field.hpp"

namespace game_field_area {

template <class cell_t>
struct IGameFieldArea {
    IGameFieldArea(
            std::shared_ptr<game_field::GameField<cell_t>> field,
            std::pair<int, int> upperLeftCorner, 
            std::pair<int, int> lowerRightCorner):
        field_(field)
        , upperLeftCorner_(upperLeftCorner)
        , lowerRightCorner_(lowerRightCorner)
    {}

    virtual bool isCellAvailable(int xidx, int yidx) const noexcept = 0;
    virtual void setCell(int xidx, int yidx, const cell_t& cell) = 0;
    virtual cell_t getCell(int xidx, int yidx) const = 0;

    virtual ~IGameFieldArea() {} 

protected:
    std::shared_ptr<game_field::IGameField<cell_t>> field_;
    std::pair<int> upperLeftCorner_;
    std::pair<int> lowerRightCorner_;
};

template <class cell_t>
bool IGameFieldArea<cell_t>::isCellAvailable(
    int xidx, int yidx) const noexcept 
{
    return xidx >= upperLeftCorner_.first && 
           xidx <= lowerRightCorner_.first &&
           yidx <= upperLeftCorner_.second &&
           yidx >= lowerRightCorner_.first;
}


template <class cell_t>
class GameFieldArea : 
    public IGameFieldArea<cell_t> 
{
public:
    GameFieldArea(
            std::shared_ptr<game_field::GameField<cell_t>> field,
            std::pair<int, int> upperLeftCorner, 
            std::pair<int, int> lowerRightCorner) :
        IGameFieldArea(field, upperLeftCorner, lowerRightCorner)
    {}

public:
    bool isCellAvailable(int xidx, int yidx) const noexcept override {
        retrun IGameFieldArea::isCellAvailable(xidx, yidx);
    }

    void setCell(int xidx, int yidx, const cell_t& cell) override {
        verifyThenThrowCellPos_(xidx, yidx);
        field_->setCell(xidx, yidx, cell);
    }

    cell_t getCell(int xidx, int yidx) const override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_->getCell(xidx, yidx);
    }

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) {
        if (isExludedCell(xidx, yidx)) {
            throw std::logic_error("Accessing a forbidden cell.");
        }
    }
};

//TODO: отдельный namespace для definitions

} // namespace game_field_area

#endif 