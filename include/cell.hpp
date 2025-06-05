#ifndef CELL_HPP
#define CELL_HPP

#include <memory>
#include <map>
#include <vector>

#include "creature.hpp"

namespace cell {

struct ICell {
    virtual const creature::ICreature& creature() const = 0;
    virtual creature::ICreature& creature() = 0;
    virtual void setCreature(std::unique_ptr<creature::ICreature> creat) = 0;
    virtual bool hasCreature() const = 0;
    virtual void removeCreature() = 0;
    virtual void addNeighbors(const ICell* ne) = 0;
    virtual std::map<const std::shared_ptr<player::Player>, int> 
        countNeighborsCreatures() const = 0;
    virtual ~ICell() = default;
};

class Cell : public ICell {
public:
    const creature::ICreature& creature() const override;
    creature::ICreature& creature() override;
    void setCreature(std::unique_ptr<creature::ICreature> creat) override;
    bool hasCreature() const override;
    void removeCreature() override;
    void addNeighbors(const ICell* ne) override;    
    std::map<const std::shared_ptr<player::Player>, int> 
        countNeighborsCreatures() const override;

private:
    std::unique_ptr<creature::ICreature> creat_; 
    std::vector<const ICell*> neighbors_;
};

} // namespace cell

#endif // CELL_HPP