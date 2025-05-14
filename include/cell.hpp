#ifndef CELL_HPP
#define CELL_HPP

#include <memory>

#include "creature.hpp"

namespace cell {

struct ICell {
    virtual const creature::ICreature& creature() const = 0;
    virtual creature::ICreature& creature() = 0;
    virtual void setCreature(std::unique_ptr<creature::ICreature> creat) = 0;
    virtual ~ICell() {}
}; 

class Cell : public ICell {
public:
    const creature::ICreature& creature() const override;
    creature::ICreature& creature() override;
    void setCreature(std::unique_ptr<creature::ICreature> creat) override;

private:
    std::unique_ptr<creature::ICreature> creat_;
};

} // namespace cell

#endif 