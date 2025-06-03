#include "creature.hpp"

#include "player.hpp"

namespace creature {

Creature::Creature(
    const std::shared_ptr<player::Player> player) :
    player_(player) 
{}

const std::shared_ptr<player::Player> Creature::player() const {
    return player_;
}

bool Creature::operator<(const Creature& cr) const {
    return player_->id() < cr.player_->id();
}

} // namespace creature