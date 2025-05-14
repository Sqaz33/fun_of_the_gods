#include "cell.hpp"

#include <stdexcept>

namespace cell {
    
creature::ICreature& Cell::creature() {
    return *creat_;
}

const creature::ICreature& Cell::creature() const {
    return *creat_;
}

void Cell::setCreature(
    std::unique_ptr<creature::ICreature> creat) {
    creat_ = std::move(creat);
}

} // namespace cell