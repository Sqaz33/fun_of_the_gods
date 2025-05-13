#include "creature_factory.hpp"

namespace factory {
    
std::unique_ptr<creature::ICreature>
CreatureFactory::createCreature() const
{ return std::make_unique<creature::Creature>(); }

} // namespace factory 