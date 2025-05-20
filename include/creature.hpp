/**
 * @file creature.hpp
 * @brief Header file for the Creature interface and implementation.
 * 
 * Defines the base interface for creatures and a basic implementation
 * with life cycle management capabilities.
 */

#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <compare>
#include <string>

namespace creature {

/**
 * @interface ICreature
 * @brief Interface representing a creature with life cycle management.
 * 
 * Provides basic operations to control and query a creature's state.
 * All methods are noexcept as they shouldn't throw exceptions during
 * creature state management.
 */
struct ICreature {
    /**
     * @brief Kills the creature, making it inactive.
     */
    virtual void kill() noexcept = 0;
    
    /**
     * @brief Revives the creature with a new identifier.
     * @param id The new identifier to assign to the revived creature.
     */
    virtual void revive(int id) noexcept = 0;
    
    /**
     * @brief Checks if the creature is currently alive.
     * @return true if the creature is alive, false otherwise.
     */
    virtual bool isAlive() const noexcept = 0;
    
    /**
     * @brief Gets the creature's current identifier.
     * @return The creature's ID or idDead if the creature is dead.
     */
    virtual int id() const noexcept = 0;

    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ICreature() = default;
};

/**
 * @brief Generates the next available creature ID.
 * @return A unique integer identifier for a new creature.
 */
int nextCreatureId();

/**
 * @brief Constant representing the ID of a dead creature.
 */
constexpr int idDead = -1;

/**
 * @class Creature
 * @brief Basic implementation of the ICreature interface.
 * 
 * Manages creature state including life status and identifier.
 * Provides comparison operation for sorting creatures by ID.
 */
class Creature : public ICreature {
public:
    /**
     * @brief Kills the creature, setting its ID to idDead.
     */
    void kill() noexcept override;
    
    /**
     * @brief Revives the creature with a new identifier.
     * @param id The new identifier to assign (must not be idDead).
     */
    void revive(int id) noexcept override;
    
    /**
     * @brief Checks the creature's life status.
     * @return true if alive, false if dead.
     */
    bool isAlive() const noexcept override;
    
    /**
     * @brief Gets the creature's current identifier.
     * @return The active ID or idDead if the creature is dead.
     */
    int id() const noexcept override;
    
    /**
     * @brief Compares creatures by their ID for ordering.
     * @param cr The creature to compare against.
     * @return true if this creature's ID is less than the other's.
     * 
     * @note Dead creatures (with idDead) are considered less than living ones.
     */
    bool operator<(const Creature& cr);

private:
    int id_ = idDead;        ///< Current creature identifier
    bool isAlive_ = false;   ///< Life status flag
};

} // namespace creature

#endif // CREATURE_HPP