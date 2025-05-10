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
    
    virtual void lock() noexcept = 0;
    virtual void unlock() noexcept = 0;
    virtual bool isCellAvailable(int xidx, int yidx) const noexcept = 0;
    virtual void setCell(int xidx, int yidx, const cell_t& cell) = 0;
    virtual const cell_t& getCell(int xidx, int yidx) const = 0;
    virtual cell_t& getCell(int xidx, int yidx) = 0;
    virtual std::pair<int, int> getUpperLeftCorner() const noexcept {
        return upperLeftCorner_; 
    }
    virtual std::pair<int, int> getLowerRightCorner() const noexcept {
        return lowerRightCorner_;
    }
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    virtual ~IGameFieldArea() {} 

protected:
    std::shared_ptr<game_field::IGameField<cell_t>> field_;
    std::pair<int, int> upperLeftCorner_;
    std::pair<int, int> lowerRightCorner_;
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
    void lock() noexcept override {
        isLocked_ = true;
    }
    
    void unlock() noexcept override {
        isLocked_ = false;
    }

    bool isCellAvailable(int xidx, int yidx) const noexcept override {
        retrun IGameFieldArea::isCellAvailable(xidx, yidx) && !isLocked_;
    }

    void setCell(int xidx, int yidx, const cell_t& cell) override {
        verifyThenThrowCellPos_(xidx, yidx);
        field_->setCell(xidx, yidx, cell);
    }

    const cell_t& getCell(int xidx, int yidx) const override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_->getCell(xidx, yidx);
    }

    cell_t& getCell(int xidx, int yidx) override {
        verifyThenThrowCellPos_(xidx, yidx);
        return field_->getCell(xidx, yidx);
    }

    int width() const noexcept override {
        return lowerRightCorner_.first 
                - upperLeftCorner_.first
                - 1;
    }

    int height() const noexcept override {
        return upperLeftCorner_.second 
                - lowerRightCorner.second
                - 1;
    }

private:
    void verifyThenThrowCellPos_(int xidx, int yidx) {
        if (isExludedCell(xidx, yidx)) {
            throw std::logic_error("Accessing a forbidden cell.");
        }
    }

private:
    bool isLocked_ = false; 
};

//TODO: отдельный namespace для definitions

} // namespace game_field_area

#endif 