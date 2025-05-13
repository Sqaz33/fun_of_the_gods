#ifndef CELL_HPP
#define CELL_HPP

#include <memory>

#include "creature.hpp"

namespace cell {

struct ICell {
    virtual creature::ICreature& creature() noexcept = 0;
    virtual void setCreature(std::unique_ptr<creature::ICreature> creat) = 0;
    virtual ~ICell() {}
}; 

class Cell : public ICell {
public:
    creature::ICreature& creature() noexcept override;
    void setCreature(std::unique_ptr<creature::ICreature> creat) override;

private:
    std::unique_ptr<creature::ICreature> creat_;
};

} // namespace cell

#endif 