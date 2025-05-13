#ifndef CREATURE_FACTORY_HPP
#define CREATURE_FACTORY_HPP

#include <memory>

#include "creature.hpp"

namespace factory {

struct ICreatureFactory {
    virtual std::unique_ptr<creature::ICreature>
    createCreature() const = 0;
    virtual ~ICreatureFactory() {} 
};

class CreatureFactory : public ICreatureFactory {
public:
    std::unique_ptr<creature::ICreature>
    createCreature() const override;
};

} // namespace factory

#endif // CREATURE_FACTORY_HPP