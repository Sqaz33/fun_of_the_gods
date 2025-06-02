#include "creature_factory.hpp"

namespace factory {
    
std::unique_ptr<creature::ICreature>
CreatureFactory::createCreature(
    std::shared_ptr<player::Player> player) const
{ return std::make_unique<creature::Creature>(player); }

} // namespace factory 