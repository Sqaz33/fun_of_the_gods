#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <compare>
#include <string>

namespace creature {

struct ICreature {
    virtual void kill() noexcept = 0;
    virtual void revive() noexcept = 0;
    virtual bool isAlive() const noexcept = 0;
    virtual int id() const noexcept = 0;
    virtual void setId(int id) noexcept = 0;

    virtual ~ICreature() {}
};

class Creature : public ICreature {
public:
    Creature() = default;
    Creature(int id); 

public:
    void kill() noexcept override;
    void revive() noexcept override;
    bool isAlive() const noexcept override;
    int id() const noexcept override;
    void setId(int id) noexcept override;
    bool operator<(const Creature& cr);

private:
    int id_ = -1;
    bool isAlive_ = false;
};

template <class creature_t> 
creature_t nextCreature();

template <>
Creature nextCreature<Creature>();


} // namespace creature

#endif // CREATURE_HPP