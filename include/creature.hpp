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
#include <memory>

namespace player {
    class Player;
} // namespace player

namespace creature {

/**
 * @interface ICreature
 * @brief Interface 
 */
struct ICreature {
    /**
     */
    virtual std::shared_ptr<player::Player> player() = 0;
    virtual const std::shared_ptr<player::Player> player() const = 0;

    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ICreature() = default;
};

/**
 * @class Creature
 * @brief Basic 
 */
class Creature : public ICreature {
public:
    Creature(std::shared_ptr<player::Player> player); 

public:
    /**
     * @brief Gets the creature's current identifier.
     * @return The active ID or idDead if the creature is dead.
     */
    std::shared_ptr<player::Player> player() override;

    const std::shared_ptr<player::Player> player() const override;

public:
    /**
     * @brief Compares creatures by their ID for ordering.
     * @param cr The creature to compare against.
     * @return true if this creature's ID is less than the other's.
     * 
     * @note Dead creatures (with idDead) are considered less than living ones.
     */
    bool operator<(const Creature& cr);

private:
    std::shared_ptr<player::Player> player_;
};

} // namespace creature

#endif // CREATURE_HPP