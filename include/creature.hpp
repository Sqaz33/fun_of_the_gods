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
    Creature(const std::shared_ptr<player::Player> player); 

public:
    const std::shared_ptr<player::Player> player() const override;

public:
    /**
     * @brief Compares creatures by their ID for ordering.
     * @param cr The creature to compare against.
     * @return true if this creature's ID is less than the other's.
     * 
     * @note Dead creatures (with idDead) are considered less than living ones.
     */
    bool operator<(const Creature& cr) const;

private:
    const std::shared_ptr<player::Player> player_;
};

} // namespace creature

#endif // CREATURE_HPP