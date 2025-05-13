#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <compare>
#include <string>

namespace creature {

struct ICreature {
    virtual void kill() noexcept = 0;
    virtual void revive(int id) noexcept = 0;
    virtual bool isAlive() const noexcept = 0;
    virtual int id() const noexcept = 0;

    virtual ~ICreature() {}
};

int nextCreatureId();
constexpr int idDead = -1;

class Creature : public ICreature {
public:
    void kill() noexcept override;
    void revive(int id) noexcept override;
    bool isAlive() const noexcept override;
    int id() const noexcept override;
    bool operator<(const Creature& cr);

private:
    int id_ = idDead;
    bool isAlive_ = false;
};

} // namespace creature

#endif // CREATURE_HPP