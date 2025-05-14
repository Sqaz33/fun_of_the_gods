#include "creature.hpp"

namespace creature {

void Creature::kill() noexcept {
    id_ = idDead;
    isAlive_ = false;
}

void Creature::revive(int id) noexcept {
    id_ = id;
    isAlive_ = true;
}

bool Creature::isAlive() const noexcept {
    return isAlive_;
}

int Creature::id() const noexcept {
    return id_;
}

bool Creature::operator<(const Creature& cr) {
    return id_ < cr.id_;
}

int nextCreatureId() {
    static int id = idDead;
    return ++id;
}

} // namespace creature