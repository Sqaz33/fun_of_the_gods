#include "creature.hpp"

namespace creature {

Creature::Creature(int id) : id_(id) {}

void Creature::kill() noexcept {
    isAlive_ = false;
}

void Creature::revive() noexcept {
    isAlive_ = true;
}

bool Creature::isAlive() const noexcept {
    return isAlive_;
}

int Creature::id() const noexcept {
    return id_;
}
void Creature::setId(int id) noexcept {
    id_ = id;
}

bool Creature::operator<(const Creature& cr) {
    return id_ < cr.id_;
}


template <>
Creature nextCreature<Creature>() {
    thread_local int curId = 0;
    return Creature(curId++);
}


} // namespace creature