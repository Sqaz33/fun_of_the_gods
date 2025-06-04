#include "player.hpp"

namespace {
    
    using IGameFieldArea = game_field_area::IGameFieldArea;

} // namespace

namespace player {

Player::Player(int id, const std::string& name) : 
    id_(id) 
    , name_(name)    
{}

void Player::setFieldArea(
        std::unique_ptr<IGameFieldArea> area)
{ area_ = std::move(area); }

IGameFieldArea& Player::fieldArea(){
    return *area_;
}

int Player::id() const noexcept {
    return id_;
}

const std::string& Player::name() const {
    return name_;
}

void Player::tapOnCreature(int x, int y) {
    if (area_->isCellAvailable(x, y)) {
        if (area_->hasCreatureInCell(x, y)) {
            auto&& cr = area_->getCreatureByCell(x, y);
            if (cr.player()->id() == id_) {
                area_->removeCreatureInCell(x, y);
            }
        } else {
            area_->setCreatureInCell(x, y, slf());
        }
    }
}

std::shared_ptr<Player> Player::slf() {
    return shared_from_this();
}


} // namespace player
