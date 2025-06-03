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

bool Cell::hasCreature() const {
    return static_cast<bool>(creat_);
}

void Cell::removeCreature() {
    creat_.release();
}



void Cell::addNeighbors(const ICell* ne) {
    neighbors_.push_back(ne);
}

std::map<const std::shared_ptr<player::Player>, int> 
    Cell::countNeighborsCreatures() const 
{   
    std::map<const std::shared_ptr<player::Player>, int> res;
    for (auto c : neighbors_) {
        if (c->hasCreature()) {
            auto&& cr = c->creature();;
            auto it = res.find(cr.player());
            if (it != res.end()) {
                ++it->second;
            } else {
                res[cr.player()] = 1;
            }
        }
    }
    return res;
}


} // namespace cell