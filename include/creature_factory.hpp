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
     * @brief Creates a new creature instance.
     * @return std::unique_ptr<creature::ICreature> A unique pointer to the newly created creature.
     */
    virtual std::unique_ptr<creature::ICreature> createCreature() const = 0;
    
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
     * @brief Creates a new creature instance.
     * @return std::unique_ptr<creature::ICreature> A unique pointer to the newly created creature.
     * 
     * @note The concrete type of the created creature depends on the implementation.
     *       This method must be implemented in derived classes to create specific creature types.
     */
    std::unique_ptr<creature::ICreature> createCreature() const override;
};

} // namespace factory

#endif // CREATURE_FACTORY_HPP