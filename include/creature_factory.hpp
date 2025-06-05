#ifndef CREATURE_FACTORY_HPP
#define CREATURE_FACTORY_HPP

#include <memory>

#include "creature.hpp"

namespace factory {

struct ICreatureFactory {
    virtual std::unique_ptr<creature::ICreature> createCreature(
        std::shared_ptr<player::Player> player) const = 0;
    virtual ~ICreatureFactory() = default;
};

class CreatureFactory : public ICreatureFactory {
public:
    std::unique_ptr<creature::ICreature> createCreature(
        std::shared_ptr<player::Player> player) const override;
};

} // namespace factory

#endif // CREATURE_FACTORY_HPP