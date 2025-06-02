#include "creature.hpp"

#include "player.hpp"

namespace creature {

Creature::Creature(
    std::shared_ptr<player::Player> player) :
    player_(player) 
{}

std::shared_ptr<player::Player> Creature::player() {
    return player_;
}

const std::shared_ptr<player::Player> Creature::player() const {
    return player_;
}

bool Creature::operator<(const Creature& cr) {
    return player_->id() < cr.player_->id();
}

} // namespace creature