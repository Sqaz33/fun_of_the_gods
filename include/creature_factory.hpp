/**
 * @file creature_factory.hpp
 * @brief Header file for the Creature Factory interface and implementation.
 */

#ifndef CREATURE_FACTORY_HPP
#define CREATURE_FACTORY_HPP

#include <memory>

#include "creature.hpp"

namespace factory {

/**
 * @interface ICreatureFactory
 * @brief Interface for a factory that creates creature objects.
 * 
 * This interface defines the contract for any factory implementation
 * that needs to create instances of ICreature.
 */
struct ICreatureFactory {
    /**

     */
    virtual std::unique_ptr<creature::ICreature> createCreature(
        std::shared_ptr<player::Player> player) const = 0;
    
    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ICreatureFactory() = default;
};

/**
 * @class CreatureFactory
 * @brief Concrete implementation of the ICreatureFactory interface.
 * 
 * This class provides a basic implementation of a factory that creates
 * new creature instances. The actual type of creature created depends
 * on the implementation of the ICreature interface.
 */
class CreatureFactory : public ICreatureFactory {
public:
    /**

     */
    std::unique_ptr<creature::ICreature> createCreature(
        std::shared_ptr<player::Player> player) const override;
};

} // namespace factory

#endif // CREATURE_FACTORY_HPP