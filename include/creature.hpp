#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <compare>
#include <string>
#include <memory>

namespace player {
    class Player;
} // namespace player

namespace creature {

struct ICreature {
    virtual const std::shared_ptr<player::Player> player() const = 0;
    virtual ~ICreature() = default;
};

class Creature : public ICreature {
public:
    Creature(const std::shared_ptr<player::Player> player); 

public:
    const std::shared_ptr<player::Player> player() const override;

public:
    bool operator<(const Creature& cr) const;

private:
    const std::shared_ptr<player::Player> player_;
};

} // namespace creature

#endif // CREATURE_HPP