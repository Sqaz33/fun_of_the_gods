#include "creature.hpp"

namespace creature {

Creature nextCreature() {
    thread_local int curId = 0;
    return Creature(curId++);
}

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

const std::string& Creature::color() const noexcept {
    return color_;
}

void Creature::setColor(const std::string& color) {
    color_ = color;
}

bool Creature::operator<(const Creature& cr) {
    return id_ < cr.id_;
}

Creature::Creature(int id) : id_(id) {}
Creature::Creature(int id, const std::string& color) : 
    id_(id)
    , color_(color)
{}

} // namespace creature